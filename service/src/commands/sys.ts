import DeviceRequestCommand from '../lib/deviceRequestCommand';
import { Acknowledgement, AcknowledgeResponses } from '../types/ack';
import MqttClient from '../lib/mqttClient';
import { SysRequest } from '../types/sys';

/**
 * Configure system functions of a device
 */
export default class Sys extends DeviceRequestCommand {
  static description = 'Configure system functions of a device';
  static flags = { ...DeviceRequestCommand.flags };
  static args = [Sys.deviceArg, { name: 'action', description: 'System action', required: true, options: ['ota'] }];

  /** Run command */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Sys);

    let res: Acknowledgement;
    const body: SysRequest = {
      otaEnabled: true,
    };

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/sys`;
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
      this.log('OTA Enabled');
    } else {
      this.error(`Failed to enable OTA: ${res.result}`);
    }
  }
}
