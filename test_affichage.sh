#!/bin/bash

cd Phase1
make

# ./readelf -h $1 > notre_programme.txt
#./readelf -S $1 > notre_programme.txt

# arm-none-eabi-readelf -h $1 > programme_fourni.txt
#arm-none-eabi-readelf -S $1 > programme_fourni.txt

#diff -s -q notre_programme.txt programme_fourni.txt

for num in $(seq 0 30)
do
    ./readelf -x $num $1 > notre_programme.txt
    arm-none-eabi-readelf -x $num $1 | cut -d " " -f 1-7 > programme_fourni.txt
    diff -w -b -B --suppress-common-lines notre_programme.txt programme_fourni.txt
done

cd ..
