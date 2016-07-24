Contributing to Mantella
=======================

This guide will accompany you on your first contribution to Mantella, covering everything from setting up a development system to committing your changes.

In case you only want to submit a pull request, go directly to **Filing a pull request** (at the end of the document).

**If you are in a hurry**, feel free to skip any part at your own account, but be aware that this might delay accepting your pull request (as someone needs to invest the time to finish it up :wink:), especially if the implementation is not yet working.

Let us know what you are planning
---------------------------------

If you found a bug in the source code, a mistake in any kind of documentation or you are missing some features within Mantella and want to implement/fix it yourself, please let us know in advance, so we can discuss the implementation or handle the bug beforehand. This way, we can also give you early support and advices.

To do this, simply add an issue to the [GitHub issue tracker](https://github.com/SebastianNiemann/Mantella/issues), describing what you plan to do and make clear whether you are already working on it or not. You can also contact us directly on [Gitter](https://gitter.im/SebastianNiemann/Mantella) at any point of your contribution.

In case you found a bug and want to fix it, please give us informations on what got wrong and how to reproduce it.

Forking Mantella
----------------

### Contributors

Your first step will be to fork Mantella on GitHub, for example by hitting `Fork` on the GitHub website and download your fork onto your local machine using the [Git tool](https://git-scm.com/downloads) of your choice.

### Team members

If you are a long-time contributor to Mantella and got write permissions for the repository, you don't need to fork Mantella to your account, but can directly add a new branch to Mantella 

``` bash
git checkout master
git pull
git checkout -b YourUsername/YourAwesomeBranch
```

Setting up a development system
-------------------------------

**The recommended and most easiest way** to set up a Mantella development system (including all dependencies), is to install 
[Docker](https://www.docker.com) and run:

```
git clone http://github.com/SebastianNiemann/Mantella.git
cd Mantella
sudo docker build -t ubuntu/mantella:latest .
sudo docker run -v .:/mantella -w /mantella --name mantella -t -d ubuntu/mantella
```

**Done!**

This will install a virtual machine and set up all required dependencies, together with other useful tools for testing or debugging. You can then connect to the Docker machine using:

```
sudo docker exec -i -t mantella /bin/bash
```

In case you have problems installing Docker on your operation system (typically on non-Linux machines), you can also install [Vagrant](https://www.vagrantup.com) after cloning Mantella and run:

```
vagrant up
```

to install a virtual machine including our Docker machine.

You can then connect to the Vagrant machine using your SSH client of choice (using `localhost:2222` as host:port and `vagrant` as username as well as password) and access the Docker machine inside Vagrant using `sudo docker exec -i -t mantella /bin/bash` as before.

Coding guidelines
-----------------

**Most important: Read some existing code, to get a better understanding of our code style :wink:**

Section `Testing your work` covers tools and commands to automatically check some of the following guidelines.

### Adding new headers and sources

When adding new files, put your sources under `src/`, headers under `include/mantella_bits/` and tests under `test/`.

To include them into the library file and test binary, add your sources and tests to `CMakeLists.txt` and your headers into `include/mantella`.

By convention, we place all files directly into `src/`, `include/mantella_bits/` or `test/` and only add sub folders for base classes (like `OptimisationProblem`), placing derivatives of such classes into these sub folders.

### Our code style and conventions

We usually follow the [Google style guide](https://code.google.com/p/google-styleguide/), with some notable exemptions/additions:

- **Never** break your code just because you reached some number of characters. Wrapping long code lines or comments should be done by your IDE, without changing the code.
- Avoid abbreviating variables and always write out their (meaning) full name.
- Use `#pragma once` instead of `#ifndef ...` as include guards.
- By conventions, we use `arma::uword` for all integer types (expects MPI forces us to use `int`) and `double` for all floating-point types.

### Writing tests

We use [Catch's BDD-style](https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md) to write our tests, whereby `SCENARIO` is used to identify the (member) function to be tested and `GIVEN` to differentiate between multiple overloads of the same function.

The actual test cases are then organised by `WHEN` and `THEN` blocks, **covering positive tests, as well as exception tests**.

``` cpp
// Catch
#include <catch.hpp>
#include "catchHelpers.hpp"

SCENARIO("myNewFunction", "[nameOfFile][myNewFunction]") {
  GIVEN("A number of elements and a precision value") {
    WHEN("The number of elements is 0") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(myNewFunction(0, 1e-12), std::domain_error);
      }
    }
    
    WHEN("The precision value is infinite") {
      THEN("Throw a domain error") {
        CHECK_THROWS_AS(myNewFunction(1, std::numeric_limits<double>::NaN()), std::domain_error);
        CHECK_THROWS_AS(myNewFunction(1, std::numeric_limits<double>::infinity()), std::domain_error);
      }
    }
  }
  
  GIVEN("A number of elements") {
    WHEN("The number of elements greater than 0") {
      THEN("Return 42") {
        CHECK(myNewFunction(100, 1e-3) == 42);
      }
    }
  }
}



SCENARIO("myNextNewFunction", "[nameOfFile][myNextNewFunction]") {
  GIVEN("A matrix") {
    WHEN("The matrix is empty") {
      THEN("Throw an invalid argument") {
        CHECK_THROWS_AS(myNextNewFunction(arma::mat()), std::invalid_argument);
      }
    }
  }
}
```

You might also want to take a look into `catchHelper.hpp`, as it adds some functions to ease testing more complex cases.

### Adding inline comments

Describe complex parts of your code as well as [magic numbers](https://en.wikipedia.org/wiki/Magic_number_(programming)) as specific as possible and avoid vague descriptions.

**If you had to think about how to implement something or needed to learn it on [StackOverflow](http://stackoverflow.com/) (or any other site) about how to do it, it is a complex part.**

However, if you find yourself documenting code that is hard to read or understand, you should consider to rewrite such tricky or unclear code first. In general, you should strive for readable, maintainable code, by using well-known patterns and simple structures.

Testing your work
-----------------

### Being in sync with the code style

You can check most code style rules and include-what-you-use (IWYU) rules by running the `./.code.sh --all` script at Mantella's root directory. Use `./.code.sh --help` to get more information on additional options.

### Running tests

``` bash
mkdir build
cd build
rm -Rf CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile # Cleans up previous builds
cmake -DBUILD_TESTS=ON ..
make -j 4
```

To execute your test, run

``` bash
../bin/mantellaTests
```

Executing the tests can also be further configured by using [Catch's commandline options](https://github.com/philsquared/Catch/blob/master/docs/command-line.md). For example, running just a subset of tests

``` bash
../bin/mantellaTest [myNewFunction]
```

### Measuring the code coverage

Assuming you are still in the `build/` directory and already run all tests, you can measure the code coverage by running [lcov](http://ltp.sourceforge.net/coverage/lcov.php).

```
cd ./CMakeFiles/mantella.dir/src/
lcov --directory . --base-directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
lcov --remove coverage.info '*.hpp' --output-file coverage.info
```

You can than either print out an overview on the code coverage of each file ...

```
lcov --list coverage.info
```

... or get details about the coverage within a file (in addition to the information above), by accessing the generated HTML code with your web browser.

```
genhtml coverage.info
```

**Be aware that reaching a 100% code coverage will most likely not result in a 100% functional coverage.** However, a less than 100% code coverage always guarantees a less than 100% functional coverage. 

Filing a pull request
---------------------

Pull request messages should then be organised as followed:

``` text
<tag>: <subject>

<body>

<licence statement>
```

### Tag
Add one of the following tags, that describe your changes the best.

- `api break`: Your change will break the existing API in any way.
- `feature`: You added a new feature, without breaking the existing API.
- `fix`: You fixed a bug.
- `test`: You changed or added a test.
- `documentation`: You extended or corrected the documentation.
- `maintenance`: For anything else.

### Subject
Add a description about the content of your commit.

### Body (optional)
In case the subject does not fully covers your changes, fell free to describe the details of your changes as specific as as you can/want. Do not be afraid to add multiple lines.

### Licence statement
End the commit message of your pull request adding a clear statement under which your licence your contribution is licensed.

Note that we cannot accept contributions that are either missing a licence statement or have chosen a licence incompatible to the MIT licence.

``` text
My contribution is licensed under the MIT license.
```