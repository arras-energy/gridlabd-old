[[/Developer/Install]] -- Installation procedure

# Synopsis

Command line:

~~~
bash$ ./install.sh [<install-options>] [<configure-options>]
~~~

# Description

**WARNING**: If you have not made a backup of your system, *stop here and read the **Caveat** section below*.

Building GridLAB-D from the source code is only for developers of the commercial *HiPAS* version of GridLAB-D. General users do not need to do this, and should refer to the [[/Getting Started]] page instead.  

This installer is for the commercial *HiPAS* version of GridLAB-D (GridLAB-D 4.2) only.  The installers for research release versions of GridLAB-D are available at http://sourceforge.net/projects/gridlab-d/files/ (for Version 3 and older) or https://github.com/gridlab-d/gridlab-d (Version 4 or later).

# Downloading and installing from the source repository

The `master`, `develop`, and all working branches of the source code can be downloaded from the GitHub project http://source.gridlabd.us/.  

## System preparation

Prior to building and installing GridLAB-D you will need to prepare your system.

### Install `curl`

You must first install `curl` on your system, if you have not already done so. To verify whether you have `curl` installed, open a command shell or terminal and type the command:

~~~
bash$ curl --version
~~~

The exact command to install `curl` will depend on your hardware and software platform.  Generally, Mac OS X users should use [`brew`](https://brew.sh).  Linux users should use `yum`, `apt`, or whichever package installer is available.

## Automated installation

An automated developer's installer is available for GridLAB-D.  To run this installer, type

~~~
bash$ curl -L http://code.gridlabd.us/<branch>/install.sh | bash
~~~

### Manual installation

Manual installation is discouraged because the process is complex, highly error-prone, and varies widely from one platform to another.  However, it is necessary on platforms that are not supported by the automated installation script.  The general approach is roughly as follows, keeping in mind that the specific will vary from one system to another, and you may need to install certain tools and libraries to be successful.

~~~
bash$ git clone https://source.gridlabd.us/ gridlabd
bash$ cd gridlabd
bash$ autoreconf -isf
bash$ ./configure
bash$ make system
bash$ export PATH=/usr/local/bin:$PATH
bash$ export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH:-/usr/local/lib}
bash$ gridlabd validate
~~~

You can manually install to the system folders, in which case all users will have access to the install, the user folders in which case only the current user will have access to the install, or to a working folder, which is typical for development installations. By default, the build is configured to install the `/usr/local/opt/gridlabd` and link the installed version to `/usr/local`. The linking process is managed by the `gridlabd version` subcommand, as described in the [[/Subcommand/Version]] page. To change this, use the `--prefix=<location>` option on the `./configure` command, e.g., to install a user-only copy:

~~~
bash$ ./configure --prefix=$HOME/gridlabd
~~~

or to install a development copy:

~~~
bash$ ./configure --prefix=$PWD/install
~~~

### Change branch

When you change the branch in the source, you will have to completely rebuild the installation.  The easiest way to do this is

~~~
bash$ make reconfigure
bash$ make system
~~~

If you want to speed up the installation, you can use the parallel make process, e.g.,

~~~
bash$ make -j30 system
~~~

which allows multiple build steps to run in parallel (in this case 30 at a time).

## Options

Install supports the following command line options:

* `-h|--help`: Print this helpful output
* `--info`: Print information about this install
* `-c|--no-check`: Do not check system for requirements
* `-d|--no-docs`: Do not install documentation
* `-f|--force`: Force install into existing target folder
* `-i|--no-index`: Do not index data archives
* `-l|--no-link`: Do not link new install to activate it
* `-t|--no-test`: Do not run validation tests
* `-u|--no-update`: Do not update system to meet requirements
* `-p|--parallel`: Enable parallelism when possible
* `--prefix <path>`: Set install prefix
* `--save`: Save the current configuration as default
* `--no-setup`: Perform system setup
* `--reset`: Reset the configuration to default
* `-q|--quick`: Run only updates instead of a clean install
* `-s|--silent`: Run without showing commands
* `-v|--verbose`: Run showing log output
* `--validate`: Run validation tests
* `--version <name>`: Override the default version name

# Caveat

The automated installer, and certainly any attempt to manually install is risky.  As with any change to your system, you must be certain you have a backup of your system prior to running the installation procedure.  If the process goes wrong, only a backup made prior to running these commands can restore your system to its condition prior to running the installation procedure. 

# See also

* [[/Cloud/AWS]]
* [[/Cloud/CircleCI]]
* [[/Install/Docker]]
* [[/Subcommand/Version]]