# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure(2) do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://atlas.hashicorp.com/search.
  config.vm.box = "ubuntu/trusty64"

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  # config.vm.synced_folder "../data", "/vagrant_data"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true
  #
     # Customize the amount of memory on the VM:
     vb.memory = "1024"
  end
  #
  # View the documentation for the provider you are using for more
  # information on available options.

  # Define a Vagrant Push strategy for pushing to Atlas. Other push strategies
  # such as FTP and Heroku are also available. See the documentation at
  # https://docs.vagrantup.com/v2/push/atlas.html for more information.
  # config.push.define "atlas" do |push|
  #   push.app = "YOUR_ATLAS_USERNAME/YOUR_APPLICATION_NAME"
  # end

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update
    
    sudo apt-get install -qq htop
    sudo apt-get install -qq build-essential
    sudo apt-get install -qq git
    
    sudo apt-get install -qq clang
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/clang 90
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/clang++ 90
    
    sudo apt-get install -qq gdb
    sudo apt-get install -qq lcov
  
    sudo apt-get install -qq cmake
  
    sudo apt-get install -qq libblas-dev
    sudo apt-get install -qq liblapack-dev
    
    wget --quiet -O openblas.tar.gz http://github.com/xianyi/OpenBLAS/archive/v0.2.14.tar.gz
    mkdir openblas
    tar -xzf openblas.tar.gz -C ./openblas --strip-components=1
    cd openblas
    make
    sudo make install PREFIX=/usr/local
    cd ..
    rm -Rf openblas openblas.tar.gz
    
    wget --quiet -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-5.100.2.tar.gz
    mkdir armadillo
    tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
    cd armadillo
    cmake .
    make
    sudo make install
    cd ..
    rm -Rf armadillo armadillo.tar.gz
    
    wget --quiet -O catch.tar.gz https://github.com/philsquared/Catch/archive/3e0c5018123e7e32b3c35f4169cbdca4bcb8f332.tar.gz
    mkdir catch
    tar -xzf catch.tar.gz -C ./catch --strip-components=1
    sudo cp -R catch/single_include/catch.hpp /usr/local/include/
    rm -Rf catch catch.tar.gz
    
    sudo apt-get install -qq libmpich2-dev
    
    wget --quiet -O redis.tar.gz http://download.redis.io/releases/redis-3.0.3.tar.gz
    tar -xzf redis.tar.gz
    cd redis
    make
  SHELL
end
