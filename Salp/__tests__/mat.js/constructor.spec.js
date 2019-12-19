const Mat = require('../../src/Mat');

describe('constructor', () => {
  test('Shape', () => {
    const mat = new Mat(2, 3);

    expect(mat.nRows).toBe(2);
    expect(mat.nCols).toBe(3);
    expect(mat.nElems).toBe(6);
    expect(mat.raw.length).toBe(6);
  });

  test('Data with shape', () => {
    const data = [
      1, 2, 3,
      4, 5, 6,
    ];

    const mat = new Mat(data, 2, 3);

    // Changes to the original data are not affecting the matrix.
    data[0] += 1;

    expect(mat.nRows).toBe(2);
    expect(mat.nCols).toBe(3);
    expect(mat.nElems).toBe(6);
    expect(mat.raw).toEqual([
      1, 2, 3,
      4, 5, 6,
    ]);
  });

  test('Matrix', () => {
    const originalMat = new Mat([
      1, 2, 3,
      4, 5, 6,
    ], 2, 3);

    const copiedMat = new Mat(originalMat);

    // Changes to the original matrix are not affecting its copy.
    originalMat.raw[0] += 1;

    expect(copiedMat.nRows).toBe(2);
    expect(copiedMat.nCols).toBe(3);
    expect(copiedMat.nElems).toBe(6);
    expect(copiedMat.raw).toEqual([
      1, 2, 3,
      4, 5, 6,
    ]);
  });
});
