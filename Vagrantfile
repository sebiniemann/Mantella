# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/wily64"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = 1536
  end
  
  config.vm.network "forwarded_port", guest: 4000, host: 4000

  config.vm.provision "shell", inline: <<-SHELL
    sudo apt-get update -qq
    sudo apt-get upgrade -qq
    
    sudo apt-get install -qq ruby-dev
    sudo apt-get install -qq zlib1g-dev
    sudo apt-get install -qq nodejs
    sudo apt-get install libcurl3
    
    sudo gem install bundler
    
    echo "NOKOGIRI_USE_SYSTEM_LIBRARIES=true" | sudo tee --append /etc/environment > /dev/null
    source /etc/environment
    
    cd /vagrant
    bundle install
    bundle exec jekyll serve&
    
    sudo apt-get install htop
  SHELL
end
