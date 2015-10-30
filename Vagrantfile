# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = 1024
  end

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update -qq
    
    # Using Clang
    sudo apt-get install -qq clang
    sudo update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100
    sudo update-alternatives --set cc /usr/bin/clang
    sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100
    sudo update-alternatives --set c++ /usr/bin/clang++
    
  
    # Prerequirements (including optional features)
    ## CMake
    sudo apt-get install -qq cmake
    
    ## Armadillo C++
    sudo apt-get install -qq libblas-dev liblapack-dev libopenblas-dev
    wget --quiet -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-5.600.2.tar.gz
    mkdir armadillo
    tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
    cd armadillo
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .
    make --quiet
    sudo make --quiet install
    ### Fixes issues with IWYU (suggesting for example <armadillo_bits/Base_bones.hpp> instead of <armadillo>)
    sudo find /usr/local/include/armadillo_bits -name *.hpp -exec sed -i -e '1i\/\/ IWYU pragma\: private\, include \<armadillo\>' {} ';'
    cd ..
    rm -Rf armadillo armadillo.tar.gz
    
    ## MPI (This will actually install 3.x on Ubuntu 14.04+ and 2.x on previous versions)
    sudo apt-get install -qq libmpich2-dev
    
    ## Redis database
    wget --quiet -O redis.tar.gz http://download.redis.io/releases/redis-3.0.3.tar.gz
    mkdir redis
    tar -xzf redis.tar.gz -C ./redis --strip-components=1
    cd redis
    make --quiet
    sudo make --quiet install
    cd ..
    rm -Rf redis.tar.gz
    
    
    # Testing
    sudo apt-get install -qq catch
    ## Using clang-format-3.6
    sudo add-apt-repository 'deb http://llvm.org/apt/trusty/ llvm-toolchain-trusty-3.6 main'
    wget -O - http://llvm.org/apt/llvm-snapshot.gpg.key|sudo apt-key add -
    sudo apt-get update -qq
    sudo apt-get install -qq clang-format-3.6
    ## Adds clang-format as an alternative to clang-format-3.6
    sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-3.6 100
    sudo update-alternatives --set clang-format /usr/bin/clang-format-3.6
    sudo apt-get install -qq iwyu
    sudo apt-get install -qq valgrind
    sudo apt-get install -qq lcov
    
    
    # Useful development tools
    sudo apt-get install -qq htop
    sudo apt-get install -qq git
    sudo apt-get install -qq ccache
    sudo apt-get install -qq gdb
    sudo apt-get install -qq dos2unix
    
    
    # Fixing some paths
    echo "LD_LIBRARY_PATH=/usr/local/lib" | sudo tee --append /etc/environment > /dev/null
    source /etc/environment
  SHELL
end
