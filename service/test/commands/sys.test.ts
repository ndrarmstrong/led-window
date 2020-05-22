import { expect, test } from '@oclif/test';

describe('sys', () => {
  test
    .stdout()
    .command(['sys'])
    .it('runs hello', (ctx) => {
      expect(ctx.stdout).to.contain('hello world');
    });

  test
    .stdout()
    .command(['sys', '--name', 'jeff'])
    .it('runs hello --name jeff', (ctx) => {
      expect(ctx.stdout).to.contain('hello jeff');
    });
});
