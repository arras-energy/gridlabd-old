| Repository | Status |
| :---: | :---: |
| [GridLAB-D](https://github.com/slacgismo/gridlabd) | ![master](https://github.com/slacgismo/gridlabd/actions/workflows/master.yml/badge.svg?branch=master) ![develop](https://github.com/slacgismo/gridlabd/workflows/develop/badge.svg?branch=develop)
| [Templates](https://github.com/slacgismo/gridlabd-template) | [![master](https://github.com/slacgismo/gridlabd-template/actions/workflows/master.yml/badge.svg)](https://github.com/slacgismo/gridlabd-template/actions/workflows/master.yml) [![develop](https://github.com/slacgismo/gridlabd-template/actions/workflows/develop.yml/badge.svg)](https://github.com/slacgismo/gridlabd-template/actions/workflows/develop.yml)
| [Weather](https://github.com/slacgismo/gridlabd-weather) | [![validate](https://github.com/slacgismo/gridlabd-weather/actions/workflows/validate.yml/badge.svg)](https://github.com/slacgismo/gridlabd-weather/actions/workflows/validate.yml)
| [Library](https://github.com/slacgismo/gridlabd-library) | [![validate](https://github.com/slacgismo/gridlabd-library/actions/workflows/validate.yml/badge.svg)](https://github.com/slacgismo/gridlabd-library/actions/workflows/validate.yml)
| [Models](https://github.com/slacgismo/gridlabd-models) | [![validate](https://github.com/slacgismo/gridlabd-models/actions/workflows/validate.yml/badge.svg)](https://github.com/slacgismo/gridlabd-models/actions/workflows/validate.yml)
| [Benchmarks](https://github.com/slacgismo/gridlabd-benchmarks) | Manual test (see [README.md](https://github.com/slacgismo/gridlabd-benchmarks/blob/main/README.md))
| [Examples](https://github.com/slacgismo/gridlabd-examples) | Manual test (see [README.md](https://github.com/slacgismo/gridlabd-examples/blob/master/README.md))

The documentation for this project is located at http://docs.gridlabd.us/.

This respository contains the source code to HiPAS GridLAB-D, which is being developed by SLAC National Accelerator Laboratory for the California Energy Commission under grant [EPC-17-046](https://www.energy.ca.gov/filebrowser/download/1147).  This version of GridLAB-D is intended to be a commercial-grade version of the [US Department of Energy's research version of GridLAB-D developed by Pacific Northwest National Laboratory](https://github.com/gridlab-d/gridlab-d).

The source code in this repository is being developed in coordination with several other CEC and DOE projects conducted by or coordinated with SLAC and Stanford University.  Only participating SLAC and Stanford projects may contribute to this repository.  Changes made in this repository will be migrated back to the research version of GridLAB-D at PNNL's discretion.

The following projects are actively contributing to HiPAS GridLAB-D at this time:

  * GLOW (CEC funding, Hitachi America Laboratories lead)
  * OpenFIDO (CEC funding, SLAC National Accelerator Laboratory lead)
  * PowerNET (DOE funding, Stanford University lead)
  * PowerNET with Market (CEC funding, SLAC National Accelerator Laboratory lead)
  * VADER (DOE funding, SLAC National Accelerator Laboratory lead)
  * GRIP (DOE funding, SLAC National Accelerator Laboratory lead)
  * TESS (DOE funding, SLAC National Accelerator Laboratory lead)
  * Advanced Load Modeling (DOE funding, SLAC National Accelerator Laboratory lead)
  * LoadInsight (DOE funding, SLAC National Accelerator Laboratory lead)

# User quick start

The preferred method for running HiPAS GridLAB-D is to download the master image from docker hub (see https://hub.docker.com/repository/docker/hipas/gridlabd).  You must install the docker daemon to use docker images.  See https://www.docker.com/get-started for details.

Once you have installed docker, you may issue the following commands to run GridLAB-D at the command line:
~~~
$ docker run -it -v $PWD:/model hipas/gridlabd:latest gridlabd -W /model [load-options] [filename.glm] [run-options] 
~~~ 
On many systems, an alias can be used to make this a simple command that resembles the command you would normally issue to run a host-based installation:
~~~
$ alias gridlabd='docker run -it -v $PWD:/tmp hipas/gridlabd:latest gridlabd'
~~~
Note that this alias will interfere with any host-based installation. You should use the `gridlabd docker` command to manage the use of docker images concurrently with host-based installations.

# Installing from AWS

Installation from AWS is designed to be as simple as possible for a user. Installation requires a user to have sudo priviledges and either wget or curl installed. The '''-b branch-name''' flag is required if you want an image from a specific branch, otherwise it will default to using the master branch for install.sh and the develop branch for install-dev.sh. If no image is found for download for your specified branch and operating system, the fast install will fail. Important Note: This install script is very different from the install.sh script used to build gridlabd from source! If you don't want to download it remotely, it is also located in cloud/websites/install.gridlabd.us. 

Here is an example of running the install script on Ubuntu 22.04 to obtain the latest version of GridLAB-D:

~~~
# apt update
# apt install curl -y
# curl -sL http://install.gridlabd.us/install.sh | sh
~~~

## Docker

To install on a linux docker container or in a linux system, use the following commands:

~~~
$ docker run -it ubuntu:22.04
# apt-get update
# apt-get install curl -y
# curl -sL http://install.gridlabd.us/install.sh | sh
~~~

You can also use curl, if preferred. For official images, use install.sh from install.gridlabd.us.

## MACOS/Debian

To install on Mac, use the following commands:

~~~
$ curl -sL http://install.gridlabd.us/install.sh | sh
~~~

# Developer quick start

*Note*: This fork of [GridLAB-D](https://github.com/gridlab-d/gridlab-d) does not support MS Windows directly. You must use docker or a virtual machine running linux.

Normally on Linux and Mac OS X developers should use the `setup.sh` script to setup the system for GridLAB-D. *Do not* run the `build.sh` scripts with sudo, as that will create a broken install. 

~~~
$ git clone https://code.gridlabd.us/ [-b BRANCH] gridlabd
$ ./setup.sh
$ ./build.sh
~~~

### Docker

Developers should use a slightly modified install script to work on the develop branch, or to test fast-install images they have created on their own branches:

~~~
$ docker run -it ubuntu:22.04
# apt-get update
# apt-get install git curl -y
# git clone https://github.com/<USER>/gridlabd -b develop /gridlabd
# cd /gridlabd
# sh setup.sh
# python3.10 -m venv 
# sh build.sh
# gridlabd -T 0 --validate
~~~

You can also use curl, if preferred. For official images, use install.sh from install.gridlabd.us.

### MACOS/Debian
To install on Mac, use the following commands:

~~~
curl -OJ https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/install-dev.sh
chmod 755 install-dev.sh
./install-dev.sh -b image-branch-name
~~~

The O and J flags for curl tells it to utilize the provided filename, otherwise you will have to manually name the file when you download it, and adjust the commands accordingly.

## Flags
The fast-install script offers a few select flags for selecting your install.


`-b your-branch here` is used to select a branch, otherwise the install will default to the master branch.

`-v #.#.#` is used to specify a specific version, and is used for when you want to use a past fast-install version. Otherwise, the script will default to the latest version of the branch being used.

`-h` is your standard help, and will display these options when called.

### AWS EC2 Installation 
Use the AWS Ubuntu AMI to build gridlabd on AWS EC2.

1) Set the path variable
~~~
host% export PATH=/usr/local/bin:$PATH
~~~
2) Update work directory permissions, change work directory and clone GitHub repository
~~~
host% sudo chown -R ${USER:-root} /usr/local/src
host% cd /usr/local/src
host% git clone https://source.gridlabd.us/ gridlabd
~~~
3) Run installation 
~~~ 
host% cd gridlabd
host% gridlabd/install.sh
~~~
To rebuild the source code and install again, use the `make system` command.  You can use parallel builds using the `make -j<nproc> system` command.

If you have modified the branch name or version information, you must reconfigure your build using the `make reconfigure` command before using `make system`.

Each build of HiPAS GridLAB-D will be installed in `/usr/local/opt/gridlabd`. Links to the active version are added to the `/usr/local/bin` folder, so this folder must be included in the path for all users, e.g., as specified in `/etc/profile` or `/etc/profile.d`. Additional links are created in `/usr/local/lib` and `/usr/local/share`, as needed. 

You may use the `gridlabd version` command to manage which version is active on the system. See the [`gridlabd version`](http://docs.gridlabd.us/index.html?owner=hipas&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Version.md) command for details.

You use `make install` to build only. To use an inactive build run the `gridlabd` command of that build instead of running the active version.  For example, if you only built `4.2.13-201019-develop` then you can run `/usr/local/opt/gridlabd/4.2.13-201019-develop/bin/gridlabd` to run it instead of running `/usr/local/bin/gridlabd`.

Before using a build of gridlabd, you should always validate it using `gridlabd --validate` in the root folder of the source tree. Be careful to verify that the branch of the source tree matches the branch of the version you are running. This is not checked automatically.

## Windows WSL

Generally, running HiPAS GridLAB-D on Docker is preferred because it is usually faster. Building, running and installing Gridlabd in WSL is not that different from a normal linux installation. You can follow Microsoft's instructions on setting up WSL and adding/changing distro's [here](https://learn.microsoft.com/en-us/windows/wsl/install). These instructions work for both cases on supported operating systems, which you can find in the build-aux directory.

1) Open PowerShell as administrator (Or run the WSL(Ubuntu) or Debian App from the start menu to open a dedicated terminal)
2) Run `wsl` (Using Debian or Ubuntu)

### Using a fast-install image

3a) Since WSL runs as a linux distribution, the linux installation methods also work here. The fastest, recommended way to install would be to follow the linux fast install method.

~~~
apt-get update
apt-get install sudo wget -y
wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/install-dev.sh
chmod 755 install-dev.sh
./install-dev.sh -b image-branch-name
~~~

### Building from Source

3b) Change to your working directory `/usr/local/src`
4) Update `apt` and install `git`
~~~
  root@host:/usr/local/src# apt update -y
  root@host:/usr/local/src# apt install git -y
~~~
5) Clone `gridlabd` and change to the `gridlabd` directory
~~~
  root@host:/usr/local/src# sudo chown ${USER:-root} /usr/local/src
  root@host:/usr/local/src# git clone https://source.gridlabd.us/
  root@host:/usr/local/src# cd gridlabd
~~~
6) Run `install.sh` for your first-time setup. *Do not* run the install.sh script with Sudo, as that will create a broken install. The script itself selectively uses sudo only where necessary.
~~~
  root@host:/usr/local/src/gridlabd# ./install.sh
~~~

After the initial setup, if you want to make changes to the gridlabd build, you can run the following steps to rebuild gridlabd with your updates.

7) Clean old build and Run `autoconf` (make sure any of your changes are committed!)
~~~
  root@host:/usr/local/src/gridlabd# git clean -fxd
  root@host:/usr/local/src/gridlabd# autoreconf -isf
~~~
8) Run `configure`
~~~
  root@host:/usr/local/src/gridlabd# ./configure
