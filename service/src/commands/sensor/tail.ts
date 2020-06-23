import DeviceCommand from '../../lib/deviceCommand';
import MqttClient from '../../lib/mqttClient';
import { flags } from '@oclif/command';
import { AsyncMqttClient } from 'async-mqtt';
import { LightLevelReading } from '../../types/lightLevel';
import { promises as fs } from 'fs';

/**
 * Tail sensor readings
 */
export default class Tail extends DeviceCommand {
  static description = 'Tail sensor readings';
  static flags = {
    ...DeviceCommand.flags,
    inject: flags.boolean({ description: 'Inject a message into the readings stream', hidden: true }),
    file: flags.string({ description: 'File to write measurements to' }),
    force: flags.boolean({ description: 'Overwrite if file exists', dependsOn: ['file'] }),
  };
  static args = [Tail.deviceArg];

  /**
   * Log with timestamp and tag
   */
  private static log(val: LightLevelReading): void {
    const redPct = (val.red / 65535).toFixed(3);
    const greenPct = (val.green / 65535).toFixed(3);
    const bluePct = (val.blue / 65535).toFixed(3);
    const color = `(${val.red.toString(16)},${val.green.toString(16)},${val.blue.toString(
      16
    )})/(${redPct},${greenPct},${bluePct})`;
    console.log(`[${new Date().toISOString()}] ${color} at ${val.lux.toLocaleString()} lux`);
  }

  /**
   * Check if the output file exists
   * @param file The output fule
   */
  private static async fileExists(file: string): Promise<boolean> {
    try {
      await fs.access(file);
      return true;
    } catch {
      return false;
    }
  }

  /**
   * Write the header to the output file
   * @param file The output file
   */
  private static async writeHeader(file: string): Promise<void> {
    const header = `IsoDate,ExcelDate,Lux,LuxWhite,R,G,B\n`;
    await fs.writeFile(file, header);
  }

  /**
   * Write a log line to the output file
   * @param file The output file
   * @param val The value to write
   */
  private static async writeLog(file: string, val: LightLevelReading): Promise<void> {
    const now = new Date();
    const isoDate = now.toISOString();
    const excelDateNum = 25569.0 + (now.getTime() - now.getTimezoneOffset() * 60 * 1000) / (1000 * 60 * 60 * 24);
    const excelDate = excelDateNum.toString().substr(0, 20);
    const data = `${isoDate},${excelDate},${val.lux},${val.luxWhite},${val.red},${val.green},${val.blue}\n`;
    await fs.appendFile(file, data);
  }

  /**
   * Stream log messages
   * @param client MQTT client
   * @param topic  Device log topic
   * @param device Device ID
   */
  private async stream(client: AsyncMqttClient, topic: string, device: string, file?: string): Promise<void> {
    client.on('message', (_topic, message) => {
      try {
        const body = JSON.parse(message.toString()) as LightLevelReading;
        Tail.log(body);
        if (file) {
          Tail.writeLog(file, body);
        }
      } catch (err) {
        // Do nothing - only log normal values
      }
    });

    await client.subscribe(topic);
    console.log(`Streaming readings from ${device}`);
    console.log('---');
    if (file) {
      await Tail.writeHeader(file);
    }
  }

  /**
   * Inject a log message into a device log stream
   * @param client MQTT client
   * @param topic Device log topic
   */
  private async inject(client: AsyncMqttClient, topic: string): Promise<void> {
    const message: LightLevelReading = {
      lux: 13000,
      luxWhite: 13000,
      red: 0xfff0,
      green: 0xff,
      blue: 0xff00,
    };
    await client.publish(topic, JSON.stringify(message));
    await client.end();
    console.log(`Reading injected to ${topic}`);
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Tail);

    const topic = `${MqttClient.topicNamespace}/device/${args.device}/lightLevel`;

    if (flags.file !== undefined) {
      if (await Tail.fileExists(flags.file)) {
        if (!flags.force) {
          this.error(`File '${flags.file}' already exists; remove first or pass --force`);
        }
      }
    }

    try {
      const client = await MqttClient.connectToBroker(flags.address, flags.port, await this.getAccessKey());

      if (flags.inject) {
        this.inject(client, topic);
      } else {
        this.stream(client, topic, args.device, flags.file);
      }
    } catch (err) {
      this.error(`Request failed: ${err}`);
    }
  }
}
