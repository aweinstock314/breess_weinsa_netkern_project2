#!/bin/sh
(cd linux-4.9.45-dev; make mrproper > /dev/null)
diff -rcNP linux-4.9.45-dev linux-4.9.45-original
cp data/config linux-4.9.45-dev/.config
