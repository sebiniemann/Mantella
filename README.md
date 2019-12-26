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
- arange
- linspace
- logspace
- geomspace
- meshGrid
- repeat
- sample

Accessing and indexing:
[col](#col),
[diag](#diag),
[elem](#elem),
[row](#row)

Inplace operations:
[inplaceElem](#inplace),
[inplace](#inplace),
[inplace](#inplace)


Functional programming:
- rotate
- fill
- replace
- shuffle
- map
- mapCol
- mapRow
- reduce
- reduceCol
- reduceRow
- filter
- filterCol
- filterRow
- some
- someCol
- someRow
- every
- everyCol
- everyRow
- none
- noneCol
- noneRow
- reverse
- reverseCol
- reverseRow
- sort
- sortCol
- sortRow
- count
- countCol
- countRow
- swapElems
- swapCols
- swapRows

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
[abs](#abs),
[cbrt](#cbrt),
[ceil](#ceil),
[clamp](#clamp),
[clz32](#clz32),
[cube](#cube),
[deg2Rad](#deg2Rad),
[erf](#erf),
[exp](#exp),
[exp2](#exp2),
[exp10](#exp10),
[expK](#expK),
[expm1](#expm1),
[floor](#floor),
[fround](#fround),
[gamma](#gamma),
[log](#log),
[log2](#log2),
[log10](#log10),
[log1p](#log1p),
[logK](#logK),
[mod](#mod),
[negate](#negate),
[pow](#pow),
[rad2Deg](#rad2Deg),
[reciprocal](#reciprocal),
[remainder](#remainder),
[root](#root),
[round](#round),
[sign](#sign),
[sqrt](#sqrt),
[square](#square),
[trunc](#trunc)

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
- _scalarNegate
- _scalarAdd
- _scalarSubtract
- _scalarMultiply
- _scalarDivide

---

Distributed under [MIT license](http://opensource.org/licenses/MIT).
