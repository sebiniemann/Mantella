'use strict';

const Mat = require('../../src/Mat');

describe('forEachRow', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.forEachRow((...args) => {
      calledWith.push(args);
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [new Mat([1, 2, 3], 1, 3), 0, mat],
      [new Mat([4, 5, 6], 1, 3), 1, mat],
    ]);
  });

  test('Array of row subscripts', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.forEachRow(
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
      [new Mat([1, 2, 3], 1, 3), 0, mat],
      [new Mat([4, 5, 6], 1, 3), 1, mat],
      [new Mat([1, 2, 3], 1, 3), 0, mat],
    ]);
  });

  test('Array of logicals', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.forEachRow(
      new Mat([false, true], 1, 2),
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
      [new Mat([4, 5, 6], 1, 3), 1, mat],
    ]);
  });
});
