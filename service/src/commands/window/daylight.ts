import DeviceCommand from '../../lib/deviceCommand';
import DaylightRequest from '../../types/daylight';
import { Acknowledgement, AcknowledgeResponses } from '../../types/ack';
import MqttClient from '../../lib/mqttClient';

/**
 * Configure daylight mode command
 */
export default class Daylight extends DeviceCommand {
  static description = 'Configure daylight mode';
  static flags = { ...DeviceCommand.flags };
  static args = [
    Daylight.deviceArg,
    {
      name: 'red',
      description: 'Red level',
      required: true,
    },
    {
      name: 'green',
      description: 'Green level',
      required: true,
    },
    {
      name: 'blue',
      description: 'Blue level',
      required: true,
    },
    {
      name: 'white',
      description: 'Daylight white level',
      required: true,
    },
  ];

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Daylight);

    let res: Acknowledgement;
    let body: DaylightRequest;
    try {
      body = DaylightRequest.fromValues(
        parseInt(args.red),
        parseInt(args.green),
        parseInt(args.blue),
        parseInt(args.white)
      );
    } catch (err) {
      this.error(`Unable to convert (${args.red},${args.green},${args.blue},${args.white}) to color: ${err}`);
    }

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/daylight`;
        res = await MqttClient.singleRequestResponse(
          flags.address,
          flags.port,
          reqTopic,
          JSON.stringify(body),
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
      this.log(`Daylight mode configured to (${args.red},${args.green},${args.blue},${args.white})`);
    } else {
      this.error(`Failed to configure daylight mode: ${res.result}`);
    }
  }
}
