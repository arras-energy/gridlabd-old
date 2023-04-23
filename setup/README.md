# Developer system setup

To setup a system for building GridLAB-D you must [install `curl` first](https://everything.curl.dev/get).

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

## Manual setup

Follow the scripts located in the `setup` folder of the repository you wish to use.
