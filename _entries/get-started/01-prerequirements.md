---
title: Prerequirements
category: get-started
---

- GCC 4.8.1, Clang 3.3 or any other C++11 feature complete compiler
- Armadillo C++ 4.600.0 (http://arma.sourceforge.net/download.html)

Installing Armadillo C++ 4.600.0 on Ubuntu (visit http://arma.sourceforge.net/download.html to find installation instructions for other operating systems):

{% highlight bash %}
sudo apt-get install libblas-dev liblapack-dev
wget -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-4.600.3.tar.gz
tar -xzf armadillo-4.600.3.tar.gz
cd armadillo-4.600.3
cmake .
make
sudo make install
{% endhighlight %}