module.exports = {
  env: {
    browser: true,
    commonjs: true,
    es6: true,
    'jest/globals': true,
  },
  extends: [
    'eslint:recommended',
    'plugin:import/recommended',
    'plugin:jest/recommended',
  ],
  parserOptions: {
    ecmaVersion: 2019,
    sourceType: 'module',
  },
  plugins: [
    'import',
    'jest',
  ],
  rules: {
    // Custom rules should only be added to ease the development/review process by pointing out common/reiterated mistakes.

    //
    // ESLint core rules
    //
    // Best practice and errors
    'no-console': ['error', {
      allow: ['warn', 'error'],
    }],
    // Styling
    'brace-style': ['error', '1tbs', {
      allowSingleLine: false,
    }],
    'comma-dangle': ['error', 'always-multiline'],
    'indent': ['error', 2, {
      SwitchCase: 1,
    }],
    'linebreak-style': ['error', 'unix'],
    'object-curly-newline': ['error', 'always'],
    'quotes': ['error', 'single'],
    'semi': ['error', 'always'],

  },
};
