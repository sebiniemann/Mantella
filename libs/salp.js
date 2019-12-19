import * as libUtility from './utility.js';

class Mat {
  constructor() {
    this._raw = [];

    switch (arguments.length) {
      case 0:
        // new Mat()
        this._constructorEmpty();
        break;
    }
  }

  _constructorEmpty() {
    this.resize(0, 0);
  }

  _subscriptToIndex(rowSubscript, colSubscript) {
    return  colSubscript * this.nRows + rowSubscript;
  }

  at() {
    switch (arguments.length) {
      case 1:
        switch(libUtility.typeOf(arguments[0])) {
          case 'number':
            // at(index)
            return this._atIndex(...arguments);
          case 'array':
            // at([firstIndex, secondIndex, ...])
            return this._atIndices(...arguments);
          case 'function':
            // at(callback)
            return this._atCallback(...arguments);
        }
      case 2:
        switch(libUtility.typeOf(arguments[0])) {
          case 'number':
            switch(libUtility.typeOf(arguments[1])) {
              case 'number':
                // at(colSubscript, rowSubscript)
                return this._atSubscript(...arguments);
              case 'array':
                // at(colSubscript, [firstRowSubscript, secondRowSubscript, ...])
                return this._atSubscripts([arguments[0]], arguments[1]);
              case 'function':
                // at(index, callback)
                return this._atIndexCallback(...arguments);
            }
          case 'array':
            switch(libUtility.typeOf(arguments[1])) {
              case 'number':
                // at([firstColSubscript, secondColSubscript, ...], rowSubscript)
                return this._atSubscripts(arguments[0], [arguments[1]]);
              case 'array':
                // at([firstColSubscript, secondColSubscript, ...], [firstRowSubscript, secondRowSubscript, ...])
                return this._atSubscripts(...arguments);
              case 'function':
                // at([firstIndex, secondIndex, ...], callback)
                return this._atIndicesCallback(...arguments);
            }
        }
      case 3:
        switch(libUtility.typeOf(arguments[0])) {
          case 'number':
            switch(libUtility.typeOf(arguments[1])) {
              case 'number':
                // at(colSubscript, rowSubscript, callback)
                return this._atSubscriptCallback(...arguments);
              case 'array':
                // at(colSubscript, [firstRowSubscript, secondRowSubscript, ...], callback)
                return this._atSubscriptsCallback([arguments[0]], arguments[1], arguments[2]);
            }
          case 'array':
            switch(libUtility.typeOf(arguments[1])) {
              case 'number':
                // at([firstColSubscript, secondColSubscript, ...], rowSubscript, callback)
                return this._atSubscriptsCallback(arguments[0], [arguments[1]], arguments[2]);
              case 'array':
                // at([firstColSubscript, secondColSubscript, ...], [firstRowSubscript, secondRowSubscript, ...], callback)
                return this._atSubscriptsCallback(...arguments);
            }
        }
    }
  }

  _atCallback(callback) {
    for (let index = 0; index < this.nElems; ++index) {
      this._atIndexCallback(index, callback);
    }

    return this;
  }

  _atIndex(index) {
    return this._raw[index];
  }

  _atIndexCallback(index, callback) {
    this._raw[index] = callback(this._raw[index], index, this);

    return this;
  }

  _atIndices(indices) {
    const col = new Mat(indices.length, 1);

    let subIndex = 0;
    for (const index of indices) {
      col._atIndexCallback(subIndex, () => this._atIndex(index));

      ++subIndex;
    }

    return col;
  }

  _atIndicesCallback(indices, callback) {
    for (const index of indices) {
      this._atIndexCallback(index, callback);
    }

    return this;
  }

  _atSubscript(rowSubscript, colSubscript) {
    const index = this._subscriptToIndex(rowSubscript, colSubscript);

    return this._atIndex(index);
  }

  _atSubscriptCallback(rowSubscript, colSubscript, callback) {
    const index = this._subscriptToIndex(rowSubscript, colSubscript);

    this._atIndexCallback(index, callback);

    return this;
  }

  _atSubscripts(rowSubscripts, colSubscripts) {
    const mat = new Mat(rowSubscripts.length, colSubscripts.length);

    let subIndex = 0;
    for (const colSubscript of colSubscripts) {
      for (const rowSubscript of rowSubscripts) {
        mat._atIndexCallback(subIndex, () => this._atSubscript(rowSubscript, colSubscript));
        
        ++subIndex;
      }
    }
    
    return mat;
  }

  _atSubscriptsCallback(rowSubscripts, colSubscripts, callback) {
    for (const colSubscript of colSubscripts) {
      for (const rowSubscript of rowSubscripts) {
        this._atSubscriptCallback(rowSubscript, colSubscript, callback);
      }
    }

    return this;
  }

  resize(nCols, nRows) {
    this.nCols = nCols;
    this.nRows = nRows;
    this.nElems = this.nRows * this.nCols;

    this._raw.length = this.nElems;

    return this;
  }

  toString(precision = 5) {
    let output = '';

    for (let rowSubscript = 0; rowSubscript < this.nRows; ++rowSubscript) {
      output += (rowSubscript === 0 ? ' [[ ' : '  [ ');

      for (let colSubscript = 0; colSubscript < this.nCols; ++colSubscript) {
        output += this._atSubscript(rowSubscript, colSubscript)
          .toPrecision(precision)
          .replace(/\.0+$/, '')
          .padStart(precision + 5, ' ');
        output += ', ';
      }

      output += (rowSubscript === this.nRows - 1 ? ']]' : '],\n');
    }

    return output;
  }
}

export {
  Mat,
};
