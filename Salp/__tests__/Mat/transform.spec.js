'use strict';

const Mat = require('../../src/Mat');

describe('transform', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transform((...args) => {
      calledWith.push(args);
      return 2 * args[0];
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 4, 6,
      8, 10, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [1, 0, mat],
      [2, 1, mat],
      [3, 2, mat],
      [4, 3, mat],
      [5, 4, mat],
      [6, 5, mat],
    ]);
  });

  test('Array of indices', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transform(
      [0, 2, 0, 4],
      (...args) => {
        calledWith.push(args);
        return 2 * args[0];
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      4, 2, 6,
      4, 10, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [1, 0, mat],
      [3, 2, mat],
      [2, 0, mat],
      [5, 4, mat],
    ]);
  });

  test('Array of row and column subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transform(
      [0, 1, 0], [0, 2],
      (...args) => {
        calledWith.push(args);
        return 2 * args[0];
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      4, 2, 12,
      8, 5, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [1, 0, mat],
      [3, 2, mat],
      [4, 3, mat],
      [6, 5, mat],
      [2, 0, mat],
      [6, 2, mat],
    ]);
  });

  test('Matrix of logicals', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transform(
      new Mat([
        true, false, true,
        true, false, true,
      ], 2, 3),
      (...args) => {
        calledWith.push(args);
        return 2 * args[0];
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 2, 6,
      8, 5, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [1, 0, mat],
      [3, 2, mat],
      [4, 3, mat],
      [6, 5, mat],
    ]);
  });
});
