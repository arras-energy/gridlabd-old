#!/bin/bash

PYTHON_VERSION=3.10

if [ "$(whoami)" != "root" ]; then
	echo "ERROR: you must run setup as root or use sudo"
	exit 1
fi

apt update
apt upgrade -y

if [ -z "$(which  python$PYTHON_VERSION)" ]; then
	echo "Installing python${PYTHON_VERSION}..."
	apt install software-properties-common -y
	add-apt-repository ppa:deadsnakes/ppa
	apt install python$PYTHON_VERSION -y
fi
