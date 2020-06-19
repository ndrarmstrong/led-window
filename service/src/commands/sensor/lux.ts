import { flags } from '@oclif/command';
import DeviceRequestCommand from '../../lib/deviceRequestCommand';
import MqttClient from '../../lib/mqttClient';
import { LuxConfig, LuxIntegrationMs } from '../../types/lux';

/**
 * Configures the lux sensor
 */
export default class Lux extends DeviceRequestCommand {
  static description = 'Configures the lux sensor';
  static flags = {
    ...DeviceRequestCommand.flags,
    gain: flags.string({ description: 'Sensor gain', options: ['0.125', '0.25', '0.5', '1', '2'] }),
    integrationTime: flags.string({
      description: 'Integration time (ms)',
      options: ['25', '50', '100', '200', '400', '800'],
    }),
  };
  static args = [Lux.deviceArg];

  /**
   * Reply with a sample message
   */
  selfReply(): LuxConfig {
    return {
      gain: 0.25,
      integrationTimeMs: 100,
    };
  }

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { args, flags } = this.parse(Lux);

    let res: LuxConfig;
    const body: LuxConfig = {};

    if (flags.gain !== undefined) {
      body.gain === parseFloat(flags.gain);
    }

    if (flags.integrationTime !== undefined) {
      body.integrationTimeMs = parseInt(flags.integrationTime) as LuxIntegrationMs;
    }

    try {
      if (flags.method == 'mqtt') {
        const reqTopic = `${this.deviceTopic(args.device)}/lux`;
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
    this.log(`Gain: ${res.gain}`);
    this.log(`Integration time: ${res.integrationTimeMs} ms`);
  }
}
