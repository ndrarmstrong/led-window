import DeviceRequestCommand from '../../lib/deviceRequestCommand';
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
    },
  ];

  /**
   * Reply with a sample message
   */
  selfReply(): PublishRequest {
    return {
      intervalS: 5,
    };
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Interval);

    let res: PublishRequest;
    const body: PublishRequest = {};

    if (args.interval) {
      body.intervalS = parseInt(args.interval);
    }

    this.log(JSON.stringify(body));

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/interval`;
        res = await MqttClient.singleRequestResponse(
          flags.address,
          flags.port,
          reqTopic,
          JSON.stringify(body),
          await this.getAccessKey(),
          flags.reply ? this.selfReply : undefined
        );
      } else {
        this.error(`Unsupported request method: ${flags.method}`);
      }
    } catch (err) {
      this.error(`Request failed: ${err}`);
    }

    this.log('---');
    this.log(`Publish interval: ${res.intervalS}s`);
    this.log(JSON.stringify(res));
  }
}
