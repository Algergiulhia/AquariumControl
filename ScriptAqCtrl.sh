#!/bin/bash

# Path to daemon executable
daemonEXE=/etc/Aquarium/daemonRasp

# Path to qt application executable
qtAppEXE=/etc/Aquarium/aquariumAPP

# i2c modules
modprobe i2c-dev
modprobe i2c-bcm2835

# one wire config
modprobe w1-gpio
modprobe w1-therm

# rtc config
modprobe rtc-ds1307
echo ds1307 0x68 >> /sys/class/i2c-adapter/i2c-1/new_device
hwclock -s

# drivers
insmod /etc/Aquarium/DDrivers/relay.ko
insmod /etc/Aquarium/DDrivers/pwm.ko

# mqueue
mkdir /dev/mqueue
mount -t mqueue none /dev/mqueue

# Executes daemon in background
exec $daemonEXE &

# Waits 0.5 sec to make sure everything is properly initialized
sleep 0.5

# Executes qt application
exec $qtAppEXE

