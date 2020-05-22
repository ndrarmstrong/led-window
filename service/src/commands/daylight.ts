import {Command, flags} from '@oclif/command'

export default class Daylight extends Command {
  static description = 'Configure daylight mode'

  static flags = {
    help: flags.help({char: 'h'}),
    // flag with a value (-n, --name=VALUE)
    name: flags.string({char: 'n', description: 'name to print'}),
    // flag with no value (-f, --force)
    force: flags.boolean({char: 'f'}),
  }

  static args = [{name: 'file'}]

  async run() {
    const {args, flags} = this.parse(Daylight)

    const name = flags.name ?? 'world'
    this.log(`hello ${name} from /home/node/service/src/commands/daylight.ts`)
    if (args.file && flags.force) {
      this.log(`you input --force and --file: ${args.file}`)
    }
  }
}
