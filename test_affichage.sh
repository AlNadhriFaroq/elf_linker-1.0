#!/bin/bash

clang -Wall -Werror -o readElfHeader readElfHeader.c

./readElfHeader $1 > notre_programme.txt

arm-none-eabi-readelf -h $1 > programme_fourni.txt

diff -y notre_programme.txt programme_fourni.txt