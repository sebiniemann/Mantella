# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/vivid64"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = 1024
  end

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    
    sudo apt-get install -qq htop
    sudo apt-get install -qq git
    
    sudo apt-get install -qq gfortran
    sudo apt-get install -qq clang
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/clang 90
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/clang++ 90
    
    sudo apt-get install -qq clang-format-3.6
    sudo apt-get install -qq ccache
    sudo apt-get install -qq iwyu
    sudo apt-get install -qq gdb
    sudo apt-get install -qq lcov
  
    sudo apt-get install -qq cmake
    
    sudo apt-get install -qq libopenblas-dev
    
    # Builds Armadillo C++
    wget --quiet -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-5.100.2.tar.gz
    mkdir armadillo
    tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
    cd armadillo
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .
    make --quiet
    sudo make --quiet install
    # Fixes issues with IWYU (suggesting for example <armadillo_bits/Base_bones.hpp> instead of <armadillo>)
    sudo find /usr/local/include/armadillo_bits . -name "*.hpp" -exec sed -i -e '1i\/\/ IWYU pragma\: private\, include \<armadillo\>' {} \;
    cd ..
    rm -Rf armadillo armadillo.tar.gz
    
    sudo apt-get install -qq catch
    sudo apt-get install -qq libmpich2-dev
    
    # Builds Redis
    wget --quiet -O redis.tar.gz http://download.redis.io/releases/redis-3.0.3.tar.gz
    mkdir redis
    tar -xzf redis.tar.gz -C ./redis --strip-components=1
    cd redis
    make --quiet
    sudo make --quiet install
    cd ..
    rm -Rf redis.tar.gz
  SHELL
end
