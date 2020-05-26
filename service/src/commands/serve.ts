import { Command, flags } from '@oclif/command';
import Broker from '../lib/broker';
import ServiceConfig from '../lib/serviceConfig';

/**
 * Start the MQTT broker and HTTP server
 */
export default class Serve extends Command {
  static description = 'Start the HTTP server and MQTT broker; runs until stopped';

  static flags = {
    help: flags.help({ char: 'h' }),
    port: flags.integer({ description: 'Port to listen on', default: 1883 }),
    host: flags.string({ description: 'Host address', default: '0.0.0.0' }),
    devKey: flags.string({ description: 'Device key, for devices to connect to MQTT', multiple: true }),
    apiKey: flags.string({ description: 'API key, for MQTT/HTTP API access', multiple: true }),
  };

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { flags } = this.parse(Serve);

    if (flags.devKey) {
      ServiceConfig.deviceKeys = flags.devKey;
    }

    if (flags.apiKey) {
      ServiceConfig.apiKeys = flags.apiKey;
    }

    const broker = new Broker(flags.port, flags.host);
    broker.listen();
  }
}
