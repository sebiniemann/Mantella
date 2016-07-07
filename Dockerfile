FROM ubuntu:16.04

MAINTAINER Sebastian Niemann <niemann@sra.uni-hannover.de>

RUN apt-get update

# GCC
RUN apt-get install -y g++-5 && \
    update-alternatives --remove g++ /usr/bin/gcc && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100 && \
    update-alternatives --set gcc /usr/bin/gcc-5 && \
    update-alternatives --remove g++ /usr/bin/g++ && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 100 && \
    update-alternatives --set g++ /usr/bin/g++-5 && \
    update-alternatives --remove gcov /usr/bin/gcov && \
    update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-5 100 && \
    update-alternatives --set gcov /usr/bin/gcov-5

# Clang
RUN apt-get install -y clang-3.8 && \
    update-alternatives --remove clang /usr/bin/clang && \
    update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 100 && \
    update-alternatives --set clang /usr/bin/clang-3.8 && \
    update-alternatives --remove clang++ /usr/bin/clang++ && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set clang++ /usr/bin/clang++-3.8
    
# CMake
RUN apt-get install -y cmake

# Armadillo C++
## Fixes issues with IWYU (suggesting for example <armadillo_bits/Base_bones.hpp> instead of <armadillo>)
RUN apt-get install -y wget xz-utils libblas-dev liblapack-dev libopenblas-dev && \
    wget -O armadillo.tar.xz http://downloads.sourceforge.net/arma/armadillo-7.200.2.tar.xz && \
    mkdir armadillo && \
    ls -al && \
    tar -xJf armadillo.tar.xz -C ./armadillo --strip-components=1 && \
    cd armadillo && \
    cmake . && \
    make -j 4 && \
    make install && \
    find /usr/include/armadillo_bits -name *.hpp -exec sed -i -e '1i\/\/ IWYU pragma\: private\, include \<armadillo\>' {} ';' && \
    cd .. && \
    rm -Rf armadillo.tar.xz armadillo/

# Optional features
## MPI
### Adds missing default include path
RUN apt-get install -y libmpich-dev

# Code checks
## Formatting
RUN apt-get install -y clang-format-3.8 && \
    update-alternatives --remove clang-format /usr/bin/clang-format && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-3.8 100 && \
    update-alternatives --set clang-format /usr/bin/clang-format-3.8
## Include rules
### Adds the correct 
RUN apt-get install -y iwyu && \
    export CPATH=$CPATH:/usr/lib/gcc/x86_64-linux-gnu/6/include:/usr/lib/gcc/x86_64-linux-gnu/6/include-fixed:/usr/lib/llvm-3.8/lib/clang/3.8.1/include

## Unit tests
RUN apt-get install -y catch