'use strict';

const Mat = require('../../src/Mat');

describe('reduce', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const value = mat.reduce((...args) => {
      calledWith.push(args);
      return args[0] + args[1];
    }, 0);

    expect(value).toBe(21);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [0, 1, 0, mat],
      [1, 2, 1, mat],
      [3, 3, 2, mat],
      [6, 4, 3, mat],
      [10, 5, 4, mat],
      [15, 6, 5, mat],
    ]);
  });

  test('Array of indices', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const value = mat.reduce(
      [0, 2, 0, 4],
      (...args) => {
        calledWith.push(args);
        return args[0] + args[1];
      }, 0
    );

    expect(value).toBe(10);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [0, 1, 0, mat],
      [1, 3, 2, mat],
      [4, 1, 0, mat],
      [5, 5, 4, mat],
    ]);
  });

  test('Array of row and column subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const value = mat.reduce(
      [0, 1, 0], [0, 2],
      (...args) => {
        calledWith.push(args);
        return args[0] + args[1];
      }, 0
    );

    expect(value).toBe(18);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [0, 1, 0, mat],
      [1, 3, 2, mat],
      [4, 4, 3, mat],
      [8, 6, 5, mat],
      [14, 1, 0, mat],
      [15, 3, 2, mat],
    ]);
  });

  test('Matrix of logicals', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const value = mat.reduce(
      new Mat([
        true, false, true,
        true, false, true,
      ], 2, 3),
      (...args) => {
        calledWith.push(args);
        return args[0] + args[1];
      }, 0
    );

    expect(value).toBe(14);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [0, 1, 0, mat],
      [1, 3, 2, mat],
      [4, 4, 3, mat],
      [8, 6, 5, mat],
    ]);
  });
});
