# Install guide

## NREL APIKEY for nsrdb_weather.py

You must obtain an API key from https://developer.nrel.gov/signup/.  Save the key
in the credentials file, which is by default `$HOME/.nsrdb/credentials.json`.

on DOCKER the path is ./root/.nsrdb/credentials.json

You can run this process in a semi-automated manner using the command

~~~
bash$ gridlabd nsrdb_weather --signup=EMAIL --apikey=APIKEY
~~~

with which you can copy and paste a new key in the credential file.

If 


## Install CBC solver for PSPS optimization

Refernece at https://coin-or.github.io/coinbrew/

You should be able to get package from YUM package manager according to the github install instructions but the repository is missing in practice

The alternative is to install the Coin-or cbc solver manually


    wget https://raw.githubusercontent.com/coin-or/coinbrew/master/coinbrew

use wget to get master branch of coinbrew

if wget is not install use

    yum install wget

make the file executable by

    chmod u+x coinbrew


use the following command to install the cbc solver using coinbrew

    bash coinbrew fetch build Cbc@master --tests main --enable-debug --prefix=/usr/local

this should allow you to have

    cbc

as a command that is in the system path

if any issues arrise refer to reference link above

be sure to install using --prefix=/usr/local if not using instructions above
