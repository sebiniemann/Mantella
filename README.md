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
[linearSpaced](#linspace),
[logSpaced](#logspace),
[meshgrid](#meshgrid),
[ones](#ones),
[randomGamma](#randomGamma),
[randomNormal](#randomNormal),
[randomUniform](#randomUniform),
[repeat](#repeat),
[sample](#sample),
[sequence](#sequence),
[zeros](#zeros)

Layout manipulation:
[appendCol](#appendcol),
[appendRow](#appendrow),
[filterCol](#filterCol),
[filterRow](#filterRow),
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
[col](#col),
[diag](#diag),
[elem](#elem),
[row](#row)

Inplace operations:
[inplaceElem](#inplace),
[inplace](#inplace),
[inplace](#inplace)

Functional programming:
[rotate](#rotate),
[fill](#fill),
[replace](#replace),
[shuffle](#shuffle),
[map](#map),
[mapCol](#mapCol),
[mapRow](#mapRow),
[reduce](#reduce),
[reduceCol](#reduceCol),
[reduceRow](#reduceRow),
[filter](#filter),
[filterCol](#filterCol),
[filterRow](#filterRow),
[some](#some),
[someCol](#someCol),
[someRow](#someRow),
[every](#every),
[everyCol](#everyCol),
[everyRow](#everyRow),
[none](#none),
[noneCol](#noneCol),
[noneRow](#noneRow),
[reverse](#reverse),
[reverseCol](#reverseCol),
[reverseRow](#reverseRow),
[sort](#sort),
[sortCol](#sortCol),
[sortRow](#sortRow),
[count](#count),
[countCol](#countCol),
[countRow](#countRow),
[swapElems](#swapElems),
[swapCols](#swapCols),
[swapRows](#swapRows),

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

Output:
[toString](#tostring)

Inner functionality:
[._raw](#_raw),
[._rng](#_rng),
[_scalarTanc](#_scalarTanc),
[_scalarSinhc](#_scalarSinhc),
[_scalarSinc](#_scalarSinc),
[_scalarSec](#_scalarSec),
[_scalarSech](#_scalarSech),
[_scalarCsc](#_scalarCsc),
[_scalarCsch](#_scalarCsch),
[_scalarCot](#_scalarCot),
[_scalarCoth](#_scalarCoth),
[_scalarAsec](#_scalarAsec),
[_scalarAcsc](#_scalarAcsc),
[_scalarAcsch](#_scalarAcsch),
[_scalarAcot](#_scalarAcot),
[_scalarAcoth](#_scalarAcoth),
[_scalarAsech](#_scalarAsech),
[_scalarRandomNormal](#_scalarRandomNormal),
[_scalarRandomGamma](#_scalarRandomGamma),
[_scalarNegate](#_scalarNegate),
[_scalarAdd](#_scalarAdd),
[_scalarSubtract](#_scalarSubtract),
[_scalarMultiply](#_scalarMultiply),
[_scalarDivide](#_scalarDivide),

---

Distributed under [MIT license](http://opensource.org/licenses/MIT).
