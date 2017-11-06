#!/bin/sh
cp data/config linux-4.9.45-dev/.config
(cd linux-4.9.45-dev; make menuconfig)
cp linux-4.9.45-dev/.config data/config
