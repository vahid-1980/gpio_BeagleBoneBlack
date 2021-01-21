#!/bin/bash

gpio_dir="/sys/class/gpio"

gpio_in="/sys/class/gpio/gpio$1"
gpio_out="/sys/class/gpio/gpio$2"

gpio_in_direction="${gpio_in}/direction"
gpio_out_direction="${gpio_out}/direction"

gpio_in_value="${gpio_in}/value"
gpio_out_value="${gpio_out}/value"


if [ ! -d $gpio_in ]; then 
    echo $1 > ${gpio_dir}/export
    if [ ! $? -eq 0 ]; then
        echo "something's wrong with gpio $1"
        exit -1
    else
        echo "in" > $gpio_in_direction
    fi
else
    echo "in" > $gpio_in_direction
fi


if [ ! -d $gpio_out ]; then 
    echo $2 > ${gpio_dir}/export
    if [ ! $? -eq 0 ]; then
        echo "something's wrong with gpio $2"
        exit -1
    else
        echo "out" > $gpio_in_direction
    fi
else
    echo "out" > $gpio_in_direction
fi

echo 0 > $gpio_out_value
