# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/wily64"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = 1536
  end

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update -qq
    sudo apt-get upgrade -qq
    
    # Clang
    sudo apt-get install -qq clang
    
    # Prerequirements (including optional features)
    ## CMake
    sudo apt-get install -qq cmake
    
    ## Armadillo C++
    sudo apt-get install -qq libblas-dev liblapack-dev libopenblas-dev
    wget --quiet -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-6.200.4.tar.gz
    mkdir armadillo
    tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
    cd armadillo
    cmake .
    make --quiet
    sudo make --quiet install
    ### Fixes issues with IWYU (suggesting for example <armadillo_bits/Base_bones.hpp> instead of <armadillo>)
    sudo find /usr/include/armadillo_bits -name *.hpp -exec sed -i -e '1i\/\/ IWYU pragma\: private\, include \<armadillo\>' {} ';'
    cd ..
    rm -Rf armadillo armadillo.tar.gz
    
    ## MPI
    sudo apt-get install -qq libmpich2-dev
    
    ## Redis database
    sudo apt-get install -qq redis-server
    sudo apt-get install -qq libhiredis-dev
    
    # Testing
    sudo apt-get install -qq catch
    ## Using clang-format-3.6
    sudo apt-get install -qq clang-format-3.6
    sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-3.6 100
    sudo update-alternatives --set clang-format /usr/bin/clang-format-3.6
    sudo apt-get install -qq iwyu
    sudo apt-get install -qq lcov
    
    # Debugging
    sudo apt-get install -qq gdb
    sudo apt-get install -qq valgrind
    
    # Other useful tools
    sudo apt-get install -qq htop
    sudo apt-get install -qq git
    sudo apt-get install -qq ccache
    sudo apt-get install -qq dos2unix
  SHELL
end
