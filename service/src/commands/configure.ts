import {Command, flags} from '@oclif/command';

export default class Configure extends Command {
  static description = 'Manage personal credentials';

  static flags = {
    help: flags.help({char: 'h'}),
    clear: flags.boolean({description:"Clear saved credentials"})
  };

  async run() : Promise<void> {
    const {flags} = this.parse(Configure);

    if (flags.clear) {
      this.log("Saved credentials cleared");
      return;
    }

    this.log("Credentials saved to ~/.ledwincmd/credentials");
  }
}
