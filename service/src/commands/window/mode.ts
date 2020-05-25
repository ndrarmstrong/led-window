import DeviceCommand from '../../lib/deviceCommand';
import { AcknowledgeResponses, Acknowledgement } from '../../types/ack';
import { WindowModePayload, WindowModes } from '../../types/mode';
import MqttClient from '../../lib/mqttClient';

/**
 * Activates a specific window mode
 */
export default class Mode extends DeviceCommand {
  static description = 'Activate a specific mode';
  static flags = { ...DeviceCommand.flags };
  static args = [
    Mode.deviceArg,
    {
      name: 'mode',
      description: 'Window mode',
      required: true,
      options: ['system', 'self-test', 'raw', 'daylight', 'stained-glass'],
    },
  ];

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Mode);

    let res: Acknowledgement;
    const body: WindowModePayload = {
      mode: WindowModes.System,
    };

    switch (args.mode) {
      case 'system':
        body.mode = WindowModes.System;
        break;
      case 'self-test':
        body.mode = WindowModes.SelfTest;
        break;
      case 'raw':
        body.mode = WindowModes.Raw;
        break;
      case 'daylight':
        body.mode = WindowModes.Daylight;
        break;
      case 'stained-glass':
        body.mode = WindowModes.StainedGlass;
        break;
    }

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/mode`;
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
      this.log(`Mode changed to ${args.mode}`);
    } else {
      this.error(`Failed to change mode: ${res.result}`);
    }
  }
}
