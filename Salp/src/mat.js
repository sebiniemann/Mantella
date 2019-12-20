class Mat {
  constructor() {
    this.raw = [];

    switch (arguments.length) {
      case 0:
        // new Mat()
        this._constructorEmpty();
        break;
      case 1:
        if (Array.isArray(arguments[0])) {
          if (Array.isArray(arguments[0][0])) {
            // new Mat([
            //   [a1, a2, ...],
            //   [b1, b2, ...],
            //   ...
            // ])
            this._constructorArrayOfArrays(...arguments);
          } else {
            // new Mat([x1, x2, ...])
            this._constructorArray(...arguments);
          }
        } else {
          // new Mat(mat)
          this._constructorMat(...arguments);
        }
        break;
      case 2:
        // new Mat(nRows, nCols)
        this._constructorDimensions(...arguments);
        break;
    }
  }

  _constructorEmpty() {
    this.resize(0, 0);
  }

  _constructorArrayOfArrays(arrayOfArrays) {
    this.resize(arrayOfArrays.length, arrayOfArrays[0].length);

    arrayOfArrays.forEach((row, rowIndex) => {
      this._rowIndexCallback(rowIndex, () => row);
    });
  }

  _constructorArray(array) {
    this.resize(array.length, 1);
    this.raw = [...array];
  }

  _constructorMat(mat) {
    this.resize(mat.nRows, mat.nCols);
    this.raw = [...mat.raw];
  }

  _constructorDimensions(nRows, nCols) {
    this.resize(nRows, nCols);
  }

  resize(nRows, nCols) {
    this.nRows = nRows;
    this.nCols = nCols;
    this.nElems = this.nRows * this.nCols;

    this.raw.length = this.nElems;
  }

  elem() {
    switch (arguments.length) {
      case 1:
        if (typeof arguments[0] === 'number') {
          // elem(n)
          return this._elemIndex(...arguments);
        } else if (Array.isArray(arguments[0])) {
          // elem([n1, n2, ...])
          return this._elemIndices(...arguments);
        } else {
          // elem(callback)
          return this._elemCallback(...arguments);
        }
      case 2:
        if (typeof arguments[0] === 'number') {
          if (typeof arguments[1] === 'number') {
            // elem(x, y)
            return this._elemSubscript(...arguments);
          } else if (Array.isArray(arguments[1])) {
            // elem(x, [y1, y2, ...])
            return this._elemSubscripts([arguments[0]], arguments[1]);
          } else {
            // elem(n, callback)
            return this._elemIndexCallback(...arguments);
          }
        } else {
          if (typeof arguments[1] === 'number') {
            // elem([x1, x2, ...], y)
            return this._elemSubscripts(arguments[0], [arguments[1]]);
          } else if (Array.isArray(arguments[1])) {
            // elem([x1, x2, ...], [y1, y2, ...])
            return this._elemSubscripts(...arguments);
          } else {
            // elem([n1, n2, ...], callback)
            return this._elemIndicesCallback(...arguments);
          }
        }
      case 3:
        if (typeof arguments[0] === 'number') {
          if (typeof arguments[0] === 'number') {
            // elem(x, y, callback)
            return this._elemSubscriptCallback(...arguments);
          } else {
            // elem(x, [y1, y2, ...], callback)
            return this._elemSubscriptsCallback(...arguments);
          }
        } else {
          if (typeof arguments[1] === 'number') {
            // elem([x1, x2, ...], y, callback)
            return this._elemSubscriptsCallback(...arguments);
          } else {
            // elem([x1, x2, ...], [y1, y2, ...], callback)
            return this._elemSubscriptsCallback(...arguments);
          }
        }
    }
  }

  _elemCallback(callback) {
    for (let index = 0; index < this.nElems; ++index) {
      this._elemIndexCallback(index, callback);
    }

    return this;
  }

  _elemIndex(index) {
    return this.raw[index];
  }

  _elemIndexCallback(index, callback) {
    this.raw[index] = callback(this.raw[index], index, this);

    return this;
  }

  _elemIndices(indices) {
    const col = new Mat(indices.length, 1);

    let subIndex = 0;
    for (const index of indices) {
      col._elemIndexCallback(subIndex, () => this._elemIndex(index));

      ++subIndex;
    }

    return col;
  }

  _elemIndicesCallback(indices, callback) {
    for (const index of indices) {
      this._elemIndexCallback(index, callback);
    }

    return this;
  }

  _subscriptToIndex(rowSubscript, colSubscript) {
    return  colSubscript * this.nRows + rowSubscript;
  }

  _elemSubscript(rowSubscript, colSubscript) {
    const index = this._subscriptToIndex(rowSubscript, colSubscript);

    return this._elemIndex(index);
  }

  _elemSubscriptCallback(rowSubscript, colSubscript, callback) {
    const index = this._subscriptToIndex(rowSubscript, colSubscript);

    this._elemIndexCallback(index, callback);

    return this;
  }

  _elemSubscripts(rowSubscripts, colSubscripts) {
    const mat = new Mat(rowSubscripts.length, colSubscripts.length);

    let subIndex = 0;
    for (const colSubscript of colSubscripts) {
      for (const rowSubscript of rowSubscripts) {
        mat._elemIndexCallback(subIndex, () => this._elemSubscript(rowSubscript, colSubscript));
        
        ++subIndex;
      }
    }
    
    return mat;
  }

  _elemSubscriptsCallback(rowSubscripts, colSubscripts, callback) {
    for (const colSubscript of colSubscripts) {
      for (const rowSubscript of rowSubscripts) {
        this._elemSubscriptCallback(rowSubscript, colSubscript, callback);
      }
    }

    return this;
  }

  elemDiag() {
    switch (arguments.length) {
      case 0:
        return this._elemDiagK(0);
      case 1:
        if (typeof arguments[0] === 'number'){
          return this._elemDiagK(...arguments);
        } else {
          return this._elemDiagKCallback(0, ...arguments);
        }
      case 2:
        return this._elemDiagKCallback(...arguments);
    }
  }

  _elemDiagK(k) {
    const length = (k < 0 ? this.nRows + k : this.nCols - k);
    
    const mat = new Mat(length, 1);



    return mat;
  }

  col() {
    switch (arguments.length) {
      case 1:
        if (typeof arguments[0] === 'number') {
          // col(n)
          return this._colIndex(...arguments);
        } else if (Array.isArray(arguments[0])) {
          // col([n1, n2, ...])
          return this._colIndices(...arguments);
        } else {
          // col(callback)
          return this._colCallback(...arguments);
        }
      case 2:
        if (typeof arguments[0] === 'number') {
          // col(n, callback)
          return this._colIndexCallback(...arguments);
        } else {
          // col([n1, n2, ...], callback)
          return this._colIndicesCallback(...arguments);
        }
    }
  }

  _colCallback(callback) {
    for (colIndex = 0; colIndex < this.nCols; ++colIndex) {
      this._colIndexCallback(colIndex, callback);
    }

    return this;
  }

  _colIndex(colIndex) {
    const col = new Mat(this.nRows, 1);

    for (rowIndex = 0; rowIndex < this.nRows; ++rowIndex) {
      col._elemIndexCallback(rowIndex, () => this._elemSubscript(rowIndex, colIndex));
    }

    return col;
  }

  _colIndexCallback(colIndex, callback) {
    const col = callback(this._colIndex(colIndex), colIndex, this);

    for (rowIndex = 0; rowIndex < this.nRows; ++rowIndex) {
      this._elemSubscriptCallback(rowIndex, colIndex, () => col._elemIndex(rowIndex));
    }

    return this;
  }

  _colIndices(colIndices) {
    const cols = new Mat(this.nRows, colIndices.length);

    let subColIndex = 0;
    for (const colIndex of colIndices) {
      cols.colIndexCallback(subColIndex, () => this._colIndex(colIndex));

      ++subColIndex;
    }

    return cols;
  }

  _colIndicesCallback(colIndices, callback) {
    for (const colIndex of colIndices) {
      this._colIndexCallback(colIndex, callback);
    }

    return this;
  }

  row() {
    switch (arguments.length) {
      case 1:
        if (typeof arguments[0] === 'number') {
          // row(n)
          return this._rowIndex(...arguments);
        } else if (Array.isArray(arguments[0])) {
          // row([n1, n2, ...])
          return this._rowIndices(...arguments);
        } else {
          // row(callback)
          return this._rowCallback(...arguments);
        }
      case 2:
        if (typeof arguments[0] === 'number') {
          // row(n, callback)
          return this._rowIndexCallback(...arguments);
        } else {
          // row([n1, n2, ...], callback)
          return this._rowIndicesCallback(...arguments);
        }
    }
  }

  _rowCallback(callback) {
    for (rowIndex = 0; rowIndex < this.nRows; ++rowIndex) {
      _rowIndex(rowIndex, callback);
    }

    return this;
  }

  _rowIndex(rowIndex) {
    const row = new Mat(1, this.nCols);

    for (colIndex = 0; colIndex < this.nCols; ++colIndex) {
      row._elemIndexCallback(colIndex, () => this._elemSubscript(rowIndex, colIndex));
    }

    return row;
  }

  _rowIndexCallback(rowIndex, callback) {
    const row = callback(this._rowIndex(rowIndex), rowIndex, this);

    for (colIndex = 0; colIndex < this.nCols; ++colIndex) {
      this._elemSubscriptCallback(rowIndex, colIndex, () => row._elemIndex(colIndex));
    }

    return this;
  }

  _rowIndices(rowIndices) {
    const rows = new Mat(rowIndices.length, this.nCols);

    let subRowIndex = 0;
    for (const rowIndex of rowIndices) {
      rows.rowIndexCallback(subRowIndex, () => this._rowIndex(rowIndex));

      ++subRowIndex;
    }

    return rows;
  }

  _rowIndicesCallback(rowIndices, callback) {
    for (const rowIndex of rowIndices) {
      this._rowIndexCallback(rowIndex, callback);
    }

    return this;
  }

  map(callback) {
    const mat = new Mat(this);

    mat._elemCallback(callback);

    return mat;
  }

  mapCol(callback) {
    const mat = new Mat(this);

    mat._colCallback(callback);

    return mat;
  }

  mapRow(callback) {
    const mat = new Mat(this);

    mat._rowCallback(callback);

    return mat;
  }

  _initialiseAccumulator(initialValue, fallbackFn) {
    let accumulator = initialValue;

    if (accumulator === undefined) {
      if (this.nElems === 0) {
        throw new TypeError('Either the initial value must be set, or the matrix must not be empty.')
      }

      accumulator = fallbackFn(0);
    }

    return accumulator;
  }

  reduce(callback, initialValue) {
    const fallbackFn = this._elemIndex;
    let accumulator = _initialiseAccumulator(initialValue, fallbackFn);
    
    for (let index = 0; index < this.nElems; ++index) {
      accumulator = callback(accumulator, this._elemIndex(index), index, this);
    }

    return accumulator;
  }

  reduceCol(callback, initialValue) {
    const fallbackFn = this._colIndex;
    let accumulator = _initialiseAccumulator(initialValue, fallbackFn);
    
    for (let colIndex = 0; colIndex < this.nCols; ++colIndex) {
      accumulator = callback(accumulator, this._colIndex(colIndex), colIndex, this);
    }

    return accumulator;
  }

  reduceRow(callback, initialValue) {
    const fallbackFn = this._rowIndex;
    let accumulator = _initialiseAccumulator(initialValue, fallbackFn);
    
    for (let rowIndex = 0; rowIndex < this.nRows; ++rowIndex) {
      accumulator = callback(accumulator, this._rowIndex(rowIndex), rowIndex, this);
    }

    return accumulator;
  }

  filter(callback) {
    const data = [];

    for (let index = 0; index < this.nElems; ++index) {
      const value = this._elemIndex(index);
      
      if (callback(value)) {
        data.push(value);
      }
    }

    return new Mat(data);
  }

  filterCol() {

  }

  filterRow() {

  }

  some() {

  }

  someCol() {

  }

  someRow() {

  }

  every() {

  }

  everyCol() {

  }

  everyRow() {

  }

  reverse() {

  }

  reverseCol() {

  }

  reverseRow() {

  }

  sort() {

  }

  sortCol() {

  }

  sortRow() {

  }

  toString(precision = 5) {
    let output = '';

    for (let rowIndex = 0; rowIndex < this.nRows; ++rowIndex) {
      output += (rowIndex === 0 ? ' [[ ' : '  [ ');

      for (let colIndex = 0; colIndex < this.nCols; ++colIndex) {
        output += this._elemSubscript(rowIndex, colIndex)
          .toPrecision(precision)
          .replace(/\.0+$/, '')
          .padStart(precision + 5, ' ');
        output += ', ';
      }

      output += (rowIndex === this.nRows - 1 ? ']]' : '],\n');
    }

    return output;
  }
}