~~~
9) Make `system`
~~~
  root@host:/usr/local/src/gridlabd# make system
~~~

## Validation
All gridlabd builds should be validated after you have completed the build. This is to ensure that the build was successful and gridlabd is running as intended. Validation is run the exact same way across all operating systems.

1) Change to your downloaded gridlabd repository
~~~
  root@host:~# cd /usr/local/src/gridlabd
~~~

2) Validate `gridlabd`
~~~
  root@host:/usr/local/src/gridlabd# gridlabd --validate
~~~

## Building and Debugging

You can configure a debugging version using `make reconfigure-debug`.  When debugging is enabled you can use the [`gridlabd trace`](http://docs.gridlabd.us/index.html?owner=hipas&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Trace.md) command and the [`gridlabd gdb`](http://docs.gridlabd.us/index.html?owner=hipas&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Gdb.md) (for linux) or [`gridlabd lldb`](http://docs.gridlabd.us/index.html?owner=hipas&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Lldb.md) (for Mac OSX) commands to debug a simulation.

## Building your own gridlabd fast-install images

Additional developer documentation for making gridlabd images is located in the readme in cloud/websites/install.gridlabd.us. 

It is strongly recommended to use clean virtual machines or docker images when creating a new image, in order to prevent unnecessary libraries from being saved in to the package. The readme also contains instructions on how to retrieve a built image from these sources. 

## Notes
- The version number should contain the _branch-name_.  If not, use the `which gridlabd` command to check that the path is correct.
- You can control whether your local version run the docker image instead of the local install using the `--docker` command-line option.
- In theory all validate tests of the master should pass. However, sometimes issues arise that aren't caught until after a merge into master.  If you encounter a validation error, please check the issues to see if it has not already been reported.  When reporting such a problem, please include the `--origin` command line option output, the `validate.txt` output, and the output from `uname -a` to assist in reproducing and diagnosing the problem.
- Each version of gridlabd is built as an entirely independent package, with its own independent python installation. This means that if you update the version or change the branch, you will likely need to run another complete build of gridlabd using the `install.sh` script. 

## Pro tip

If you accumulate a lot of local branches that no longer exist on the remote repo, you can use the following command to purge them:

~~~
host% git fetch -p && git branch -vv | awk '/: gone]/{print $1}' | xargs git branch -D
~~~

## Citation

If you use this fork of GridLAB-D for a publication you are required to cite it, e.g.,

Chassin, D.P., et al., "GridLAB-D Version _major_._minor_._patch_-_build_ (_branch_) _platform_", (_year_) [online]. Available at _url_, Accessed on: _month_ _day_, _year_.

You may use the `--cite` command option to obtain the correct citation for your version:
~~~
host% gridlabd --cite
Chassin, D.P., et al. "GridLAB-D 4.2.0-191008 (fix_python_validate) DARWIN", (2019) [online]. Available at https://source.gridlabd.us/commit/dfc392dc0208419ce9be0706f699fdd9a11e3f5b, Accessed on: Oct. 8, 2019.
~~~
This will allow anyone to identify the exact version you are using to obtain it from GitHub.

## US Government Rights

This version of GridLAB-D is derived from the original US Department of Energy version of GridLAB-D developed at Pacific Northwest National Laboratory. The US Government retains certain rights as described in [the original GridLAB-D license](https://raw.githubusercontent.com/gridlab-d/gridlab-d/master/LICENSE).

## Contributions

Please see https://source.gridlabd.us/blob/master/CONTRIBUTING.md for information on making contributions to this repository.
