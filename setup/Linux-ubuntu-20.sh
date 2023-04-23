#!/bin/bash

PYTHON_VERSION=3.10.8

if ! "$(whoami)" == "root"; then
	echo "ERROR: you must run setup as root or use sudo"
	exit 1
fi

apt update
apt upgrade -y

if "$(which  python$PYTHON_VERSION)" == "" ; then
	echo "Installing python${PYTHON_VERSION}..."
	apt install software-properties-common -y
	add-app-repository ppa:deadsnakes/ppa
	apt install python$PYTHON_VERSION -qqqqy
	exit 1
fi
