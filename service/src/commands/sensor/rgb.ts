import { flags } from '@oclif/command';
import DeviceRequestCommand from '../../lib/deviceRequestCommand';
import MqttClient from '../../lib/mqttClient';
import { RgbConfig, RgbIntensityRange, RgbIRFilterOffset, RgbIRFilterLevel } from '../../types/rgb';

/**
 * Configures the RGB sensor
 */
export default class Rgb extends DeviceRequestCommand {
  static description = 'Configures the RGB sensor';
  static flags = {
    ...DeviceRequestCommand.flags,
    range: flags.string({ description: 'Sensor intensity range', options: ['375', '10k'] }),
    irOffset: flags.string({ description: 'Lower or upper range of IR filtering', options: ['off', 'on'] }),
    irLevel: flags.string({ description: 'Level of IR filtering', options: ['low', 'mid', 'high'] }),
  };
  static args = [Rgb.deviceArg];

  /**
   * Reply with a sample message
   */
  selfReply(): RgbConfig {
    return {
      intensityRange: RgbIntensityRange.Range10kLux,
      irFilterOffset: RgbIRFilterOffset.Off,
      irFilterLevel: RgbIRFilterLevel.High,
    };
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Rgb);

    let res: RgbConfig;
    const body: RgbConfig = {};

    if (flags.range === '375') {
      body.intensityRange = RgbIntensityRange.Range375Lux;
    } else if (flags.range === '10k') {
      body.intensityRange = RgbIntensityRange.Range10kLux;
    }

    if (flags.irOffset === 'off') {
      body.irFilterOffset = RgbIRFilterOffset.Off;
    } else if (flags.irOffset === 'on') {
      body.irFilterOffset = RgbIRFilterOffset.On;
    }

    if (flags.irLevel === 'low') {
      body.irFilterLevel = RgbIRFilterLevel.Low;
    } else if (flags.irLevel === 'mid') {
      body.irFilterLevel = RgbIRFilterLevel.Mid;
    } else if (flags.irLevel === 'high') {
      body.irFilterLevel = RgbIRFilterLevel.High;
    }

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/rgb`;
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

    if (res.intensityRange !== undefined) {
      this.log(`Intensity range: ${RgbIntensityRange[res.intensityRange]}`);
    }

    if (res.irFilterOffset !== undefined) {
      this.log(`IR filter offset:  ${RgbIRFilterOffset[res.irFilterOffset]}`);
    }

    if (res.irFilterLevel !== undefined) {
      this.log(`IR filter level: ${RgbIRFilterLevel[res.irFilterLevel]}`);
    }
  }
}
