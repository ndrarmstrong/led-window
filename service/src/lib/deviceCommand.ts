import { Command, flags } from '@oclif/command';
import ClientConfig from './clientConfig';

/**
 * Base type for device commands.
 */
export default abstract class DeviceCommand extends Command {
  static flags = {
    help: flags.help({ char: 'h' }),
    port: flags.integer({ description: 'Server port', default: 1883 }),
    address: flags.string({ description: 'Server address', default: '127.0.0.1' }),
  };

  static deviceArg = { name: 'device', description: 'Device ID', required: true };

  /**
   * Get the device topic
   * @param id Device ID
   */
  protected deviceTopic(id: string): string {
    return `device/${id}`;
  }

  /** Get the configured access key */
  protected async getAccessKey(): Promise<string> {
    try {
      return await ClientConfig.readCreds();
    } catch {
      this.error('Unable to read access key; run `ledwinctrl configure` to set');
    }
  }
}
