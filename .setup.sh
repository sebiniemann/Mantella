#!/bin/bash
sudo apt-get update

# Clang
sudo apt-get install -y clang

# Prerequirements (including optional features)
## CMake
sudo apt-get install -y cmake

## Armadillo C++
sudo apt-get install -y libblas-dev liblapack-dev libopenblas-dev
wget -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-6.200.4.tar.gz
mkdir armadillo
tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
cd armadillo
cmake .
make
sudo make install
### Fixes issues with IWYU (suggesting for example <armadillo_bits/Base_bones.hpp> instead of <armadillo>)
sudo find /usr/include/armadillo_bits -name *.hpp -exec sed -i -e '1i\/\/ IWYU pragma\: private\, include \<armadillo\>' {} ';'
cd ..
rm -Rf armadillo armadillo.tar.gz

## MPI
sudo apt-get install -y libmpich2-dev

## Redis database
sudo apt-get install -y redis-server
sudo apt-get install -y libhiredis-dev

# Testing
sudo apt-get install -y catch
## Using clang-format-3.6
sudo add-apt-repository 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.6 main'
wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
sudo apt-get update -qq
if [ -f /usr/local/clang-3.5.0/bin/clang-format ]; then
  sudo mv /usr/local/clang-3.5.0/bin/clang-format /usr/local/clang-3.5.0/bin/clang-format-3.5
fi
sudo apt-get install -y clang-format-3.6
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-3.6 100
sudo update-alternatives --set clang-format /usr/bin/clang-format-3.6
sudo apt-get install -y iwyu
sudo apt-get install -y lcov

# Travis CI is a continous integeration system and does not need debugger or other tools

if [[ $TRAVIS != "true" ]]; then
  # Debugging
  sudo apt-get install -y gdb
  sudo apt-get install -y valgrind

  # Other useful tools
  sudo apt-get install -y htop
  sudo apt-get install -y git
  sudo apt-get install -y ccache
  sudo apt-get install -y dos2unix 
fi
