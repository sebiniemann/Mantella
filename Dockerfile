FROM ubuntu:16.04

# Used to differentiate between continuous integration server and developer builds.
ARG CI
ENV CI ${CI:-false}

RUN apt-get update

# Installs compilers
# - GCC
# - Clang (set as default)
RUN apt-get install -y g++-5 && \
    update-alternatives --remove g++ /usr/bin/g++ && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 100 && \
    update-alternatives --set g++ /usr/bin/g++-5
RUN apt-get install -y clang-3.8 && \
    update-alternatives --remove clang++ /usr/bin/clang++ && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set clang++ /usr/bin/clang++-3.8 && \
    update-alternatives --remove c++ /usr/bin/c++ && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100 && \
    update-alternatives --set c++ /usr/bin/clang++-3.8

# Installs test dependencies
# - CMake
# - Catch (unit tests)
RUN apt-get install -y cmake
RUN apt-get install -y wget && \
    wget -O catch.tar.gz https://github.com/philsquared/Catch/archive/V1.5.0.tar.gz && \
    tar -xzf catch.tar.gz -C /usr/include --strip-components=2 Catch-1.5.0/single_include/catch.hpp && \
    rm -Rf catch.tar.gz catch/ && \
    apt-get remove -y --purge wget && \
    apt-get autoremove -y --purge

# Installs documentation dependencies
# - Python3 (documentation processing)
# - Sphinx (documentation generation)
# - Tinkerer (blog generation)
# - Octave (image generation)
# - ffmpeg (animation generation)
RUN apt-get install -y python3
RUN apt-get install -y python3-pip && \
    pip3 install --upgrade pip && \
    pip3 install Sphinx
RUN apt-get install -y liboctave-dev
RUN apt-get install -y ffmpeg && \
    apt-get install -y libavcodec-extra

# Installs benchmark dependencies (excluded on CI servers)
# - Docker
# - Octave (already installed as documentation dependency)
RUN if ! "$CI" -eq "true"; then \
      apt-get install -y apt-transport-https ca-certificates && \
      apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D && \
      echo 'deb https://apt.dockerproject.org/repo ubuntu-xenial main' | tee /etc/apt/sources.list.d/docker.list && \
      apt-get update && \
      apt-get install -y docker-engine \
    ; fi
