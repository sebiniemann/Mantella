FROM ubuntu:16.04

MAINTAINER Sebastian Niemann <sebiniemann@gmail.com>

RUN apt-get update

# Installs compilers
# - GCC
# - Clang (set as default)
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
RUN apt-get install -y clang-3.8 && \
    update-alternatives --remove clang /usr/bin/clang && \
    update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 100 && \
    update-alternatives --set clang /usr/bin/clang-3.8 && \
    update-alternatives --remove clang++ /usr/bin/clang++ && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set clang++ /usr/bin/clang++-3.8 && \
    update-alternatives --remove cc /usr/bin/cc && \
    update-alternatives --install /usr/bin/cc cc /usr/bin/clang-3.8 100 && \
    update-alternatives --set cc /usr/bin/clang-3.8 && \
    update-alternatives --remove c++ /usr/bin/c++ && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set c++ /usr/bin/clang++-3.8

# Installs test dependencies
# - CMake
# - Catch (unit tests)
RUN apt-get install -y cmake
RUN apt-get install -y wget && \
    wget -O catch.tar.gz https://github.com/philsquared/Catch/archive/V1.5.0.tar.gz && \
    mkdir catch && \
    tar -xzf catch.tar.gz -C ./catch --strip-components=1 && \
    cp catch/single_include/catch.hpp /usr/include/ && \
    rm -Rf catch.tar.gz catch/ && \
    apt-get remove -y --purge wget && \
    apt-get autoremove -y --purge

# Installs documentation dependencies
# - Sphinx, with read-the-document theme (documentation generation)
# - Octave (image generation)
RUN apt-get install -y python-pip && \
    pip install --upgrade pip && \
    pip install Sphinx sphinx_rtd_theme && \
    apt-get remove -y --purge python-pip && \
    apt-get autoremove -y --purge
RUN apt-get install -y liboctave-dev
