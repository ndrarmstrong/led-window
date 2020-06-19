import DeviceRequestCommand from '../../lib/deviceRequestCommand';
import { AcknowledgeResponses, Acknowledgement } from '../../types/ack';
import MqttClient from '../../lib/mqttClient';
import { PublishRequest } from '../../types/publish';

/**
 * Change the sensor publish interval
 */
export default class Interval extends DeviceRequestCommand {
  static description = 'Change the sensor publish interval';
  static flags = { ...DeviceRequestCommand.flags };
  static args = [
    Interval.deviceArg,
    {
      name: 'interval',
      description: 'Publish interval, in seconds',
      required: true,
    },
  ];

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Interval);

    let res: Acknowledgement;
    const body: PublishRequest = {
      intervalS: parseInt(args.interval),
    };

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/interval`;
        res = await MqttClient.singleRequestResponse(
          flags.address,
          flags.port,
          reqTopic,
          JSON.stringify(body),
          await this.getAccessKey(),
          flags.reply ? this.selfAcknowledge : undefined
        );
      } else {
        this.error(`Unsupported request method: ${flags.method}`);
      }
    } catch (err) {
      this.error(`Request failed: ${err}`);
    }

    this.log('---');
    if (res.result === AcknowledgeResponses.Success) {
      this.log(`Interval changed to ${args.interval}s`);
    } else {
      this.error(`Failed to change interval: ${res.result}`);
    }
  }
}
