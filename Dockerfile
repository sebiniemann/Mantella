FROM ubuntu:16.04

MAINTAINER Sebastian Niemann <sebiniemann@gmail.com>

# Used to differentiate between continuous integration server and developer builds.
ARG CI
ENV CI ${CI:-false}

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

# Installs user dependencies
# - CBLAS
# - LAPACKE
RUN apt-get install -y libblas-dev liblapacke-dev

# Installs optional user dependencies
# - MPI, including missing default include path
RUN apt-get install -y libmpich-dev
ENV CPATH "$CPATH:/usr/include/mpich/"

# Installs test dependencies
# - CMake
# - Catch (unit tests)
# - Clang-format (formatting test)
# - Clang-tidy (static code analyser)
RUN apt-get install -y cmake
RUN apt-get install -y wget && \
    wget -O catch.tar.gz https://github.com/philsquared/Catch/archive/V1.5.0.tar.gz && \
    mkdir catch && \
    tar -xzf catch.tar.gz -C ./catch --strip-components=1 && \
    cp catch/single_include/catch.hpp /usr/include/ && \
    rm -Rf catch.tar.gz catch/ && \
    apt-get remove -y --purge wget && \
    apt-get autoremove -y --purge
RUN apt-get install -y clang-format-3.8 && \
    update-alternatives --remove clang-format /usr/bin/clang-format && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-3.8 100 && \
    update-alternatives --set clang-format /usr/bin/clang-format-3.8
RUN apt-get install -y clang-tidy-3.8 && \
    update-alternatives --remove clang-tidy /usr/bin/clang-tidy && \
    update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-3.8 100 && \
    update-alternatives --set clang-tidy /usr/bin/clang-tidy-3.8

# Installs documentation dependencies
# - Sphinx, with read-the-document theme (documentation generation)
# - Octave (image generation)
RUN apt-get install -y python-pip && \
    pip install --upgrade pip && \
    pip install Sphinx sphinx_rtd_theme && \
    apt-get remove -y --purge python-pip && \
    apt-get autoremove -y --purge
RUN apt-get install -y liboctave-dev

# Installs helpful development tools (skipped on CI servers)
# - Google micro benchmark
RUN if [ "$CI" == 'true' ]; then \
      apt-get install -y wget && \
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
      apt-get autoremove -y --purge \
    ; fi

# Adds local library path
ENV LD_LIBRARY_PATH "$LD_LIBRARY_PATH:/usr/local/lib"
