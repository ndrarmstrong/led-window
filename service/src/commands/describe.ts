import { Command, flags } from '@oclif/command';
import { DescribeResponse } from '../types/describe';
import MqttClient from '../lib/mqttClient';

export default class Describe extends Command {
  static description = 'Describe the state of a device';

  static flags = {
    help: flags.help({ char: 'h' }),
    port: flags.integer({ description: 'Server port', default: 1883 }),
    address: flags.string({ description: 'Server address', default: '127.0.0.1' }),
    method: flags.string({ description: 'Request method', options: ['http', 'mqtt'], default: 'mqtt' }),
  };

  static args = [{ name: 'device', description: 'Device ID', required: true }];

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Describe);

    let res: DescribeResponse;

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `device/${args.device}/describe`;
        res = await MqttClient.singleRequestResponse(flags.address, flags.port, reqTopic, '');
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
