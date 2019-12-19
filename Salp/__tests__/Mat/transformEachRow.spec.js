'use strict';

const Mat = require('../../src/Mat');

describe('transformEachRow', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformEachRow((...args) => {
      calledWith.push(args);
      return args[0].transform(x => 2 * x);
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 4, 6,
      8, 10, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([2, 4, 6], 1, 3), 0, mat],
      [new Mat([8, 10, 12], 1, 3), 1, mat],
    ]);
  });

  test('Array of indices', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformEachRow(
      [0, 1, 0],
      (...args) => {
        calledWith.push(args);
        return args[0].transform(x => 2 * x);
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      4, 8, 12,
      8, 10, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([2, 4, 6], 1, 3), 0, mat],
      [new Mat([8, 10, 12], 1, 3), 1, mat],
      [new Mat([4, 8, 12], 1, 3), 0, mat],
    ]);
  });

  test('Array of row subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformEachRow(
      new Mat([false, true], 1, 2),
      (...args) => {
        calledWith.push(args);
        return args[0].transform(x => 2 * x);
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      8, 10, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([8, 10, 12], 1, 3), 1, mat],
    ]);
  });
});
