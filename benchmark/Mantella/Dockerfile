FROM ubuntu:16.04

MAINTAINER Sebastian Niemann <sebiniemann@gmail.com>

RUN apt-get update

# Installs compilers
# - Clang (set as default)
RUN apt-get install -y clang-3.8 && \
    update-alternatives --remove clang++ /usr/bin/clang++ && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set clang++ /usr/bin/clang++-3.8 && \
    update-alternatives --remove c++ /usr/bin/c++ && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set c++ /usr/bin/clang++-3.8

# Installs benchmark dependencies
# - CMake
RUN apt-get install -y cmake