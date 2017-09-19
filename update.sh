#!/bin/sh

git pull &&
qmake-qt5 &&
make &&
sudo chmod u+s Shower

exec ./Shower
