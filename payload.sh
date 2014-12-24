#!/bin/bash
set -e

echo 'nameserver 208.67.222.222' > /etc/resolv.conf
echo 'nameserver 208.67.220.220' >> /etc/resolv.conf

# install node.js
apt-get update -qq
apt-get install curl
curl -sL https://deb.nodesource.com/setup | bash -
apt-get install -y nodejs

npm install --arch=armhf

cd src/static
npm install
npm run bower
