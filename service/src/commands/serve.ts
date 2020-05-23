import { Command, flags } from '@oclif/command';
import Broker from '../broker';

/**
 * Start the MQTT broker and HTTP server
 */
export default class Serve extends Command {
  static description = 'Start the HTTP server and MQTT broker; runs until stopped';

  static flags = {
    help: flags.help({ char: 'h' }),
    port: flags.integer({ description: 'Port to listen on', default: 1883 }),
    host: flags.string({ description: 'Host address', default: '0.0.0.0' }),
  };

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { flags } = this.parse(Serve);

    const broker = new Broker(flags.port, flags.host);
    broker.listen();
  }
}
