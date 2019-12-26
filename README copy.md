# Salp

Salp, an easy to learn/use linear algebra library for Javascript.

It supports NodeJS as well as browser builds and has no third-party dependencies.

If you have a question, problem or feature request, feel free to contact me anytime. The best way (for me) is to use the [Github issue tracker](https://github.com/SebastianNiemann/Mantella/issues).

## Roadmap

- Support of basic matrix manipulations
- Support of matrix layout manipulations
- Support of common mathematical & trigonometric functions
- Support of common statistical functions
- Support of common set operations
- Support of common decompositions, factorizations, inversions and equation solvers
- Support of complex numbers
- Acceleration of BLAS/LAPACK-like operations with WebAssembly

## API

Fields:
[nCols](#nCols),
[nElems](#nElems),
[nRows](#nRows)

Creation:
[Constructor](#constructor),
[empty](#empty),
[identity](#identity),
[ones](#ones),
[randomGamma](#randomGamma),
[randomNormal](#randomNormal),
[randomUniform](#randomUniform),
[zeros](#zeros)

Layout manipulation:
[appendCol](#appendcol),
[appendRow](#appendrow),
[flat](#flat),
[insertCol](#insertcol),
[insertRow](#insertrow),
[lowerTriangular](#lowertriangular),
[prependCol](#prependcol),
[prependRow](#prependrow),
[removeCol](#removecol),
[removeRow](#removerow),
[resize](#resize),
[upperTriangular](#uppertriangular)

Accessing and indexing:


Inplace operations:


Functional programming:


Logical functions:
[find](#find),
[hasInf](#hasinf),
[hasNaN](#hasnan),
[hasNull](#hasnull),
[isApproxEqual](#isapproxequal),
[isBetween](#isbetween),
[isCol](#iscol),
[isEmpty](#isempty),
[isEqual](#isequal),
[isFinite](#isfinite),
[isGreater](#isgreater),
[isGreaterOrEqual](#isgreaterorequal),
[isLess](#isless),
[isLessOrEqual](#islessorequal),
[isPositiveDefinite](#ispositivedefinite),
[isRow](#isrow),
[isSquare](#issquare),
[isSymmetric](#issymmetric),
[isVector](#isvector)

Basic mathematical functions:

Trigonometric & hyperbolic functions:

Statistical function:
[centralMoment](#centralmoment),
[comulative](#comulative),
[correlation](#correlation),
[covariance](#covariance),
[decile](#decile),
[diff](#diff),
[histogram](#histogram),
[interquartileRange](#interquartilerange),
[kurtosis](#kurtosis),
[max](#max),
[maxIndex](#maxIndex),
[mean](#mean),
[median](#median),
[min](#min),
[minIndex](#minIndex),
[mode](#mode),
[percentile](#percentile),
[quantile](#quantile),
[quartile](#quartile),
[range](#range),
[skewness](#skewness),
[standardDeviation](#standarddeviation),
[standardizedCentralMoment](#standardizedcentralmoment),
[variance](#variance)

Set operations:
[setDifference](#setdifference),
[setIntersect](#setintersect),
[setSymmetricDifference](#setsymmetricdifference),
[setUnion](#setunion),
[unique](#unique)

Linear algebra:
[add](#add),
[cond](#cond),
[cross](#cross),
[det](#det),
[divide](#divide),
[dot](#dot),
[kron](#kron),
[multiply](#multiply),
[norm](#norm),
[rank](#rank),
[subtract](#subtract),
[trace](#trace)
[transpose](#transpose)

Output: [toString](#tostring)

Inner functionality:
[._raw](#_raw),
[._rng](#_rng)

---

### .nCols

The number of columns.

---

### .nElems

The number of elements.
  
Same as `mat.nRows * mat.nCols`.

---

### .nRows

The number of rows.

---

### Constructor

- new Mat()
- new Mat(nRows, nCols)
- new Mat(nRows, nCols, callback)
- new Mat(array)
- new Mat(arrayOfArray)
- new Mat(mat)

---

### empty

- empty(nRows, nCols)

Shorthand for: `new Mat(nRows, nCols)`

---

### zeros

- zeros(nRows, nCols)

Shorthand for: `new Mat(nRows, nCols, () => 0)`

---

### ones

- ones(nRows, nCols)

Shorthand for: `new Mat(nRows, nCols, () => 1)`

---

### identity

- identity(nRows, nCols)

Shorthand for: `zeros(nRows, nCols).diag(() => 0)`

---

### randomUniform

- randomUniform(nRows, nCols)
- randomUniform(nRows, nCols, { min: 0, max: 1 })

Shorthand for: `zeros(nRows, nCols).diag(() => _scalarRandomUniform())

---

### randomNormal

- randomNormal(nRows, nCols)
- randomNormal(nRows, nCols, { mean: 0, standardDeviation: 1 })

Shorthand for: `zeros(nRows, nCols).diag(() => _scalarRandomNormal())``

---

### randomGamma

- randomGamma(nRows, nCols)
- randomGamma(nRows, nCols, { shape: 1, scale: 1 })

Shorthand for: `zeros(nRows, nCols).diag(() => _scalarRandomGamma())`

---

### arange()

---

### linspace()

---

### logspace()

---

### geomspace()

---

### meshGrid()

---

### repeat()

---

### sample()

### .elem()
### .elemDiag()
### .col()
### .row()
### .rotate()
### .fill()
### .replace()
### .shuffle(mat)
### .clamp(mat, lowerBounds, upperBounds)

### .resize(nRows, nCols)
### .flat()
### .appendCol()
### .appendRow()
### .prependCol()
### .prependRow()
### .insertCol()
### .insertRow()
### .removeCol()
### .removeRow()
### .lowerTriangular()
### .upperTriangular()

### .map(callback)
### .mapCol(callback)
### .mapRow(callback)
### .reduce(callback, initialValue)
### .reduceCol(callback, initialValue)
### .reduceRow(callback, initialValue)
### .filter(callback)
### .filterCol()
### .filterRow()
### .some()
### .someCol()
### .someRow()
### .every()
### .everyCol()
### .everyRow()
### .none()
### .noneCol()
### .noneRow()
### .reverse()
### .reverseCol()
### .reverseRow()
### .sort()
### .sortCol()
### .sortRow()
### .count()
### .countCol()
### .countRow()
### swapElems()
### swapCols()
### swapRows()

### .toString(precision=5)

### isApprox()
### isGreater()
### isGreaterOrEqual()
### isLess()
### isLessOrEqual()
### isEqual()
### isBetween()
### isEmpty()
### isVector()
### isCol()
### isRow()
### isSquare()
### isSymmetric()
### isPositiveDefinite()
### isFinite()
### hasInf()
### hasNaN()
### hasNull()
### find()

### min(mat)
### max(mat)
### range(mat)
### mode(mat)
### quantile(mat, q, k)
### median(mat)

    Shorthand for: `quantile(mat, 2, 1)`

### quartile(mat, k)

    Shorthand for: `quantile(mat, 4, k)`

### decile(mat, k)

    Shorthand for: `quantile(mat, 10, k)`

### percentile(mat, k)

    Shorthand for: `quantile(mat, 100, k)`

### interquartileRange(mat)
### mean(mat)
### centralMoment(mat, k)
### variance(mat)
### standardDeviation(mat)
### standardizedCentralMoment(mat, k)
### skewness(mat)
### kurtosis(mat)
### histogram()
### minIndex()
### maxIndex()
### covariance()
### correlation()
### comulative(mat, callback)
### diff(mat, callback)

### .acos()

    Shorthand for: `mat.map(Math.acos)`

### .acosh()

    Shorthand for: `mat.map(Math.acosh)`

### .acot()

    Shorthand for: `mat.map(salp._scalarAcot)`

### .acoth()

    Shorthand for: `mat.map(salp._scalarAcoth)`

### .acsc()

    Shorthand for: `mat.map(salp._scalarAcsc)`

### .acsch()

    Shorthand for: `mat.map(salp._scalarAcsch)`

### .asec()

    Shorthand for: `mat.map(salp._scalarAsec)`

### .asech()

    Shorthand for: `mat.map(salp._scalarAsech)`
  
### .asin()

    Shorthand for: `mat.map(Math.asin)`

### .asinh()

    Shorthand for: `mat.map(Math.asinh)`

### .atan()

    Shorthand for: `mat.map(Math.atan)`

### .atanh()

    Shorthand for: `mat.map(Math.atanh)`

### .cos()

    Shorthand for: `mat.map(Math.cos)`

### .cosh()

    Shorthand for: `mat.map(Math.cosh)`

### .cot()

    Shorthand for: `mat.map(salp._scalarCot)`

### .coth()

    Shorthand for: `mat.map(salp._scalarCoth)`

### .csc()

    Shorthand for: `mat.map(salp._scalarCsc)`

### .csch()

    Shorthand for: `mat.map(salp._scalarCsch)`

### .sec()

    Shorthand for: `mat.map(salp._scalarSec)`

### .sech()

    Shorthand for: `mat.map(salp._scalarSech)`

### .sin()

    Shorthand for: `mat.map(Math.sin)`

### .sinc(flavor='normalised')

    If `flavor` is set to `'normalised'`:

    Shorthand for: `mat.map(x => salp._scalarSinc(Math.PI * x))`

    If `flavor` is set to `'unnormalised'`:

    Shorthand for: `mat.map(salp._scalarSinc)`

### .sinh()

    Shorthand for: `mat.map(Math.sinh)`

---

### .sinhc()

    Shorthand for: `mat.map(salp._scalarSinhc)`

---

### .tan()

Shorthand for: `mat.map(Math.tan)`

---

### .tanc()

Shorthand for: `mat.map(salp._scalarTanc)`

---

### .tanh()

Shorthand for: `mat.map(Math.tanh)`

---

### .tanhc()

Shorthand for: `mat.map(salp._scalarTanhc)`

---

### atan2(matA, matB)

---

### hypot(matA, matB)

### setIntersect
### setDifference(matA, matB)
### setSymmetricDifference(matA, matB)
### setUnion(matA, matB)
### unique(mat)

### .erf()
### .gamma()
### .negate()

    Shorthand for: `mat.map(x => -x)`

### .reciprocal()

    Shorthand for: `mat.map(x => 1 / x)`

### .mod()
### .remainder()
### .abs()

    Shorthand for: `mat.map(Math.abs)`

### .ceil()

    Shorthand for: `mat.map(Math.ceil)`

### .floor()

    Shorthand for: `mat.map(Math.floor)`

### .round()

    Shorthand for: `mat.map(Math.round)`

### .log()

    Shorthand for: `mat.map(Math.log)`.

### .logK(k)

    Shorthand for:
    
    ```js
    const log2k = Math.log2(k);
    mat.map(x => Math.log2(x) / log2k);
    ```

### .log2()

    Shorthand for: `mat.map(Math.log2)`

### .log10()

    Shorthand for: `mat.map(Math.log10)`

### .pow(exponent)

    Shorthand for: `mat.map(x => Math.pow(x, exponent))`

### .root(k)

    Shorthand for: `mat.pow(1 / k)`

### .sqrt()

    Shorthand for: `mat.map(Math.sqrt)`

### .clz32()

    Shorthand for: `mat.map(Math.clz32)`

### .expm1()

    Shorthand for: `mat.map(Math.expm1)`

### .fround()

    Shorthand for: `mat.map(Math.fround)`

### .log1p()

    Shorthand for: `mat.map(Math.log1p)`

### .square()

    Shorthand for: `mat.pow(2)`

### .cube()

    Shorthand for: `mat.pow(3)`

### .cbrt()

    Shorthand for: `mat.map(Math.cbrt)`

### .sign()

    Shorthand for: `mat.map(Math.sign)`

### .exp()

    Shorthand for: `mat.map(Math.exp)`

### .expK(k)

    Shorthand for: `mat.map(x => Math.pow(k, x))`

### .exp2()

    Shorthand for: `mat.expK(2)`

### .exp10()

    Shorthand for: `mat.expK(10)`

### .deg2Rad()
### .rad2Deg()

### .trunc()

    Shorthand for: `mat.map(Math.trunc)`

### cond()
### cross()
### dot()
### det()
### transpose()
### kron()
### norm()
### rank()
### add()
### multiply()
### subtract()
### divide()
### trace()

### _negate(a, b)
### _add(a, b)
### _subtract(a, b)
### _multiply(a, b)
### _divide(a, b)

---

Distributed under [MIT license](http://opensource.org/licenses/MIT).
