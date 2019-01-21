#!/bin/bash

EXENAME="$(basename $0)"
function error()
{
	echo "$EXENAME: $2"
	exit $1
}
UNAME="$(uname)"
case "$UNAME" in
(Darwin) # Mac OSX
	if [ "$(brew update||echo none)" == "none" ]; then
		ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
	fi
	brew install autoconf
	brew install automake
	brew install libtool
	brew install gnu-sed && sudo ln -s /usr/local/bin/gsed /usr/local/bin
	brew install xerces-c
	brew install mysql-connector-c
	brew install armadillo
	;;
# (Linux)
# 	if [ "$(yum version)||echo none" != "none" ]; then
# 		yum update -y
# 		yum install systemd -y
# 		yum groupinstall "Development Tools" -y
# 		yum install cmake -y
# 		yum install xerces-c -y
# 		yum clean all
# 	else
# 		error 1 "package manager not recognized"
# 	fi
# 	;;
(*)	# other
	error 1 "unable to verify requirements for $UNAME"
	;;
esac
echo "system ready"
exit 0
