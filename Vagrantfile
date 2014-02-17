# -*- mode: ruby -*-
# vi: set ft=ruby :


$provision_script = <<SCRIPT
apt-get update -yqq
/Marlin/setup-ubuntu.sh
echo "ARDUINO_INSTALL_DIR=/usr/share/arduino" >> /etc/environment
echo "AVR_TOOLS_PATH=/usr/bin/" >> /etc/environment
SCRIPT

Vagrant::Config.run do |config|
  config.vm.box = "precise64"
end

Vagrant.configure("2") do |config|
  # change the name of the synced_folder mount, so that it matches repo name
  config.vm.synced_folder ".", "/vagrant", :disabled => true
  config.vm.synced_folder ".", "/Marlin"

  config.vm.provision "shell", inline: $provision_script
end