//
// Stand-alone functions
//

// Creation

function empty(nRows, nCols) {
  return new Mat(nRows, nCols);
}

function zeros(nRows, nCols) {
  const mat = new Mat(nRows, nCols);

  mat.fill(0);

  return mat;
}

function ones(nRows, nCols) {
  const mat = new Mat(nRows, nCols);

  mat.fill(1);

  return mat;
}

function trace() {
  
}

function identity(nRows, nCols) {
  const mat = zeros(nRows, nCols);

  mat.diag(() => 1);

  return mat;
}

function normal(nRows, nCols) {

}

function uniform(nRows, nCols) {

}

function gamma(nRows, nCols) {

}

function cond() {

}

function repeat() {

}

function isApprox() {

}

function cross() {

}

function dot() {

}

function det() {

}

// Statistics

function min(mat) {
  return mat.raw.reduce((acc, value) => Math.min(acc, value), Number.POSITIVE_INFINITY);
}

function max(mat) {
  return mat.raw.reduce((acc, value) => Math.max(acc, value), Number.NEGATIVE_INFINITY);
}

function range(mat) {
  return max(mat) - min(mat);
}

function mode(mat) {
  const counts = {};

  let _mode;
  let maxCounts = 0;
  mat.raw.forEach(value => {
    counts[value] = (counts[value] || 0) + 1;

    if (counts[value] > maxCounts) {
      _mode = value;
      maxCounts = counts[value];
    }
  });

  return _mode;
}

