#!/bin/sh

cd ..
version=$(git rev-parse HEAD | sed -e 's/[0-9a-f]\{2\}/\\x&/g' -e 's/^.*$/"&"/')
m4 -DVERSION=$version version.c.in > version.c
