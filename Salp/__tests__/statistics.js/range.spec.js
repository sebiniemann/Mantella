const functions = require('../../src/functions');

describe('range', () => {
  test('To < from', () => {
    const range = functions.range(-2, 3);

    expect(range).toEqual([-2, -1, 0, 1, 2, 3]);
  });

  test('To > from', () => {
    const range = functions.range(2, -3);

    expect(range).toEqual([2, 1, 0, -1, -2, -3]);
  });

  test('To = from', () => {
    const range = functions.range(0, 0);

    expect(range).toEqual([0]);
  });

  test('Positive step size', () => {
    const range = functions.range(-2, 3, 2);

    expect(range).toEqual([-2, 0, 2]);
  });

  test('Negative step size', () => {
    const range = functions.range(2, -3, -2);

    expect(range).toEqual([2, 0, -2]);
  });
});
