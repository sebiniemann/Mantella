# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/wily64"

  config.vm.provider "virtualbox" do |vb|
    # Increases memory to avoid compiler errors, due to excessive memory usage
    vb.memory = 1536
  end
  
  # Forwards Jekyll default port to access the website on the host system via http://localhost:4000
  config.vm.network "forwarded_port", guest: 4000, host: 4000

  config.vm.provision "shell", inline: <<-SHELL
    # Updates the system
    sudo apt-get update -qq
    sudo apt-get upgrade -qq
    
    # Installs Jekyll (github-pages) dependencies
    sudo apt-get install -qq ruby-dev
    sudo apt-get install -qq zlib1g-dev
    sudo apt-get install -qq nodejs
    
    # Installs html-proofer dependencies
    sudo apt-get install -qq libcurl3
    
    # Install Bundler
    sudo gem install bundler
    
    # Tells Nokogiri to use already installed system libraries - instead of installing new ones - to speed up the installation process of html-proofer.
    echo "NOKOGIRI_USE_SYSTEM_LIBRARIES=true" | sudo tee --append /etc/environment > /dev/null
    source /etc/environment
    
    # Installs Jekyll (github-pages) and html-proofer
    cd /vagrant
    bundle install
    
    # Installs Mantella (including dependencies)
    ## Installs Clang
    sudo apt-get install -qq clang
    
    ## Installs CMake
    sudo apt-get install -qq cmake
    
    ## Fixing some paths
    echo "LD_LIBRARY_PATH=/usr/local/lib" | sudo tee --append /etc/environment > /dev/null
    source /etc/environment
    
    ## Installs Armadillo C++
    sudo apt-get install -qq libblas-dev liblapack-dev libopenblas-dev
    wget --quiet -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-6.200.4.tar.gz
    mkdir armadillo
    tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
    cd armadillo
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local .
    make --quiet
    sudo make --quiet install
    cd ..
    rm -Rf armadillo armadillo.tar.gz
    
    ## Installs Mantella
    git clone --depth 1 --branch master https://github.com/SebastianNiemann/Mantella.git
    cd Mantella
    cmake .
    make
    sudo make install
    cd ..
    rm -Rf Mantella
    
    # Installs useful development tools
    sudo apt-get install htop
  SHELL
end
