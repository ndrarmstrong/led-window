import { Command, flags } from '@oclif/command';

export default class Describe extends Command {
  static description = 'Describe the state of a resource';

  static flags = {
    help: flags.help({ char: 'h' }),
    // flag with a value (-n, --name=VALUE)
    name: flags.string({ char: 'n', description: 'name to print' }),
    // flag with no value (-f, --force)
    force: flags.boolean({ char: 'f' }),
  };

  static args = [{ name: 'file' }];

  async run(): Promise<void> {
    const { args, flags } = this.parse(Describe);

    const name = flags.name ?? 'world';
    this.log(`hello ${name} from /home/node/service/src/commands/describe.ts`);
    if (args.file && flags.force) {
      this.log(`you input --force and --file: ${args.file}`);
    }
  }
}
