import { promises as fs } from 'fs';
import { homedir } from 'os';

export default class ClientConfig {
  /**
   * Directory to store credentials
   */
  private static credsDir = `${homedir()}/.ledwincmd`;

  /**
   * Filename for saved credentials
   */
  private static credsFileName = 'credentials';

  /**
   * Full path to credentials
   */
  static credsPath = `${ClientConfig.credsDir}/${ClientConfig.credsFileName}`;

  /**
   * Check if credentials exist
   */
  static async credsExist(): Promise<boolean> {
    try {
      await fs.access(ClientConfig.credsPath);
      return true;
    } catch {
      return false;
    }
  }

  /**
   * Clear saved credentials
   */
  static async clearCreds(): Promise<boolean> {
    if (await ClientConfig.credsExist()) {
      await fs.unlink(ClientConfig.credsPath);
      return true;
    }
    return false;
  }

  /**
   * Save new credentials
   */
  static async storeCreds(accessKey: string): Promise<void> {
    const data = {
      accessKey: accessKey,
    };

    if (!(await ClientConfig.credsExist())) {
      await fs.mkdir(ClientConfig.credsDir, {
        recursive: true,
      });
    }

    await fs.writeFile(ClientConfig.credsPath, JSON.stringify(data, undefined, 2));
  }

  /**
   * Read access key from credentials
   */
  static async readCreds(): Promise<string> {
    const credsData = await fs.readFile(ClientConfig.credsPath, 'utf8');
    const accessKey = JSON.parse(credsData).accessKey;
    return accessKey;
  }
}
