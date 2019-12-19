<template>
  <div>
    <h1>API</h1>

    <section>
      <h2>Constructor</h2>

      <section>
        <h3>
          Mat(nRows, nCols)<br>
          Mat(data, nRows, nCols)<br>
          Mat(mat)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Defining the shape</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat(2, 3);
          </c-example>
        </section>

        <section>
          <h4>Providing data</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Copying a matrix</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat(2, 3);
const copy = new Mat(mat);
          </c-example>
        </section>
      </section>
    </section>

    <section>
      <h2>Attributes</h2>

      <section>
        <h3>
          .nRows
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

console.log(mat.nRows);
// 2
        </c-example>
      </section>

      <section>
        <h3>
          .nCols
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

console.log(mat.nCols);
// 3
        </c-example>
      </section>

      <section>
        <h3>
          .nElems
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

console.log(mat.nElems);
// 6
        </c-example>
      </section>

      <section>
        <h3>
          .raw
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

console.log(mat.raw);
// [ 1, 2, 3, 4, 5, 6 ]
        </c-example>
      </section>
    </section>

    <section>
      <h2>Methods</h2>

      <section>
        <h3>
          .at(index)<br>
          .at(rowSubscript, colSubscript)<br>
          .at(indices)<br>
          .at(rowSubscripts, colSubscripts)<br>
          .at(logicals)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Element access via an index</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

console.log(mat.at(1));
// 2
console.log(mat.at(2));
// 3
console.log(mat.at(3));
// 4
          </c-example>
        </section>

        <section>
          <h4>Element access via a row and column subscript</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
console.log(mat.at(0, 1));
// 2
console.log(mat.at(1, 1));
// 5
console.log(mat.at(1, 2));
// 6
          </c-example>
        </section>

        <section>
          <h4>Subsetting via indicies</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
console.log(mat.at([1]));
// Mat {
//   nRows: 1,
//   nCols: 1,
//   nElems: 1,
//   raw: [ 2 ]
// }

console.log(mat.at([0, 4, 0]));
// Mat {
//   nRows: 1,
//   nCols: 3,
//   nElems: 3,
//   raw: [ 1, 5, 1 ]
// }
          </c-example>
        </section>

        <section>
          <h4>Subsetting via row and column subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
console.log(mat.at([0], [1]));
// Mat {
//   nRows: 1,
//   nCols: 1,
//   nElems: 1,
//   raw: [ 2 ]
// }

console.log(mat.at([0, 1, 0], [2, 1]));
// Mat {
//   nRows: 3,
//   nCols: 2,
//   nElems: 6,
//   raw: [
//     3, 2,
//     6, 5,
//     3, 2,
//   ]
// }
          </c-example>
        </section>

        <section>
          <h4>Subsetting via logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
console.log(mat.at(new Mat([
  true, false, true,
  true, false, true,
], 2, 3)));
// Mat {
//   nRows: 1,
//   nCols: 4,
//   nElems: 4,
//   raw: [ 0, 2, 3, 5 ]
// }
          </c-example>
        </section>
      </section>

      <section>
        <h3>
          .diag()
          .diag(k)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Main diagonal</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

console.log(mat.diag());
// Mat {
//   nRows: 1,
//   nCols: 2,
//   nElems: 2,
//   raw: [ 1, 5 ]
// }
          </c-example>
        </section>

        <section>
          <h4>Superdiagonal</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
console.log(mat.diag(1));
// Mat {
//   nRows: 1,
//   nCols: 2,
//   nElems: 2,
//   raw: [ 2, 6 ]
// }
          </c-example>
        </section>

        <section>
          <h4>Subdiagonal</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
console.log(mat.diag(-1));
// Mat {
//   nRows: 1,
//   nCols: 1,
//   nElems: 1,
//   raw: [ 4 ]
// }
          </c-example>
        </section>
      </section>

      <section>
        <h3>
          .forEach(callback)<br>
          .forEach(indices, callback)<br>
          .forEach(rowSubscripts, colSubscripts, callback)<br>
          .forEach(logicals, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Looping over all elements</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);

mat.forEach((...args) => {
  console.log(args);
});
// [ 1, 0, mat ]
// [ 2, 1, mat ]
// [ 3, 2, mat ]
// [ 4, 3, mat ]
// [ 5, 4, mat ]
// [ 6, 5, mat ]
          </c-example>
        </section>

        <section>
          <h4>Having matching indicies</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
mat.forEach(
  [1, 5, 1]
  (...args) => {
    console.log(args);
  },
);
// [ 2, 1, mat ]
// [ 6, 5, mat ]
// [ 2, 1, mat ]
          </c-example>
        </section>

        <section>
          <h4>Having matching row and column subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
mat.forEach(
  [0, 1, 0], [2, 1]
  (...args) => {
    console.log(args);
  },
);
// [ 3, 2, mat ]
// [ 2, 1, mat ]
// [ 6, 5, mat ]
// [ 5, 4, mat ]
// [ 3, 2, mat ]
// [ 2, 1, mat ]
          </c-example>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
mat.forEach(
  new Mat([
    true, false, true,
    true, false, true,
  ], 2, 3),
  (...args) => {
    console.log(args);
  },
);
// [ 1, 0, mat ]
// [ 3, 2, mat ]
// [ 4, 3, mat ]
// [ 6, 5, mat ]
          </c-example>
        </section>
      </section>

      <section>
        <h3>
          .forEachRow(callback)<br>
          .forEachRow(rowSubscripts, callback)<br>
          .forEachRow(logicals, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Looping over all rows</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Having matching row subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>

      <section>
        <h3>
          .forEachCol(callback)<br>
          .forEachCol(colSubscripts, callback)<br>
          .forEachCol(logicals, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Looping over all columns</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Having matching column subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>

      <section>
        <h3>
          .transform(callback)<br>
          .transform(indices, callback)<br>
          .transform(rowSubscripts, colSubscripts, callback)<br>
          .transform(logicals, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Transforming all elements</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Having matching indices</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having row and columns subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>

      <section>
        <h3>
          .transformRow(callback)<br>
          .transformRow(rowSubscripts, callback)<br>
          .transformRow(logicals, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Transforming all rows</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Having row subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>

      <section>
        <h3>
          .transformCol(callback)<br>
          .transformCol(colSubscripts, callback)<br>
          .transformCol(logicals, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Transforming all columns</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Having column subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>

      <section>
        <h3>
          .transformDiag(callback)<br>
          .transformDiag(k, callback)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Transforming all main diagonal elements</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Superdiagonal elements</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Subdiagonal elements</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>

      <section>
        <h3>
          .reduce(callback, initialValue)<br>
          .reduce(indices, callback, initialValue)<br>
          .reduce(rowSubscripts, columnSubscripts, callback, initialValue)<br>
          .reduce(logicals, callback, initialValue)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <section>
          <h4>Reducing all values</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>

          <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
          </c-example>
        </section>

        <section>
          <h4>Having matching indices</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having row and columns subscripts</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>

        <section>
          <h4>Having truthy logicals</h4>

          <p>
            Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
          </p>
        </section>
      </section>
    </section>

    <section>
      <h2>Functions</h2>

      <section>
        <h3>
          range(from, to, stepSize?)
        </h3>

        <p>
          Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
        </p>

        <c-example>
const mat = new Mat([
  1, 2, 3,
  4, 5, 6,
], 2, 3);
        </c-example>
      </section>
    </section>
  </div>
</template>

<script>
import * as salp from '../../src';

export default {

}
</script>
