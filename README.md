| Repository | Status |
| :---: | :---: |
| GridLAB-D | ![master](https://github.com/slacgismo/gridlabd/actions/workflows/master.yml/badge.svg?branch=master) ![develop](https://github.com/slacgismo/gridlabd/workflows/develop/badge.svg?branch=develop)
| Templates | [![master](https://github.com/slacgismo/gridlabd-template/actions/workflows/master.yml/badge.svg)](https://github.com/slacgismo/gridlabd-template/actions/workflows/master.yml) [![develop](https://github.com/slacgismo/gridlabd-template/actions/workflows/develop.yml/badge.svg)](https://github.com/slacgismo/gridlabd-template/actions/workflows/develop.yml)
| Weather | [![validate](https://github.com/slacgismo/gridlabd-weather/actions/workflows/validate.yml/badge.svg)](https://github.com/slacgismo/gridlabd-weather/actions/workflows/validate.yml)
| Library | [![validate](https://github.com/slacgismo/gridlabd-library/actions/workflows/validate.yml/badge.svg)](https://github.com/slacgismo/gridlabd-library/actions/workflows/validate.yml)
| Models | [![validate](https://github.com/slacgismo/gridlabd-models/actions/workflows/validate.yml/badge.svg)](https://github.com/slacgismo/gridlabd-models/actions/workflows/validate.yml)
| Benchmarks | (no status)
| Examples | (no status)

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

The preferred method for running HiPAS GridLAB-D is to download the SLAC master image from docker hub (see https://hub.docker.com/repository/docker/slacgismo/gridlabd).  You must install the docker daemon to use docker images.  See https://www.docker.com/get-started for details.

Once you have installed docker, you may issue the following commands to run GridLAB-D at the command line:
~~~
  host% docker run -it -v $PWD:/model slacgismo/gridlabd:latest gridlabd -W /model [load-options] [filename.glm] [run-options] 
~~~ 
On many systems, an alias can be used to make this a simple command that resembles the command you would normally issue to run a host-based installation:
~~~
  host% alias gridlabd='docker run -it -v $PWD:/tmp slacgismo/gridlabd:latest gridlabd'
~~~
Note that this alias will interfere with any host-based installation. You should use the `gridlabd docker` command to manage the use of docker images concurrently with host-based installations.

# Developer quick start

*Note*: This fork of [GridLAB-D](https://github.com/gridlab-d/gridlab-d) does not support MS Windows directly. You must use docker or a virtual machine running linux.

Normally on Linux and Mac OS X developers should use the `install.sh` script to setup the system, perform the initial build, and install GridLAB-D for all users on the system. 
~~~
host% git clone https://github.com/slacgismo/gridlabd gridlabd
host% gridlabd/install.sh
~~~
### AWS EC2 Installation 
1) Set the path variable
~~~
host% sudo su
host% export PATH=/usr/local/bin:$PATH
~~~
2) Change work dictionary and clone GitHub repository
~~~
host% cd /usr/local/src
host% git clone https://github.com/slacgismo/gridlabd gridlabd
~~~
3) Run installation 
~~~ 
host% gridlabd/install.sh
~~~
To rebuild the source code and install again, use the `make system` command.  You can use parallel builds using the `make -j<nproc> system` command.

If you have modified the branch name or version information, you must reconfigure your build using the `make reconfigure` command before using `make system`.

Each build of HiPAS GridLAB-D will be installed in `/usr/local/opt/gridlabd`. Links to the active version are added to the `/usr/local/bin` folder, so this folder must be included in the path for all users, e.g., as specified in `/etc/profile` or `/etc/profile.d`. Additional links are created in `/usr/local/lib` and `/usr/local/share`, as needed. 

You may use the `gridlabd version` command to manage which version is active on the system. See the [`gridlabd version`](http://docs.gridlabd.us/index.html?owner=slacgismo&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Version.md) command for details.

You use `make install` to build only. To use an inactive build run the `gridlabd` command of that build instead of running the active version.  For example, if you only built `4.2.13-201019-develop` then you can run `/usr/local/opt/gridlabd/4.2.13-201019-develop/bin/gridlabd` to run it instead of running `/usr/local/bin/gridlabd`.

Before using a build of gridlabd, you should always validate it using `gridlabd --validate` in the root folder of the source tree. Be careful to verify that the branch of the source tree matches the branch of the version you are running. This is not checked automatically.

## Windows WSL

Generally, running HiPAS GridLAB-D on Docker is preferred because it is usually faster. You can build, install, and run GridLAB-D in WSL as well by doing the following:

1) Open PowerShell as administrator
2) Run `wsl` (the Debian distro is preferred, but Ubuntu should work also)
3) Change directory to `/usr/local/src`
4) Update `apt` and install `git`
~~~
  root@host:/usr/local/src# apt update -y
  root@host:/usr/local/src# apt install git -y
~~~
5) Clone `gridlabd` and change to the `gridlabd` directory
~~~
  root@host:/usr/local/src# git clone https://source.gridlabd.us/
  root@host:/usr/local/src# cd gridlabd
~~~
6) Run `autoconf`
~~~
  root@host:/usr/local/src/gridlabd# autoreconf -isf
~~~
7) Run `configure`
~~~
  root@host:/usr/local/src/gridlabd# ./configure
~~~
8) Make `system`
~~~
  root@host:/usr/local/src/gridlabd# make system
~~~
9) Validate `gridlabd`
~~~
  root@host:/usr/local/src/gridlabd# gridlabd --validate
~~~

## Building and Debugging

You can configure a debugging version using `make reconfigure-debug`.  When debugging is enabled you can use the [`gridlabd trace`](http://docs.gridlabd.us/index.html?owner=slacgismo&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Trace.md) command and the [`gridlabd gdb`](http://docs.gridlabd.us/index.html?owner=slacgismo&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Gdb.md) (for linux) or [`gridlabd lldb`](http://docs.gridlabd.us/index.html?owner=slacgismo&project=gridlabd&branch=master&folder=/Subcommand&doc=/Subcommand/Lldb.md) (for Mac OSX) commands to debug a simulation.

## Notes
- The version number should contain the _branch-name_.  If not, use the `which gridlabd` command to check that the path is correct.
- You can control whether your local version run the docker image instead of the local install using the `--docker` command-line option.
- In theory all validate tests of the master should pass. However, sometimes issues arise that aren't caught until after a merge into master.  If you encounter a validation error, please check the issues to see if it has not already been reported.  When reporting such a problem, please include the `--origin` command line option output, the `validate.txt` output, and the output from `uname -a` to assist in reproducing and diagnosing the problem.

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
Chassin, D.P., et al. "GridLAB-D 4.2.0-191008 (fix_python_validate) DARWIN", (2019) [online]. Available at https://github.com/slacgismo/gridlabd/commit/dfc392dc0208419ce9be0706f699fdd9a11e3f5b, Accessed on: Oct. 8, 2019.
~~~
This will allow anyone to identify the exact version you are using to obtain it from GitHub.

## Contributions

Please see https://github.com/slacgismo/gridlabd/blob/master/CONTRIBUTING.md for information on making contributions to this repository.
