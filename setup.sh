#!/bin/bash

sudo insmod driver/lcd_driver.ko
sudo insmod driver/buzzer.ko
sudo insmod driver/mpu_wheel.ko
sudo insmod driver/button_ctl.ko
