import DeviceCommand from '../../lib/deviceCommand';
import MqttClient from '../../lib/mqttClient';
import { flags } from '@oclif/command';
import { AsyncMqttClient } from 'async-mqtt';
import { LightLevelReading } from '../../types/lightLevel';

/**
 * Tail sensor readings
 */
export default class Tail extends DeviceCommand {
  static description = 'Tail sensor readings';
  static flags = {
    ...DeviceCommand.flags,
    inject: flags.boolean({ description: 'Inject a message into the readings stream', hidden: true }),
  };
  static args = [Tail.deviceArg];

  /**
   * Log with timestamp and tag
   */
  private static log(val: LightLevelReading): void {
    const redPct = (val.red / 65535).toFixed(3);
    const greenPct = (val.green / 65535).toFixed(3);
    const bluePct = (val.blue / 65535).toFixed(3);
    const color = `(${val.red.toString(16)},${val.green.toString(16)},${val.blue.toString(16)})/(${redPct},${greenPct},${bluePct})`;
    console.log(`[${new Date().toISOString()}] ${color} at ${val.lux.toLocaleString()} lux`);
  }

  /**
   * Stream log messages
   * @param client MQTT client
   * @param topic  Device log topic
   * @param device Device ID
   */
  private async stream(client: AsyncMqttClient, topic: string, device: string): Promise<void> {
    client.on('message', (_topic, message) => {
      try {
        const body = JSON.parse(message.toString()) as LightLevelReading;
        Tail.log(body);
      } catch (err) {
        // Do nothing - only log normal values
      }
    });

    await client.subscribe(topic);
    console.log(`Streaming readings from ${device}`);
    console.log('---');
  }

  /**
   * Inject a log message into a device log stream
   * @param client MQTT client
   * @param topic Device log topic
   */
  private async inject(client: AsyncMqttClient, topic: string): Promise<void> {
    const message: LightLevelReading = {
      lux: 13000,
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

    try {
      const client = await MqttClient.connectToBroker(flags.address, flags.port, await this.getAccessKey());

      if (flags.inject) {
        this.inject(client, topic);
      } else {
        this.stream(client, topic, args.device);
      }
    } catch (err) {
      this.error(`Request failed: ${err}`);
    }
  }
}
