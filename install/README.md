# Install

To install GridLAB-D without building, run the following command:

~~~
curl -sL http://install.gridlabd.us/install.sh | sh
~~~

To change the default installation:

~~~
export INSTALL_SOURCE=INSTALLURL
export INSTALL_TARGET=FOLDERNAME
export INSTALL_STDOUT=FILENAME
export INSTALL_STDERR=FILENAME
export GRIDLABD_IMAGE=OS_VERSION-MACHINE
curl -sL $INSTALL_SOURCE/install.sh | sh
~~~

# Build Install

The typical setup/install for a gridlabd on the system is as follows.

~~~
git clone https://core.gridlabd.us/ [-b BRANCH] [--depth 1] gridlabd
cd gridlabd
sudo ./setup.sh --local
./build.sh --system --validate --parallel
~~~

# Security

Arras Energy requires access to two python virtual environments to build and run.  The build `venv` is created in the user's home folder under the name `.gridlabd`.  This environment contains all the modules needed to build and install `gridlabd`. The runtime `venv` is created in the `gridlabd/bin` folder and contains all the modules needed to run `gridlabd`.  Because `gridlabd` users needed access to these, there must a set of group permissions for them to use.  The following procedure implements a security policy that achieves this.

1. Create a `gridlabd` user and group, e.g., `adduser gridlabd`.
2. Add each `gridlabd` user to the `gridlabd` group, e.g., `adduser $USER gridlabd`.
3. Download or build, and install `gridlabd` as the `gridlabd` user, e.g., `curl -sL https://install.gridlabd.us/install.sh | sudo sh`.
4. Change the permissions to the build `venv` and `/usr/local/opt/gridlabd` to allow group full access, e.g., `chmod -R g+rwx ~gridlabd /usr/local/opt/gridlabd`.
