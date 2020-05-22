import {expect, test} from '@oclif/test';

describe('configure', () => {
  test
    .stdout()
    .command(['configure'])
    .it('stores credentials', ctx => {
      expect(ctx.stdout).to.contain('saved');
    });

  test
    .stdout()
    .command(['configure', '--clear'])
    .it('runs clear', ctx => {
      expect(ctx.stdout).to.contain('cleared');
    });
});
