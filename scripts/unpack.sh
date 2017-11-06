#!/bin/sh
rm -rf linux-4.9.45-dev linux-4.9.45-original
tar xf linux-4.9.45.tar.gz
mv linux-4.9.45 linux-4.9.45-original
cp -r linux-4.9.45-original linux-4.9.45-dev