function quantile(mat, q, k) {
  const rank = k / q * (mat.nElems - 1) + 1;
  const flooredRank = Math.floor(rank);

  const sorted = [...mat.raw].sort();
  const lower = sorted[flooredRank - 1];
  const upper = sorted[flooredRank];

  return lower + (rank - flooredRank) * (upper - lower);
}

function median(mat) {
  return quantile(mat, 2, 1);
}

function quartile(mat, k) {
  return quantile(mat, 4, k);
}

function decile(mat, k) {
  return quantile(mat, 10, k);
}

function percentile(mat, k) {
  return quantile(mat, 100, k);
}

function interquartileRange(mat) {
  return quartile(mat, 3) - quartile(mat, 1);
}

function mean(mat) {
  return mat.raw.reduce((acc, value) => acc + value, 0) / mat.nElems;
}

function centralMoment(mat, k) {
  const _mean = mean(mat);

  return mat.raw.reduce((acc, value) => acc + Math.pow((value - _mean), k), 0) / mat.nElems;
}

function variance(mat) {
  const _variance = centralMoment(mat, 2);
  const sampleCorrection = (mat.nElems / (mat.nElems - 1));

  return _variance * sampleCorrection;
}

function standardDeviation(mat) {
  return Math.sqrt(variance(mat));
}

