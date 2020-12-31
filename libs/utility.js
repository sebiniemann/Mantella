function typeOf(value) {
  const type = typeof value;

  switch (type) {
    case 'object':
      if (value === null) {
        return 'null';
      }

      if (Array.isArray(value)) {
        return 'array';
      }

      if (value instanceof Date) {
        return 'date';
      }

      return 'object';
    default:
      return type;
  }
}

function unique(array) {
  return [...new Set(array)];
}

export {
  typeOf,
  unique,
};