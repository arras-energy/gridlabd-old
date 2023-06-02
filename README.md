---

<div align=center><H3>IMPORTANT NOTICE</H3></div>

This is the last release of HiPAS GridLAB-D before it is transferred to LF Energy's [Arras Energy project](https://github.com/arras-energy).

---

| Repository | Build | Deploy |
| :---: | :---: | :---: |
| [GridLAB-D](https://github.com/arras-energy/gridlabd) | ![master](https://github.com/arras-energy/gridlabd/actions/workflows/master.yml/badge.svg?branch=master) ![develop](https://github.com/arras-energy/gridlabd/workflows/develop/badge.svg?branch=develop) | [![master-images](https://github.com/arras-energy/gridlabd/actions/workflows/master-image.yml/badge.svg)](https://github.com/arras-energy/gridlabd/actions/workflows/master-image.yml) [![develop-images](https://github.com/arras-energy/gridlabd/actions/workflows/develop-image.yml/badge.svg)](https://github.com/arras-energy/gridlabd/actions/workflows/develop-image.yml) |
| [Templates](https://github.com/arras-energy/gridlabd-template) | [![master](https://github.com/arras-energy/gridlabd-template/actions/workflows/master.yml/badge.svg)](https://github.com/arras-energy/gridlabd-template/actions/workflows/master.yml) [![develop](https://github.com/arras-energy/gridlabd-template/actions/workflows/develop.yml/badge.svg)](https://github.com/arras-energy/gridlabd-template/actions/workflows/develop.yml)
| [Weather](https://github.com/arras-energy/gridlabd-weather) | [![validate](https://github.com/arras-energy/gridlabd-weather/actions/workflows/validate.yml/badge.svg)](https://github.com/arras-energy/gridlabd-weather/actions/workflows/validate.yml)
| [Library](https://github.com/arras-energy/gridlabd-library) | [![validate](https://github.com/arras-energy/gridlabd-library/actions/workflows/master.yml/badge.svg)](https://github.com/arras-energy/gridlabd-library/actions/workflows/master.yml) [![validate](https://github.com/arras-energy/gridlabd-library/actions/workflows/develop.yml/badge.svg)](https://github.com/arras-energy/gridlabd-library/actions/workflows/develop.yml)
| [Models](https://github.com/arras-energy/gridlabd-models) | [![validate](https://github.com/arras-energy/gridlabd-models/actions/workflows/validate.yml/badge.svg)](https://github.com/arras-energy/gridlabd-models/actions/workflows/validate.yml)
| [Benchmarks](https://github.com/arras-energy/gridlabd-benchmarks) | Manual test (see [README.md](https://github.com/arras-energy/gridlabd-benchmarks/blob/main/README.md))
| [Examples](https://github.com/arras-energy/gridlabd-examples) | Manual test (see [README.md](https://github.com/arras-energy/gridlabd-examples/blob/master/README.md))


The documentation for this project is located at http://docs.gridlabd.us/.

This respository contains the source code to HiPAS GridLAB-D, which is being developed by SLAC National Accelerator Laboratory for the California Energy Commission under grant [EPC-17-046](https://www.energy.ca.gov/filebrowser/download/1147).  This version of GridLAB-D is intended to be a commercial-grade version of the [US Department of Energy's research version of GridLAB-D developed by Pacific Northwest National Laboratory](https://github.com/gridlab-d/gridlab-d).

*Note*: This fork of [GridLAB-D](https://github.com/gridlab-d/gridlab-d) does not support MS Windows directly. You must use docker or a virtual machine running linux.

# Quick start using Docker

The preferred method for running HiPAS GridLAB-D is to download the master image from docker hub (see https://hub.docker.com/repository/docker/arras-energy/gridlabd).  You must install the docker daemon to use docker images.  See https://www.docker.com/get-started for details.

Once you have installed docker, you may issue the following commands to run GridLAB-D at the command line:

~~~
docker run -it -v $PWD:/model arras-energy/gridlabd:latest gridlabd -W /model [LOADOPTIONS] [FILENAME.EXT] [RUNOPTIONS] 
~~~ 

On many systems, an alias can be used to make this a simple command that resembles the command you would normally issue to run a host-based installation:

~~~
alias gridlabd='docker run -it -v $PWD:/tmp arras-energy/gridlabd:latest gridlabd'
~~~

Note that this alias will interfere with any host-based installation. You may use the `gridlabd docker` command to manage the use of docker images concurrently with host-based installations.

# Downloading pre-built images

Installation from downloads may require `sudo` priviledges and always requires `curl`. The `install` script will automatically download and install the latest production image for your system if you use the following command:

~~~
curl -sL http://install.gridlabd.us/install.sh | [sudo] sh
~~~

You can download the latest development image using the command:

~~~
curl -sL http://install-dev.gridlabd.us/install.sh | [sudo] sh
~~~

If you must use `sudo`, then don't forget to grant user permission to access the build and runtime virtual environments created by the installer, e.g.,

~~~
sudo chmod -R g+rwx ~root /usr/local
sudo adduser $USER root
~~~

If you want to use a more secure approach to sharing the install among multiple users, see [install/README.md](install/README.md#Security).

The installer recognizes the following environment variables:

| Variable | Default | Description
| -------- | ------- | -----------
| `INSTALL_SOURCE` | `http://install.gridlabd.us` | URL from which image is downloaded
| `INSTALL_TARGET` | `/usr/local/opt` | Folder in which image is installed
| `INSTALL_STDERR` | `/dev/stderr` | File to which error messages are sent
| `INSTALL_STDOUT` | `/dev/stdout` | File to which output messages are sent
| `GRIDLABD_IMAGE` | *varies* | Install image name, e.g., `$OSNAME_$VERMAJOR-$MACHINE`

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
git clone https://code.gridlabd.us/ [-b BRANCH] gridlabd
cd gridlabd
./setup.sh --local
./build.sh --system --validate
~~~

If you want to clone an alternate repository, use the following `git` command instead:

~~~
git clone https://github.com/ORG/REPO [-b BRANCH] gridlabd
~~~

If you do not specify the `--local` then by default the `setup.sh` source will match the `git` repository origin and branch if any. Otherwise the default source will be `arras-energy/gridlabd/master`. If you want to setup from a different origin, use the command `export GRIDLABD_ORIGIN=ORG/REPO/BRANCH` to specify an alternate source for `setup.sh`.  The `build.sh` will also match the current `git` repository.

*Do not* run the `setup.sh` and `build.sh` scripts with `sudo`, as that will usually create a broken install. If necessary, you should give yourself permission to write `/usr/local` and `brew`'s install folder. If you have not already done so, add `brew` to your path.

To upload the image to the AWS installer you must install the AWS CLI, and obtain credentials to access the installer's S3 buckets before using the command:

~~~
./build.sh --upload
~~~

To make the image the latest release, use the command:

~~~
./build.sh --release
~~~

When you are working in a master branch, these command will update `install.gridlabd.us`, otherwise the upload will go to `install-dev.gridlabd.us`.

## Docker

Developers should use the following command to build GridLAB-D in a Docker container:

~~~
docker/build.sh
~~~

Note that Docker will build the currently checked out branch *from the repository rather than from your local code*.

To push the docker image to your personal Dockerhub, use the command:

~~~
docker/build.sh --push
~~~

The Dockerhub account is assumed to match the name of your GitHub account.

To release the docker image, use the command:

~~~
docker/build.sh --release
~~~

## AWS EC2

The latest development and master builds of HiPAS gridlabd are available as community AMIs.
Simply launch an ec2, browse the community AMIs and search for HiPAS Gridlabd

If you want to build gridlabd yourself, use the AWS Ubuntu AMI on AWS EC2 using the commands

~~~
git clone https://code.gridlabd.us/ [-b BRANCH] gridlabd
cd gridlabd
./setup.sh --local
./build.sh --system --validate
~~~

## Windows WSL

Generally, running HiPAS GridLAB-D on Docker is preferred because it is usually faster. Building, running and installing Gridlabd in WSL is not that different from a normal linux installation. You can follow Microsoft's instructions on setting up WSL and adding/changing distro's [here](https://learn.microsoft.com/en-us/windows/wsl/install). These instructions work for both cases on supported operating systems, which you can find in the build-aux directory.

1) Open PowerShell as administrator or run the WSL (Ubuntu) from the start menu to open a dedicated terminal
2) Run `wsl` (Using Ubuntu)
3) Follow the Linux build procedure above.

## Manual Build

You can build HiPAS GridLAB-D manually by running following commands in the top level repository folder:

1. Create the target folder:

~~~
mkdir -p /usr/local/opt/gridlabd
~~~

2. Activate the python build environment

~~~
. $HOME/.gridlabd/bin/activate
~~~

3. Create the configuration script

~~~
autoreconf -isf
~~~

4. Run the configuration script

~~~
./configure
~~~

5. Compile everything

~~~
make
~~~

6. Install everything

~~~
make install
~~~

7. Validate the install

~~~
make validate
~~~

8. Release install to all users

~~~
make system
~~~

## Pro Tips

1. If you accumulate a lot of local branches that no longer exist on the remote repo, you can use the following command to purge them:

~~~
host% git fetch -p && git branch -vv | awk '/: gone]/{print $1}' | xargs git branch -D
~~~

2. You can manage multiple installs using the `gridlabd version` command.  See `gridlabd version help` for details

3. You can prevent `./configure` using the configure cache by deleting the `config.cache` folder.

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

