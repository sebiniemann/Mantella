[![Mantella](http://mantella.info/assets/images/logo_with_name.png)](http://mantella.info/)

Version 0.70.0
==============

[![Join the chat at https://gitter.im/Mantella/Mantella](https://badges.gitter.im/Mantella/Mantella.svg)](https://gitter.im/Mantella/Mantella) [![Linux and Mac build status](https://travis-ci.org/Mantella/Mantella.png?branch=master)](https://travis-ci.org/Mantella/Mantella) [![Windows build status](https://ci.appveyor.com/api/projects/status/2haeycl3xl1n66ai/branch/master?svg=true)](https://ci.appveyor.com/project/SebastianNiemann/mantella) [![Static code anylsis status](https://scan.coverity.com/projects/10668/badge.svg)](https://scan.coverity.com/projects/10668)

[Mantella](http://mantella.info/) is an efficiency-focused C++14 header-only library for analysing and solving optimisation problems. As it only depends on the C++ standard library, Linux, Windows and Mac OS X support is a given.

**Our goal for version 1.0 is to combine a high-performance, self-accelerating optimisation experience with an easy to use API.**

**Installation**

Download Mantella and copy the include folder's content into the system's default search path for header files.
``` bash
wget -O mantella.tar.gz https://github.com/Mantella/Mantella/archive/master.tar.gz
tar -xzf mantella.tar.gz -C /usr/include --strip-components=2 --wildcards Mantella-master/include/mantella*
```

**Usage**
``` cpp
#include <mantella0>
#include <iostream> // Used for std::cout
#include <iterator> // Used for std::ostream_iterator

int main() {
  // ... Your company code.

  // Adds your own optimisation problem.
  mant::problem<double, 2> my_problem;
  my_problem.objective_function = [](const std::array<double, 2>& parameter) {
    // Or your own objective value calculation ;)
    return std::accumulate(parameter.cbegin(), parameter.cend(), 0.0);
  };

  // Optimises your problem. Selects and tunes the optimiser automatically at each invocation.
  const mant::optimise_result<double, 2>&& result = mant::optimise(my_problem);

  // Prints out the best parameter and its objective value
  std::copy(result.parameter.cbegin(), result.parameter.cend(), std::ostream_iterator<double>(std::cout, " "));
  std::cout << "-> " << result.objective_value << std::endl;

  // Continuing your company code ...

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
