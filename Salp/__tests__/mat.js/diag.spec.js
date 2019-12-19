const Mat = require('../../src/Mat');

describe('at', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const diag = mat.diag();

    expect(diag).toEqual(new Mat([
      1, 5,
    ], 1, 2));
  });

  test('Main diagonal', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const diag = mat.diag(0);

    expect(diag).toEqual(new Mat([
      1, 5,
    ], 1, 2));
  });

  test('Superdiagonal', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const diag = mat.diag(1);

    expect(diag).toEqual(new Mat([
      2, 6,
    ], 1, 2));
  });

  test('Subdiagonal', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const diag = mat.diag(-1);

    expect(diag).toEqual(new Mat([
      4,
    ], 1, 1));
  });
});
