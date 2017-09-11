#!/bin/sh

git pull &&
qmake-qt5 &&
make &&
exec ./Shower
