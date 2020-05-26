import { DescribeResponse } from '../types/describe';
import MqttClient from '../lib/mqttClient';
import DeviceCommand from '../lib/deviceCommand';

/**
 * Describe the state of a device
 */
export default class Describe extends DeviceCommand {
  static description = 'Describe the state of a device';
  static flags = { ...DeviceCommand.flags };
  static args = [Describe.deviceArg];

  /**
   * Reply with a sample message
   */
  selfReply(): DescribeResponse {
    return {
      device: 'ABC123',
      temperatureC: 22,
      relativeHumidityPct: 40,
    };
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Describe);

    let res: DescribeResponse;

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `device/${args.device}/describe`;
        res = await MqttClient.singleRequestResponse(
          flags.address,
          flags.port,
          reqTopic,
          '',
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
    this.log(`Device ID: ${res.device}`);
    this.log(`Free heap: ${res.freeHeapB} B`);
    this.log(`Sketch size: ${res.sketchSizeB} B`);
    this.log(`Free sketch space: ${res.freeSketchSpaceB} B`);
    this.log(`OTA Enabled: ${res.otaEnabled}`);
    if (res.temperatureC && res.relativeHumidityPct) {
      this.log(`Temperature: ${res.temperatureC}°C`);
      this.log(`Humidity: ${res.relativeHumidityPct}%`);
    }
  }
}
