FROM ubuntu:16.04

MAINTAINER Sebastian Niemann <sebiniemann@gmail.com>

RUN apt-get update

# Installs compiler
# - Clang (set as default)
RUN apt-get install -y clang-3.8 && \
    update-alternatives --remove clang++ /usr/bin/clang++ && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set clang++ /usr/bin/clang++-3.8 && \
    update-alternatives --remove c++ /usr/bin/c++ && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set c++ /usr/bin/clang++-3.8

# Installs benchmark library
# - CMake
# - Google micro benchmark
RUN apt-get install -y cmake
RUN apt-get install -y wget && \
    wget -O benchmark.tar.gz https://github.com/google/benchmark/archive/master.tar.gz && \
    mkdir benchmark && \
    tar -xzf benchmark.tar.gz -C ./benchmark --strip-components=1 && \
    cd benchmark  && \
    cmake -DCMAKE_BUILD_TYPE=Release . && \
    make benchmark && \
    make install && \
    cd .. && \
    rm -Rf benchmark.tar.gz benchmark/ && \
    apt-get remove -y --purge wget && \
    apt-get autoremove -y --purge
