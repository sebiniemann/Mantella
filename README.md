# Salp

## API

### Fields

- `.nElems`

  The number of elements.
  
  Same as `.nRows * .nCols`.

- `.nRows`

  The number of rows.

- `.nCols`

  The number of columns.

- `.raw`

  The 1D `Array`

- `._rng`

- `._add(a, b)`

- `._subtract(a, b)`

- `._multiply(a, b)`

- `._divide(a, b)`

### Creation

- `new Mat()`
- `empty(nRows, nCols)`

    Shorthand for `new Mat(nRows, nCols)`.

- `zeros(nRows, nCols)`

    Shorthand for `new Mat(nRows, nCols, () => 0)`.

- `ones(nRows, nCols)`

    Shorthand for `new Mat(nRows, nCols, () => 1)`.

- `identity(nRows, nCols)`

    Shorthand for `zeros(nRows, nCols).diag(() => 0)`.

- `normal(nRows, nCols)`
- `uniform(nRows, nCols)`
- `gamma(nRows, nCols)`
- `arange()`
- `linspace()`
- `logspace()`
- `geomspace()`
- `meshGrid()`
- `repeat()`

### Inplace-Manipulation and Access

- `.elem()`
- `.elemDiag()`
- `.col()`
- `.row()`
- `.roll()`
- `.rollCol()`
- `.rollRow()`
- `.fill()`
- `.replace()`
- `.shuffle(mat)`
- `.clamp(mat, lowerBounds, upperBounds)`

### Layout

- `.resize(nRows, nCols)`
- `.flat()`
- `.appendCol()`
- `.appendRow()`
- `.prependCol()`
- `.prependRow()`
- `.insertCol()`
- `.insertRow()`
- `.removeCol()`
- `.removeRow()`
- `.lowerTriangular()`
- `.upperTriangular()`

### Functional

- `.map(callback)`
- `.mapCol(callback)`
- `.mapRow(callback)`
- `.reduce(callback, initialValue)`
- `.reduceCol(callback, initialValue)`
- `.reduceRow(callback, initialValue)`
- `.filter(callback)`
- `.filterCol()`
- `.filterRow()`
- `.some()`
- `.someCol()`
- `.someRow()`
- `.every()`
- `.everyCol()`
- `.everyRow()`
- `.reverse()`
- `.reverseCol()`
- `.reverseRow()`
- `.sort()`
- `.sortCol()`
- `.sortRow()`
- `swapElems()`
- `swapCols()`
- `swapRows()`

### Output

- .toString(precision = 5)`

### Logical

- `isApprox()`
- `isGreater()`
- `isGreaterOrEqual()`
- `isLess()`
- `isLessOrEqual()`
- `isEqual()`
- `isBetween()`
- `isEmpty()`
- `isVector()`
- `isCol()`
- `isRow()`
- `isSquare()`
- `isSymmetric()`
- `isPositiveDefinite()`
- `isFinite()`
- `hasInf()`
- `hasNaN()`
- `hasNull()`
- `find()`

### Statistic

- `min(mat)`
- `max(mat)`
- `range(mat)`
- `mode(mat)`
- `quantile(mat, q, k)`
- `median(mat)`

    Shorthand for `quantile(mat, 2, 1)`.

- `quartile(mat, k)`

    Shorthand for `quantile(mat, 4, k)`.

- `decile(mat, k)`

    Shorthand for `quantile(mat, 10, k)`.

- `percentile(mat, k)`

    Shorthand for `quantile(mat, 100, k)`.

- `interquartileRange(mat)`
- `mean(mat)`
- `centralMoment(mat, k)`
- `variance(mat)`
- `standardDeviation(mat)`
- `standardizedCentralMoment(mat, k)`
- `skewness(mat)`
- `kurtosis(mat)`
- `histogram()`
- `minIndex()`
- `maxIndex()`
- `covariance()`
- `correlation()`
- `comulative(mat, callback)`
- `diff(mat, callback)`

### Trigonometric 

- `.sin()`

    Shorthand for `.map(Math.sin)`.

- `.sinh()`

    Shorthand for `.map(Math.sinh)`.

- `.sinc(flavor='normalised')`

    If `flavor` is set to `'normalised'`:

    Shorthand for `.map(x => _scalarSinc(_multiply(Math.pi, x)))`.

    If `flavor` is set to `'unnormalised'`:

    Shorthand for `.map(_scalarSinc)`.

- `.sinch()`
- `.asin()`

    Shorthand for `.map(Math.asin)`.

- `.asinh()`

    Shorthand for `.map(Math.asinh)`.

- `.cos()`

    Shorthand for `.map(Math.cos)`.

- `.cosh()`

    Shorthand for `.map(Math.cosh)`.

- `.acos()`

    Shorthand for `.map(Math.acos)`.

- `.acosh()`

    Shorthand for `.map(Math.acosh)`.

- `.tan()`

    Shorthand for `.map(Math.tan)`.

- `.tanh()`

    Shorthand for `.map(Math.tanh)`.

- `.tanc()`
- `.tanch()`
- `.atan()`
- `.atanh()`
- `atan2(matA, matB)`
- `hypot(matA, matB)`

### Sets

- `intersect()`
- `setDiff()`
- `union()`
- `unique(mat)`

### Mathematical functions

- `.erf()`
- `.gamma()`
- `.negate()`
- `.reciprocal()`
- `.mod()`
- `.remainder()`
- `.abs()`

    Shorthand for `.map(Math.abs)`.

- `.ceil()`
- `.floor()`
- `.round()`
- `.logE()`
- `.log10()`
- `.log2()`
- `.pow()`
- `.sqrt()`
- `.square()`
- `.cubic()`
- `.cbrt()`
- `.sign()`
- `.expE()`
- `.exp2()`
- `.deg2Rad()`
- `.rad2Deg()`
- `.exp10()`
- `.truncate()`

### Linear algebra

- `cond()`
- `cross()`
- `dot()`
- `det()`
- `transpose()`
- `kron()`
- `norm()`
- `rank()`
- `add()`
- `multiply()`
- `subtract()`
- `divide()`
- `trace()`

---

Distributed under [MIT license](http://opensource.org/licenses/MIT).

---

[Mantella](http://mantella.info/) is an efficiency-focused C++14 header-only library for analysing and solving optimisation problems. As it only depends on the C++ standard library, Linux, Windows and Mac OS X support is a given.

**Our goal for version 1.0 is to combine a high-performance, self-accelerating optimisation experience with an easy to use API.**

Got a Problem or Question?
--------------------------

If you have a question about how to use the Mantella C++ optimisation library, please direct these to [StackOverflow](http://stackoverflow.com/questions/tagged/mantella), or ask us directly on [Gitter](https://gitter.im/Mantella/Mantella).

Found an Issue or Bug?
----------------------

If you found a bug in the source code or a mistake in any kind of documentation, please **let us know** by adding an issue to the [Github Issue Tracker](https://github.com/Mantella/Mantella/issues).

You are welcomed to submit a pull request with your fix afterwards, if at hand.

Requesting a Feature?
---------------------

If you are missing some features within Mantella, feel free to ask us about it by adding a new request to the [Github Issue Tracker](https://github.com/Mantella/Mantella/issues) labelled `feature request`.

Note that submitting a pull request, implementing your requested feature, usually speeds up the process.