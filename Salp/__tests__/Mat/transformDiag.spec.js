'use strict';

const Mat = require('../../src/Mat');

describe('transformDiag', () => {
  test('Default', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformDiag((...args) => {
      calledWith.push(args);
      return 2 * args[0];
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 2, 3,
      4, 10, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [1, 0, mat],
      [5, 1, mat],
    ]);
  });

  test('Main diagonal', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformDiag(0, (...args) => {
      calledWith.push(args);
      return 2 * args[0];
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      2, 2, 3,
      4, 10, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [1, 0, mat],
      [5, 1, mat],
    ]);
  });

  test('Superdiagonal', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformDiag(1, (...args) => {
      calledWith.push(args);
      return 2 * args[0];
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 4, 3,
      4, 5, 12,
    ], 2, 3));
    expect(calledWith).toEqual([
      [2, 0, mat],
      [6, 1, mat],
    ]);
  });

  test('Subdiagonal', () => {
    const mat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const calledWith = [];
    const returnValue = mat.transformDiag(-1, (...args) => {
      calledWith.push(args);
      return 2 * args[0];
    });

    expect(mat).toEqual(returnValue);
    expect(mat).toEqual(new Mat([
      1, 2, 3,
      8, 5, 6,
    ], 2, 3));
    expect(calledWith).toEqual([
      [4, 0, mat],
    ]);
  });
});
