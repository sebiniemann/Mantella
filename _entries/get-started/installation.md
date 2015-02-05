---
title: Installation
category: get-started
---

<div class="custom-callout custom-callout-warning" id="jquery-required">

### Prerequirements

#### Necessary dependencies
- GCC v4.8.1 (or any other C++11 feature complete compiler)
- [CMake v2.8.8](http://www.cmake.org/download/)
- [Armadillo C++ v4.600.0](http://arma.sourceforge.net/download.html)
- [Cereal v1.0.0](https://github.com/USCiLab/cereal/releases) (or its [development version](https://github.com/USCiLab/cereal/archive/develop.tar.gz), in case  cereal errors are thrown during the installation)

#### Optional dependencies (only needed for parallel/cluster algorithms)
- MPI v3.0.0 (we recommend the [MVAPICH](http://mvapich.cse.ohio-state.edu/) implementation)

</div>

{% highlight bash %}
cd path/to/project      # Goto the project directory
cmake .                 # Prepare the Makefiles
make                    # Compile the library
sudo make install       # Install Mantella into your local system
{% endhighlight %}