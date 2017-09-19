#!/bin/sh

git pull &&
qmake &&
make &&
sudo chmod u+s Shower

exec ./Shower
