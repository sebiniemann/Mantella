FROM ubuntu:16.04

MAINTAINER Sebastian Niemann <sebiniemann@gmail.com>

# Used to differentiate between continuous integration server and developer builds.
ARG CI
ENV CI ${CI:-false}
    
RUN apt-get update

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
    