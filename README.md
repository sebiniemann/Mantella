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
[erf](#erf),
[gamma](#gamma),
[negate](#negate),
[reciprocal](#reciprocal),
[mod](#mod),
[remainder](#remainder),
[abs](#abs),
[ceil](#ceil),
[floor](#floor),
[round](#round),
[log](#log),
[logK](#logK),
[log2](#log2),
[log10](#log10),
[pow](#pow),
[root](#root),
[sqrt](#sqrt),
[clz32](#clz32),
[expm1](#expm1),
[fround](#fround),
[log1p](#log1p),
[square](#square),
[cube](#cube),
[cbrt](#cbrt),
[sign](#sign),
[exp](#exp),
[expK](#expK),
[exp2](#exp2),
[exp10](#exp10),
[deg2Rad](#deg2Rad),
[rad2Deg](#rad2Deg),
[trunc](#trunc),

Trigonometric & hyperbolic functions:
[acos](#acos),
[acosh](#acosh),
[acot](#acot),
[acoth](#acoth),
[acsc](#acsc),
[acsch](#acsch),
[asec](#asec),
[asech](#asech),
[asin](#asin),
[asinh](#asinh),
[atan](#atan),
[atan2](#atan2),
[atanh](#atanh),
[cos](#cos),
[cosh](#cosh),
[cot](#cot),
[coth](#coth),
[csc](#csc),
[csch](#csch),
[hypot](#hypot),
[sec](#sec),
[sech](#sech),
[sin](#sin),
[sinc](#sinc),
[sinh](#sinh),
[sinhc](#sinhc),
[tan](#tan),
[tanc](#tanc),
[tanh](#tanh),
[tanhc](#tanhc)

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





### arange()
### linspace()
### logspace()
### geomspace()
### meshGrid()
### repeat()
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









### _negate(a, b)
### _add(a, b)
### _subtract(a, b)
### _multiply(a, b)
### _divide(a, b)

---

Distributed under [MIT license](http://opensource.org/licenses/MIT).
