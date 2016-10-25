[![Mantella](http://mantella.info/assets/images/logo_with_name.png)](http://mantella.info/)

Version 0.61.0
==============

[![Join the chat at https://gitter.im/Mantella/Mantella](https://badges.gitter.im/Mantella/Mantella.svg)](https://gitter.im/Mantella/Mantella) [![Linux and Mac build status](https://travis-ci.org/Mantella/Mantella.png?branch=master)](https://travis-ci.org/Mantella/Mantella) [![Windows build status](https://ci.appveyor.com/api/projects/status/2haeycl3xl1n66ai?svg=true)](https://ci.appveyor.com/project/SebastianNiemann/mantella) [![Static code anylsis status](https://scan.coverity.com/projects/3285/badge.svg)](https://scan.coverity.com/projects/3285) 

[Mantella](http://mantella.info/) is an efficiency-focused C++14 header-only library for analysing and solving optimisation problems. As it only depends on the C++ standard library, Linux, Windows and Mac OS X support is a given.

**Our goal for version 1.0 is to combine a high-performance, self-accelerating optimisation experience with an easy to use API.**

``` cpp
#include <mantella>

int main() {
  // ... company code.

  // Adds your own optimisation problem.
  mant::problem<double, 2> my_problem;
  my_problem.objective_function = [](const std::array<double, 2>& parameter) {
    return /* Your objective value calculation */;
  };
  
  // Optimises your problem. Selects and tunes the optimiser automatically at each invocation.
  mant::optimise_result<double, 2> result = mant::optimise(my_problem);
  
  // Print best parameters and objective value
  for(const auto element : res.best_parameter){
    std::cout << element << " ";
  }
  std::cout << " -> " << res.best_objective_value << std::endl;

  // ... continuing company code.
  
  return 0;
}
```

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
