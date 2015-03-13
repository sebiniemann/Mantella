- GCC 4.8.1, Clang 3.3 or any other C++11 feature complete compiler
- Armadillo C++ 4.650.0 ([project page](http://arma.sourceforge.net))

Installing Armadillo C++ on Ubuntu (visit the [Armadillo's download page](http://arma.sourceforge.net/download.html) to find installation instructions for other operating systems):

{% highlight bash %}
sudo apt-get install libblas-dev liblapack-dev
wget http://downloads.sourceforge.net/project/arma/armadillo-4.650.3.tar.gz
tar -xzf armadillo-4.650.3.tar.gz
cd armadillo-4.650.3
cmake .
make
sudo make install
{% endhighlight %}