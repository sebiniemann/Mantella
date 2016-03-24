# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"

  config.vm.provider "virtualbox" do |vb|
    # Increases the memory as some weird internal compiler errors occur, if the compiler runs out-of-memory.
    vb.memory = 1536
  end

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update

    export CC=clang
    export CXX=clang++
    
    # Mantella's dependencies and Testing
    source /vagrant/.setup.sh
    
    sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang-3.8 100
    sudo update-alternatives --set cc /usr/bin/clang-3.8
    sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.8 100
    sudo update-alternatives --set c++ /usr/bin/clang++-3.8

    # Benchmarking
    wget -O benchmark.tar.gz https://github.com/google/benchmark/archive/master.tar.gz
    mkdir benchmark
    tar -xzf benchmark.tar.gz -C ./benchmark --strip-components=1
    cd benchmark || exit
    cmake -DCMAKE_BUILD_TYPE=Release .
    make benchmark
    sudo make install
    cd ..
    rm -Rf benchmark benchmark.tar.xz

    # Debugging
    sudo apt-get install -y valgrind

    # Other useful tools
    sudo apt-get install -y git
    sudo apt-get install -y htop
    sudo apt-get install -y unzip
    sudo apt-get install -y dos2unix
  SHELL
end
