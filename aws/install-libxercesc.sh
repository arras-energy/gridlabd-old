#!/bin/bash

#copy gridlab source and install lib-xercesc
cd /home/ec2-user
mkdir gridlabd
cd gridlabd
git clone https://github.com/slacgismo/gridlabd source
cd /home/ec2-user/gridlabd/source/third_party/
. install_xercesc # need to source the script for automated run.
