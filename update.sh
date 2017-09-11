#!/bin/sh

git pull &&
qmake-qt5 &&
make &&
chmod u+s Shower

exec ./Shower
