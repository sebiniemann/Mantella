const Mat = require('../../src/Mat');

describe('elem', () => {
  test('Single index', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const value = mat.at(2);

    expect(value).toBe(3);
  });

  test('Single row and column subscript', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const value = mat.at(0, 1);

    expect(value).toBe(2);
  });

  test('Array of indices', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const submat = mat.at([0, 2, 0, 4]);

    expect(submat).toEqual(new Mat([
      1, 3, 1, 5,
    ], 1, 4));
  });

  test('Array of row and column subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const submat = mat.at([0, 1, 0], [0, 2]);

    expect(submat).toEqual(new Mat([
      1, 3,
      4, 6,
      1, 3,
    ], 3, 2));
  });

  test('Matrix of logicals', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const submat = mat.at(new Mat([
      true, false, true,
      true, false, true,
    ], 2, 3));

    expect(submat).toEqual(new Mat([
      1, 3, 4, 6,
    ], 1, 4));
  });

});
