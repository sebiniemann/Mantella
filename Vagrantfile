# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  config.vm.box = "ubuntu/trusty64"

  config.vm.provider "virtualbox" do |vb|
    # Increases the memory, avoiding that the compiler runs out-of-memory.
    vb.memory = 1536
  end

  config.vm.provision "shell", inline: <<-SHELL
    # Installs Docker
    # See https://docs.docker.com/engine/installation/linux/ubuntulinux/ for more details
    sudo apt-get update
    sudo apt-get install -y apt-transport-https ca-certificates
    
    sudo apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D
    echo 'deb https://apt.dockerproject.org/repo ubuntu-trusty main' | sudo tee /etc/apt/sources.list.d/docker.list
    
    sudo apt-get update
    sudo apt-get install -y linux-image-extra-$(uname -r)
    sudo apt-get install -y docker-engine
    
    # Builds and runs Mantella's Docker image
    cd /vagrant
    sudo docker build -t ubuntu/mantella:latest .
    sudo docker run -v /vagrant:/mantella -w /mantella --name mantella -t -d ubuntu/mantella
    
    ## Adds useful applications to the running Docker container
    sudo docker exec mantella apt-get install -y vim
  SHELL
end