function standardizedCentralMoment(mat, k) {
  const _centralMoment = centralMoment(mat, k);
  const _standardDeviation = standardDeviation(mat);

  return _centralMoment / Math.pow(_standardDeviation, k);
}

function skewness(mat) {
  const _skewness = standardizedCentralMoment(mat, 3);
  const sampleCorrection = Math.pow(mat.nElems, 2) / ((mat.nElems - 1) * (mat.nElems - 2));

  return _skewness * sampleCorrection;
}

function kurtosis(mat) {
  const _kurtosis = standardizedCentralMoment(mat, 4);
  const sampleCorrectionA = (Math.pow(mat.nElems, 2) * (mat.nElems + 1)) / ((mat.nElems - 1) * (mat.nElems - 2) * (mat.nElems - 3));
  const sampleCorrectionB = (9 * mat.nElems - 15) / ((mat.nElems - 2) * (mat.nElems - 3));

  return sampleCorrectionA * _kurtosis - sampleCorrectionB;
}

// Others

function comulative(mat, callback) {

}

function diff(mat, callback) {

}

function unique(mat) {
  return new Mat([...new Set(mat.raw)]);
}

function shuffle(mat) {

}

function fill() {
  
}

function replace() {

}

function appendCol() {

}

function appendRow() {

}

