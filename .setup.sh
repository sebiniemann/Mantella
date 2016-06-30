#!/bin/bash
# Adds new sources to install a newer version of GCC and Clang
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo add-apt-repository 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.8 main'
wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key | sudo apt-key add -
sudo apt-get update

# GCC
sudo apt-get install -y g++-6
sudo update-alternatives --remove g++ /usr/bin/gcc
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 100
sudo update-alternatives --set gcc /usr/bin/gcc-6
sudo update-alternatives --remove g++ /usr/bin/g++
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 100
sudo update-alternatives --set g++ /usr/bin/g++-6
sudo update-alternatives --remove gcov /usr/bin/gcov
sudo update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-6 100
sudo update-alternatives --set gcov /usr/bin/gcov-6

# Clang
sudo apt-get install -y clang-3.8
sudo update-alternatives --remove clang /usr/bin/clang
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 100
sudo update-alternatives --set clang /usr/bin/clang-3.8
sudo update-alternatives --remove clang++ /usr/bin/clang++
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 100
sudo update-alternatives --set clang++ /usr/bin/clang++-3.8

# Messy Ubuntu 14.04 <-> recent GCC / Clang workarounds
## Adds missing openmp library
sudo cp /usr/lib/gcc/x86_64-linux-gnu/6/libgomp.so /usr/lib/libomp.so
if [ "$CXX" == "g++" ]; then
  export CPATH=$CPATH:/usr/lib/gcc/x86_64-linux-gnu/6/include:/usr/lib/gcc/x86_64-linux-gnu/6/include-fixed
else
  export CPATH=$CPATH:/usr/lib/llvm-3.8/lib/clang/3.8.1/include
  sudo cp /usr/lib/gcc/x86_64-linux-gnu/6.1.1/include/omp.h /usr/lib/clang/3.8.1/include/
fi

# CMake
wget -O cmake.tar.gz https://cmake.org/files/v3.5/cmake-3.5.2-Linux-x86_64.tar.gz
mkdir cmake
tar -xzf cmake.tar.gz -C ./cmake --strip-components=1
sudo cp -R cmake/* /usr/

# Armadillo C++
sudo apt-get install -y libblas-dev liblapack-dev libopenblas-dev
wget -O armadillo.tar.xz http://downloads.sourceforge.net/arma/armadillo-7.200.2.tar.xz
mkdir armadillo
tar -xJf armadillo.tar.xz -C ./armadillo --strip-components=1
cd armadillo || exit
cmake .
make -j 4
sudo make install
## Fixes issues with IWYU (suggesting for example <armadillo_bits/Base_bones.hpp> instead of <armadillo>)
sudo find /usr/include/armadillo_bits -name *.hpp -exec sed -i -e '1i\/\/ IWYU pragma\: private\, include \<armadillo\>' {} ';'
cd ..

# Optional features
## MPI
sudo apt-get install -y libmpich-dev
### Adds missing default include path
export CPATH=$CPATH:/usr/include/mpich

# Code checks
## Formatting
sudo apt-get install -y clang-format-3.8
sudo update-alternatives --remove clang-format /usr/bin/clang-format
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-3.8 100
sudo update-alternatives --set clang-format /usr/bin/clang-format-3.8
## Include rules
sudo apt-get install -y libncurses5-dev zlib1g-dev libclang-3.8-dev
wget -O iwyu.tar.gz http://include-what-you-use.org/downloads/include-what-you-use-0.6.src.tar.gz
mkdir iwyu
tar -xzf iwyu.tar.gz -C ./iwyu --strip-components=1
cd iwyu || exit
cmake -DIWYU_LLVM_ROOT_PATH=/usr/lib/llvm-3.8/ .
make -j 4
sudo make install
cd ..

## Unit tests
wget -O catch.tar.gz https://github.com/philsquared/Catch/archive/V1.5.0.tar.gz
mkdir catch
tar -xzf catch.tar.gz -C ./catch --strip-components=1
sudo cp catch/single_include/catch.hpp /usr/include
wget -O lcov.tar.gz http://downloads.sourceforge.net/ltp/lcov-1.12.tar.gz
mkdir lcov
tar -xzf lcov.tar.gz -C ./lcov --strip-components=1
cd lcov || exit
sudo make install
cd ..

# Keeps the path fixes persistent
echo CPATH="$CPATH" | sudo tee --append /etc/environment > /dev/null
source /etc/environment