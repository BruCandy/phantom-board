#!/bin/bash

sudo cp ../rules/99-phantom-kbd.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