function prependCol() {

}

function prependRow() {

}

function insertCol() {

}

function insertRow() {

}

function removeCol() {

}

function removeRow() {

}

function transpose() {
  
}

function clamp(mat, lowerBounds, upperBounds) {
  return mat.map(x => Math.min(Math.max(x, lowerBounds), upperBounds));
}

function range() {

}

function covariance() {

}

function correlation() {

}

function abs() {
  
}

function acos() {

}

function acosh() {
  
}

function asin() {
  
}

function asinh() {
  
}

function atan() {
  
}

function atan2() {
  
}

function atanh() {
  
}

function ceil() {
  
}

function cos() {
  
}

function cosh() {
  
}

function floor() {
  
}

function round() {
  
}

function logE() {
  
}

function log10() {
  
}

function log2() {
  
}

function pow() {
  
}

function sqrt() {
  
}

function square() {
  
}

function cubic() {
  
}

function cbrt() {
  
}

function sign() {
  
}

function sin() {
  
}

function sinh() {
  
}

function tan() {
  
}

function tanh() {
  
}

function hypot() {
  
}

function exp() {
  
}

function truncate() {

}

function minIndex() {

}

function maxIndex() {

}

function add() {

}

function multiply() {
  
}

function subtract() {
  
}

function divide() {
  
}

function transpose() {

}

function isBetween() {

}

function isEmpty() {
  
}

function isVector() {
  
}

function isCol() {
  
}

function isRow() {
  
}

function isSquare() {
  
}

function isSymmetric() {
  
}

function isPositiveDefinite() {

}

function isFinite() {

}

function hasInf() {
  
}

function hasNaN() {
  
}

function hasNull() {
  
}

function find() {
  
}

function intersect() {

}

function kron() {

}

function norm() {

}

function rank() {

}

function erf() {

}

function gamma() {

}

function 

// Phase 1

// Standard Arrays unterstützen
// Finanz-Funktionen unterstützen
// skipNull
// group

// Phase 3

// singular value decomposition
// solve linear equation
// QR decomposition
// Moore-Penrose pseudo-inverse
// orthonormal basis
// LU decomposition
// Inverse
// Eigen decomposition
// Cholesky decomposition

// Phase 4

// RNG fixieren

export {
  Mat,
};



(async function main() {
  let mat = new Mat([
    [1, 3, 5],
    [2, 4, 6],
  ]);
  console.log(mat);

  mat.elem(1, () => 5);
  mat.elem([0, 1], x => x * 2);
  mat.elem([0, 1], [1], x => -x + 2.1111111);
  console.log(String(mat.elem([0, 1, 0], [2, 0, 0, 1])));

  mat = new Mat([21.3, 38.4, 12.7, 12.3, 41.6, 12.3]);
  console.log('min: ', min(mat));
  console.log('max: ', max(mat));
  console.log('range: ', range(mat));
  console.log('mode: ', mode(mat));
  console.log('median: ', median(mat));
  console.log('quartile: ', quartile(mat, 1));
  console.log('decile: ', decile(mat, 2));
  console.log('percentile: ', percentile(mat, 12));
  console.log('interquartileRange: ', interquartileRange(mat));
  console.log('mean: ', mean(mat));
  console.log('variance: ', variance(mat));
  console.log('standardDeviation: ', standardDeviation(mat));
  console.log('skewness: ', skewness(mat));
  console.log('kurtosis: ', kurtosis(mat));
})();