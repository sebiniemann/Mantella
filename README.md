[![Mantella](http://sebastianniemann.github.io/Mantella/assets/images/logo_with_name.png)](http://mantella.info/)

Version 0.50.2
==============

[![Travis CI Build Status](https://travis-ci.org/Mantella/Mantella.png?branch=master)](https://travis-ci.org/Mantella/Mantella) [![Coverity Scan Build Status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) [![Join the chat at https://gitter.im/Mantella/Mantella](https://badges.gitter.im/Mantella/Mantella.svg)](https://gitter.im/Mantella/Mantella)

[Mantella](http://mantella.info/) is an efficiency-focused C++ header-only library for analysing and solving optimisation problems.

It is written in C++14 and only depends on the C++ standard library, supporting Linux, Windows and Mac OS X.

Our goal is to make an optimisation process as easy as writing

``` cpp
#include <mantella>

int main() {
  // ... company code.

  // Add your own optimisation problem.
  constexpr std::size_t number_of_dimensions = 2;
  mant::optimisation_problem<double, number_of_dimensions> my_problem;
  my_problem.objective_functions.push_back({
    [](const auto parameter) {
      return my_objective_value_computaton(parameter);
    }
    "My problem"
  });
  
  // Optimise the problem, selecting and parametrising an appropriate algorithm automatically.
  auto result = mant::optimise(my_problem);

  // ... continuing company code.
  
  return 0;
}
```

while (automatically) analysing and learning the problems properties over time, in order to select the most efficient solver to do the job.

Getting started
---------------
Please see our [official website](http://mantella.info/getting-started/), to get started with Mantella.

You will also find **detailed API information** under http://mantella.info/overview/.

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

License
-------
Distributed under [MIT license](http://opensource.org/licenses/MIT).

Copyright (c) 2013-2016 [Sebastian Niemann](mailto:niemann@sra.uni-hannover.de) and contributors.
