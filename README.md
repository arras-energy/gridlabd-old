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

# Quick start using Docker

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

# Downloading pre-built images

Installation from downloads may require `sudo` priviledges and always requires `curl`. The `install` script will automatically download and install the latest production image for your system if you use the following command:

~~~
# curl -sL http://install.gridlabd.us/install.sh | sh
~~~

You can download the latest development image using the command:

~~~
# curl -sL http://install-dev.gridlabd.us/install.sh | sh
~~~

The installer recognizes the following environment variables:

| Variable | Default | Description
| -------- | ------- | -----------
| `INSTALL_SOURCE` | `http://install.gridlabd.us` | URL from which image is downloaded
| `INSTALL_TARGET` | `/usr/local/opt` | Folder in which image is installed
| `INSTALL_STDERR` | `/dev/stderr` | File to which error messages are sent
| `INSTALL_STDOUT` | `/dev/stdout` | File to which output messages are sent

This procedure may also be used in AWS EC2 instances and Docker containers.

If you have installed the AWS CLI, you can use the following command to get a list of available images:

~~~
aws s3 ls s3://install.gridlabd.us | grep tarz
~~~

Note that the installer only works with image name that conform to the name pattern `VERSION-BUILD-BRANCH-SYSTEM-MACHINE.tarz`.

# Build from source

The prerequesites for building HiPAS GridLAB-D from source include `git` and `curl`.  In general you can use the `setup.sh` script to verify and update your system so that the prerequesites are satisifed.  

On most systems, the process is as follows:

~~~
$ git clone https://code.gridlabd.us/ [-b BRANCH] gridlabd
$ cd gridlabd
$ ./setup.sh
$ ./build.sh --system --validate
~~~

If you want to clone an alternate repository, use the following `git` command instead:

~~~
$ git clone https://github.com/ORG/REPO [-b BRANCH] gridlabd
~~~

*Do not* run the `setup.sh` and `build.sh` scripts with `sudo`, as that will usually create a broken install. If necessary, you should give yourself permission to write `/usr/local`.

*Note*: This fork of [GridLAB-D](https://github.com/gridlab-d/gridlab-d) does not support MS Windows directly. You must use docker or a virtual machine running linux.

To upload the image to the AWS installer you must install the AWS CLI, and obtain credentials to access the installer's S3 buckets before using the command:

~~~
$ ./build.sh --upload
~~~

To make the image the latest release, use the command:

~~~
$ ./build.sh --release
~~~

When you are working in a master branch, these command will update `install.gridlabd.us`, otherwise the upload will go to `install-dev.gridlabd.us`.

## Docker

Developers should use the following commands to build GridLAB-D in a Docker container, where `USER` and `BRANCH` are your github username and the gridlabd branch:

~~~
$ docker run -it ubuntu:22.04
# apt update
# apt install git curl -y
# git clone https://github.com/$USER/gridlabd -b $BRANCH /gridlabd
# cd /gridlabd
# export GRIDLABD_ORIGIN=$USER/gridlabd/$BRANCH # only needed for setups from forks
# sh setup.sh
# python3.10 -m venv ~/.gridlabd
# . ~/.gridlabd/bin/activate
# sh build.sh
# gridlabd -T 0 --validate
~~~

## AWS EC2

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

## Pro Tips

1. If you accumulate a lot of local branches that no longer exist on the remote repo, you can use the following command to purge them:

~~~
host% git fetch -p && git branch -vv | awk '/: gone]/{print $1}' | xargs git branch -D
~~~

2. You can manage multiple installs using the `gridlabd version` command.  See `gridlabd version help` for details

3. You can speed up the `./configure` setup using `--cache-config` option with `./build.sh`

4. You can start a clean build using `--clean` option with `./build.sh`. Note that this will delete any new files not added with `git add`.

5. You can change the install prefix using the `--prefix FOLDER` option with `./build.sh`.

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
