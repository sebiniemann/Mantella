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
    sudo apt-get install -y apt-transport-https ca-certificates
    
    sudo apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D
    echo 'deb https://apt.dockerproject.org/repo ubuntu-trusty main' | sudo tee /etc/apt/sources.list.d/docker.list
    
    sudo apt-get update
    sudo apt-get install -y linux-image-extra-$(uname -r)
    sudo apt-get install -y docker-engine
    
    sudo groupadd docker
    sudo usermod -aG docker vagrant
    
    su vagrant
    cd /vagrant
    docker build -t ubuntu/mantella:latest .
    docker run -v /vagrant:/mantella -w /mantella --name mantella -t -d ubuntu/mantella
  SHELL
end
