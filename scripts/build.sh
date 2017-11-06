#!/bin/sh
cp data/config linux-4.9.45-dev/.config
(cd linux-4.9.45-dev; make -j4)
