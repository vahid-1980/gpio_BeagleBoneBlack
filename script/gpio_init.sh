#!/bin/bash
# usage: source gpio_init.sh <input gpio num> <output gpio num>
#it will export gpios and set their directions for those argumants, in sysfs.

if [ -d "/sys/class/gpio/gpio"$1 ]; then 
	echo $1 > /sys/class/gpio/unexport
	sleep 0.01
fi

if [ -d "/sys/class/gpio/gpio"$2 ]; then
	echo $2 > /sys/class/gpio/unexport
	sleep 0.01
fi

echo $1 > /sys/class/gpio/export
sleep 0.1

echo $2 > /sys/class/gpio/export
sleep 0.1

if [ -d "/sys/class/gpio/gpio"$1 ]; then
	echo "in" > /sys/class/gpio/gpio$1/direction
else
	echo "something's wrong with gpio $1"
	exit -1
fi

if [ -d "/sys/class/gpio/gpio"$2 ]; then
	echo "out" > /sys/class/gpio/gpio$2/direction && 
	echo "0" > /sys/class/gpio/gpio$2/value
else
	echo "something's wrong with gpio $2"
	exit -1
fi

