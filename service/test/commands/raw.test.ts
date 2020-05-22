import { expect, test } from '@oclif/test';

describe('raw', () => {
  test
    .stdout()
    .command(['raw'])
    .it('runs hello', (ctx) => {
      expect(ctx.stdout).to.contain('hello world');
    });

  test
    .stdout()
    .command(['raw', '--name', 'jeff'])
    .it('runs hello --name jeff', (ctx) => {
      expect(ctx.stdout).to.contain('hello jeff');
    });
});
