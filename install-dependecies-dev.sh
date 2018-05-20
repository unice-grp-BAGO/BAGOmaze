#!/bin/bash

bash install-dependencies.sh

sudo apt install -y -qq \
	libncurses5-dev \
	libcdk5-dev \
	libncursesw5-dev \
	qt4-qmake
