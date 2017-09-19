#!/bin/sh

gpio export 25 output &&
gpio export 23 output &&
gpio export 24 output &&

gpio export 20 output &&
gpio export 21 output &&
gpio export 12 output &&
gpio export 16 output &&
gpio export 17 output &&
gpio export 27 output &&
gpio export 22 output &&

gpio export 5 input &&
gpio -g mode 5 down &&
gpio edge 5 both &&
gpio export 6 input &&
gpio -g mode 6 down &&
gpio edge 6 both
