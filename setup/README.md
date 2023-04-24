# Developer system setup

## Preparation

You should update your installer, e.g., with `brew update` or `apt update`.  

It's also a good idea to upgrade any old or outdated packages, e.g., with `brew upgrade` or `apt upgrade`.  

You will need to install curl, e.g., with `brew install curl` or `apt install curl`. See [the `curl` website for details](https://everything.curl.dev/get).

## Automated setup

To setup your system to build the latest release, run the command:

~~~
curl -sL https://code.gridlabd.us/master/setup.sh | sh
~~~

To setup for a development build, run the command:

~~~
export GRIDLABD_ORG=slacgismo
export GRIDLABD_REPO=gridlabd
export GRIDLABD_BRANCH=develop
curl -sL https://raw.githubusercontent.com/${GRIDLABD_ORG}/${GRIDLABD_REPO}/${GRIDLABD_BRANCH}/setup.sh | sh
~~~

where you can substitute your own values for `GRIDLABD_ORG`, `GRIDLABD_REPO`, and `GRIDLABD_BRANCH` as needed.

After the setup is complete, you can active the gridlabd build environment for python using the command

~~~
. $HOME/.venv/gridlabd/bin/activate
~~~

Once you have completed this procedure you may download and build GridLAB-D using the usual procedure, e.g.,

~~~
git clone https://github.com/$GRIDLABD_ORG/$GRIDLABD_REPO -b $GRIDLABD_BRANCH --depth 1 gridlabd
cd /gridlabd
autoreconf -isf
./configure
make -j$(($(nproc)*3)) system
gridlabd -T 0 --validate
~~~

## Manual setup

Follow the scripts located in the `setup` folder of the repository you wish to use.
