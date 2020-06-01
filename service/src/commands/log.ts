import DeviceCommand from '../lib/deviceCommand';
import { LogPayload } from '../types/log';
import MqttClient from '../lib/mqttClient';
import { flags } from '@oclif/command';
import { AsyncMqttClient } from 'async-mqtt';

export default class Log extends DeviceCommand {
  static description = 'Tail logs of a device';
  static flags = {
    ...DeviceCommand.flags,
    inject: flags.boolean({ description: 'Inject a message into the log stream', hidden: true }),
  };
  static args = [Log.deviceArg];

  /**
   * Log with timestamp and tag
   */
  private static log(message: string): void {
    console.log(`[${new Date().toISOString()}] ${message}`);
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
        const body = JSON.parse(message.toString()) as LogPayload;
        Log.log(body.line);
      } catch (err) {
        Log.log(`Unparsable message: '${message}'`);
      }
    });

    await client.subscribe(topic);
    console.log(`Streaming logs from ${device}`);
    console.log('---');
  }

  /**
   * Inject a log message into a device log stream
   * @param client MQTT client
   * @param topic Device log topic
   */
  private async inject(client: AsyncMqttClient, topic: string): Promise<void> {
    const message: LogPayload = {
      line: 'Injected log message',
    };
    await client.publish(topic, JSON.stringify(message));
    await client.end();
    console.log(`Log injected to ${topic}`);
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Log);

    const topic = `${MqttClient.topicNamespace}/device/${args.device}/log`;

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
