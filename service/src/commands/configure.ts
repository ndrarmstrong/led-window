import { Command, flags } from '@oclif/command';
import cli from 'cli-ux';
import ClientConfig from '../lib/clientConfig';

/**
 * Configure command.
 */
export default class Configure extends Command {
  static description = 'Configure CLI credentials';

  static flags = {
    help: flags.help({ char: 'h' }),
    clear: flags.boolean({ description: 'Clear saved credentials' }),
  };

  /**
   * Run command
   */
  async run(): Promise<void> {
    const { flags } = this.parse(Configure);

    // Clear stored credentials
    if (flags.clear) {
      await this.clearCreds();
      return;
    }

    await this.storeCreds();
  }
  /**
   * Clear saved credentials
   */
  async clearCreds(): Promise<void> {
    if (await ClientConfig.credsExist()) {
      try {
        ClientConfig.clearCreds();
        this.log('Saved credentials cleared');
      } catch (err) {
        this.log(`Failed to clear stored credentials: ${err}`);
      }
    } else {
      this.log('No saved credentials');
    }
  }

  /**
   * Save new credentails
   */
  async storeCreds(): Promise<void> {
    const accessKey = await cli.prompt('User access key', { type: 'mask' });

    try {
      ClientConfig.storeCreds(accessKey);
      this.log(`Access key saved to profile`);
    } catch (err) {
      this.error(`Error saving credentials: ${err}`);
    }
  }
}
