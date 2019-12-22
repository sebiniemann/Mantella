# Salp

Salp is an easy to use matrix library for Javascript. It supports NodeJS as well as browser builds and has no third-party dependencies.

My main goal for Salp is to provide a comprehensive, easy to learn mathematical library for linear algebra that simplifies and accelerates daily work.



## API

### Fields

- `.nElems`

  The number of elements.
  
  Same as `mat.nRows * mat.nCols`.

- `.nRows`

  The number of rows.

- `.nCols`

  The number of columns.

- `._raw`
- `._rng`

### Creation

- `new Mat()`
- `empty(nRows, nCols)`

    Shorthand for: `new Mat(nRows, nCols)`

- `zeros(nRows, nCols)`

    Shorthand for: `new Mat(nRows, nCols, () => 0)`

- `ones(nRows, nCols)`

    Shorthand for: `new Mat(nRows, nCols, () => 1)`

- `identity(nRows, nCols)`

    Shorthand for: `zeros(nRows, nCols).diag(() => 0)`

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

- `.toString(precision=5)`

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

    Shorthand for: `quantile(mat, 2, 1)`

- `quartile(mat, k)`

    Shorthand for: `quantile(mat, 4, k)`

- `decile(mat, k)`

    Shorthand for: `quantile(mat, 10, k)`

- `percentile(mat, k)`

    Shorthand for: `quantile(mat, 100, k)`

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

- `.acos()`

    Shorthand for: `mat.map(Math.acos)`

- `.acosh()`

    Shorthand for: `mat.map(Math.acosh)`

- `.acot()`

    Shorthand for: `mat.map(salp._scalarAcot)`

- `.acoth()`

    Shorthand for: `mat.map(salp._scalarAcoth)`

- `.acsc()`

    Shorthand for: `mat.map(salp._scalarAcsc)`

- `.acsch()`

    Shorthand for: `mat.map(salp._scalarAcsch)`

- `.asec()`

    Shorthand for: `mat.map(salp._scalarAsec)`

- `.asech()`

    Shorthand for: `mat.map(salp._scalarAsech)`
  
- `.asin()`

    Shorthand for: `mat.map(Math.asin)`

- `.asinh()`

    Shorthand for: `mat.map(Math.asinh)`

- `.atan()`

    Shorthand for: `mat.map(Math.atan)`

- `.atanh()`

    Shorthand for: `mat.map(Math.atanh)`

- `.cos()`

    Shorthand for: `mat.map(Math.cos)`

- `.cosh()`

    Shorthand for: `mat.map(Math.cosh)`

- `.cot()`

    Shorthand for: `mat.map(salp._scalarCot)`

- `.coth()`

    Shorthand for: `mat.map(salp._scalarCoth)`

- `.csc()`

    Shorthand for: `mat.map(salp._scalarCsc)`

- `.csch()`

    Shorthand for: `mat.map(salp._scalarCsch)`

- `.sec()`

    Shorthand for: `mat.map(salp._scalarSec)`

- `.sech()`

    Shorthand for: `mat.map(salp._scalarSech)`

- `.sin()`

    Shorthand for: `mat.map(Math.sin)`

- `.sinc(flavor='normalised')`

    If `flavor` is set to `'normalised'`:

    Shorthand for: `mat.map(x => salp._scalarSinc(Math.PI * x))`

    If `flavor` is set to `'unnormalised'`:

    Shorthand for: `mat.map(salp._scalarSinc)`

- `.sinh()`

    Shorthand for: `mat.map(Math.sinh)`

- `.sinhc()`

    Shorthand for: `mat.map(salp._scalarSinhc)`

- `.tan()`

    Shorthand for: `mat.map(Math.tan)`

- `.tanc()`

    Shorthand for: `mat.map(salp._scalarTanc)`

- `.tanh()`

    Shorthand for: `mat.map(Math.tanh)`

- `.tanhc()`

    Shorthand for: `mat.map(salp._scalarTanhc)`

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

    Shorthand for: `mat.map(x => -x)`

- `.reciprocal()`

    Shorthand for: `mat.map(x => 1 / x)`

- `.mod()`
- `.remainder()`
- `.abs()`

    Shorthand for: `mat.map(Math.abs)`

- `.ceil()`

    Shorthand for: `mat.map(Math.ceil)`

- `.floor()`

    Shorthand for: `mat.map(Math.floor)`

- `.round()`

    Shorthand for: `mat.map(Math.round)`

- `.log()`

    Shorthand for: `mat.map(Math.log)`.

- `.logK(k)`

    Shorthand for:
    
    ```js
    const log2k = Math.log2(k);
    mat.map(x => Math.log2(x) / log2k);
    ```

- `.log2()`

    Shorthand for: `mat.map(Math.log2)`

- `.log10()`

    Shorthand for: `mat.map(Math.log10)`

- `.pow(exponent)`

    Shorthand for: `mat.map(x => Math.pow(x, exponent))`

- `.root(k)`

    Shorthand for: `mat.pow(1 / k)`

- `.sqrt()`

    Shorthand for: `mat.map(Math.sqrt)`

- `.clz32()`

    Shorthand for: `mat.map(Math.clz32)`

- `.expm1()`

    Shorthand for: `mat.map(Math.expm1)`

- `.fround()`

    Shorthand for: `mat.map(Math.fround)`

- `.log1p()`

    Shorthand for: `mat.map(Math.log1p)`

- `.square()`

    Shorthand for: `mat.pow(2)`

- `.cube()`

    Shorthand for: `mat.pow(3)`

- `.cbrt()`

    Shorthand for: `mat.map(Math.cbrt)`

- `.sign()`

    Shorthand for: `mat.map(Math.sign)`

- `.exp()`

    Shorthand for: `mat.map(Math.exp)`

- `.expK(k)`

    Shorthand for: `mat.map(x => Math.pow(k, x))`

- `.exp2()`

    Shorthand for: `mat.expK(2)`

- `.exp10()`

    Shorthand for: `mat.expK(10)`

- `.deg2Rad()`
- `.rad2Deg()`

- `.trunc()`

    Shorthand for: `mat.map(Math.trunc)`

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