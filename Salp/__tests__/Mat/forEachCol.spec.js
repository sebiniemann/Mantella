'use strict';

const Mat = require('../../src/Mat');

describe('forEachCol', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.forEachCol((...args) => {
      calledWith.push(args);
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([1, 4], 2, 1), 0, mat],
      [new Mat([2, 5], 2, 1), 1, mat],
      [new Mat([3, 6], 2, 1), 2, mat],
    ]);
  });

  test('Array of column subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.forEachCol(
      [0, 1, 0],
      (...args) => {
        calledWith.push(args);
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([1, 4], 2, 1), 0, mat],
      [new Mat([2, 5], 2, 1), 1, mat],
      [new Mat([1, 4], 2, 1), 0, mat],
    ]);
  });

  test('Array of logicals', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.forEachCol(
      new Mat([true, false, true], 1, 3),
      (...args) => {
        calledWith.push(args);
      },
    );

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([1, 4], 2, 1), 0, mat],
      [new Mat([3, 6], 2, 1), 2, mat],
    ]);
  });
});
