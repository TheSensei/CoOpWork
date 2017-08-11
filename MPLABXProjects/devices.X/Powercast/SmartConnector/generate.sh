#!/bin/sh
version=$(git rev-parse HEAD)
./tag.py -v $version -i Smart\ Connector.X/dist/Production/production/Smart_Connector.X.production.hex -o $version.hex
cp Smart\ Connector.X/dist/Production/production/Smart_Connector.X.production.hex $version-flash.hex
echo $version
