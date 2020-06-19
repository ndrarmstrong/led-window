import { flags } from '@oclif/command';
import DeviceRequestCommand from '../../lib/deviceRequestCommand';
import { AcknowledgeResponses, Acknowledgement } from '../../types/ack';
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
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Rgb);

    if (flags.range === undefined && flags.irLevel === undefined && flags.irOffset === undefined) {
      this.error(`Must set at least one of range, irOffset, or irLevel to configure RGB sensor`);
    }

    let res: Acknowledgement;
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
          flags.reply ? this.selfAcknowledge : undefined
        );
      } else {
        this.error(`Unsupported request method: ${flags.method}`);
      }
    } catch (err) {
      this.error(`Request failed: ${err}`);
    }

    this.log('---');

    let range = '';
    let irOffset = '';
    let irLevel = '';

    if (body.intensityRange !== undefined) {
      range = `range ${RgbIntensityRange[body.intensityRange]} `;
    }

    if (body.irFilterOffset !== undefined) {
      irOffset = `filter offset ${RgbIRFilterOffset[body.irFilterOffset]} `;
    }

    if (body.irFilterLevel !== undefined) {
      irLevel = `filter level ${RgbIRFilterLevel[body.irFilterLevel]} `;
    }

    if (res.result === AcknowledgeResponses.Success) {
      this.log(`Configured RGB sensor: ${range}${irOffset}${irLevel}`);
    } else {
      this.error(`Failed to change mode: ${range}${irOffset}${irLevel}`);
    }
  }
}
