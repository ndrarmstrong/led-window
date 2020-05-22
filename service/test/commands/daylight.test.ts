import {expect, test} from '@oclif/test';

describe('daylight', () => {
  test
    .stdout()
    .command(['daylight'])
    .it('runs hello', ctx => {
      expect(ctx.stdout).to.contain('hello world');
    });

  test
    .stdout()
    .command(['daylight', '--name', 'jeff'])
    .it('runs hello --name jeff', ctx => {
      expect(ctx.stdout).to.contain('hello jeff');
    });
});
