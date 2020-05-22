import { Command, flags } from '@oclif/command';
import cli from 'cli-ux';
import { promises as fs } from 'fs';
import { homedir } from 'os';

/**
 * Directory to store credentials
 */
const credsDir = `${homedir()}/.ledwincmd`;

/**
 * Filename for saved credentials
 */
const credsFileName = 'credentials';

/**
 * Full path to credentials
 */
const credsPath = `${credsDir}/${credsFileName}`;

/**
 * Configure command.
 */
export default class Configure extends Command {
  static description = 'Manage personal credentials';

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
   * Check if credentials exist
   */
  async credsExist(): Promise<boolean> {
    try {
      await fs.access(credsPath);
      return true;
    } catch {
      return false;
    }
  }

  /**
   * Clear saved credentials
   */
  async clearCreds(): Promise<void> {
    if (await this.credsExist()) {
      try {
        await fs.unlink(credsPath);
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
    const data = {
      accessKey: accessKey,
    };

    if (!(await this.credsExist())) {
      try {
        await fs.mkdir(credsDir, {
          recursive: true,
        });
      } catch (err) {
        this.error(`Error saving credentials: ${err}`);
        return;
      }
    }

    try {
      await fs.writeFile(credsPath, JSON.stringify(data, undefined, 2));
      this.log(`Access key saved to ${credsPath}`);
    } catch (err) {
      this.error(`Error saving credentials: ${err}`);
    }
  }

  /**
   * Read access key from credentials
   */
  async readAccessKey(): Promise<string> {
    let credsData: string;
    let accessKey: string;

    try {
      credsData = await fs.readFile(credsPath, 'utf8');
      accessKey = JSON.parse(credsData).accessKey;
    } catch {
      throw new Error('Unable to read credentials file');
    }

    return accessKey;
  }
}
