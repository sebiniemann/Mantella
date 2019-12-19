const Mat = require('../../src/Mat');

describe('transformEachCol', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformEachCol((...args) => {
      calledWith.push(args);
      return args[0].transform(x => 2 * x);
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 4, 6,
      8, 10, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([2, 8], 2, 1), 0, mat],
      [new Mat([4, 10], 2, 1), 1, mat],
      [new Mat([6, 12], 2, 1), 2, mat],
    ]);
  });

  test('Array of indices', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformEachCol(
      [0, 1, 0],
      (...args) => {
        calledWith.push(args);
        return args[0].transform(x => 2 * x);
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      4, 4, 3,
      16, 10, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([2, 8], 2, 1), 0, mat],
      [new Mat([4, 10], 2, 1), 1, mat],
      [new Mat([4, 16], 2, 1), 0, mat],
    ]);
  });

  test('Array of column subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformEachCol(
      new Mat([true, false, true], 1, 3),
      (...args) => {
        calledWith.push(args);
        return args[0].transform(x => 2 * x);
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 2, 6,
      8, 5, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([2, 8], 2, 1), 0, mat],
      [new Mat([6, 12], 2, 1), 2, mat],
    ]);
  });
});
