import DeviceCommand from '../../lib/deviceCommand';
import { Acknowledgement, AcknowledgeResponses } from '../../types/ack';
import MqttClient from '../../lib/mqttClient';
import RawRequest from '../../types/raw';

/**
 * Configure raw mode command
 */
export default class Raw extends DeviceCommand {
  static description = 'Configure raw mode';
  static flags = { ...DeviceCommand.flags };
  static args = [
    Raw.deviceArg,
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
    const { args, flags } = this.parse(Raw);

    let res: Acknowledgement;
    let body: RawRequest;
    try {
      body = RawRequest.fromValues(parseInt(args.red), parseInt(args.green), parseInt(args.blue), parseInt(args.white));
    } catch (err) {
      this.error(`Unable to convert (${args.red},${args.green},${args.blue},${args.white}) to color: ${err}`);
    }

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/raw`;
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
      this.log(`Raw mode configured to (${args.red},${args.green},${args.blue},${args.white})`);
    } else {
      this.error(`Failed to configure raw mode: ${res.result}`);
    }
  }
}
