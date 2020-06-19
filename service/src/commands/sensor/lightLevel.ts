import MqttClient from '../../lib/mqttClient';
import DeviceRequestCommand from '../../lib/deviceRequestCommand';
import { LightLevelReading } from '../../types/lightLevel';

/**
 * Read the current light levels from the sensor
 */
export default class LightLevel extends DeviceRequestCommand {
  static description = 'Read the current light levels';
  static flags = { ...DeviceRequestCommand.flags };
  static args = [LightLevel.deviceArg];

  /**
   * Reply with a sample message
   */
  selfReply(): LightLevelReading {
    return {
      lux: 13000,
      red: 0xfff0,
      green: 0xff,
      blue: 0xff00,
    };
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(LightLevel);

    let res: LightLevelReading;

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `device/${args.device}/lightLevel`;
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
    this.log(`Intensity: ${res.lux} lux`);
    this.log(`Red: ${res.red.toString(16)} (${((res.red * 100) / 65535).toFixed(1)}%)`);
    this.log(`Green: ${res.green.toString(16)} (${((res.green * 100) / 65535).toFixed(1)}%)`);
    this.log(`Blue: ${res.blue.toString(16)} (${((res.blue * 100) / 65535).toFixed(1)}%)`);
  }
}
