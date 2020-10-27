![master](https://github.com/dchassin/gridlabd/workflows/master/badge.svg) ![develop](https://github.com/slacgismo/gridlabd/workflows/develop/badge.svg?branch=develop)

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

The preferred method of using SLAC releases of GridLAB-D is to download the SLAC master image from docker hub (see https://cloud.docker.com/u/gridlabd/repository/docker/gridlabd/slac-master).  You must install the docker daemon to use docker images.  See https://www.docker.com/get-started for details.

Once you have installed docker, you may issue the following commands to run GridLAB-D at the command line:
~~~
  host% docker run -it -v $PWD:/model gridlabd/slac-master gridlabd -W /model [load-options] [filename.glm] [run-options] 
~~~ 
On many systems, an alias can be used to make this a simple command that resemble the command you would normally issue to run a host-based installation:
~~~
  host% alias gridlabd='docker run -it -v $PWD:/model gridlabd/slac-master gridlabd -W /model'
~~~
Note that this alias will interfere with the host-based installation.

# Developer quick start

*Note*: This fork of [GridLAB-D](https://github.com/gridlab-d/gridlab-d) does not support MS Windows directly. You must use docker or a virtual machine running linux.

Normally on Linux and Mac OS X developers you should use the `install.sh` script to setup the system, perform the initial build, and install GridLAB-D for all users on the system. 
~~~
host% git clone https://github.com/slacgismo/gridlabd gridlabd
host% gridlabd/install.sh
~~~

The `gridlabd` command is added to the `/usr/local/bin` folder, so this folder must be included in the path for all users, e.g., as specified in `/etc/profile` or `/etc/profile.d`.

You may work with a user installation instead of a system installation. Assuming your development system is ready (see https://github.com/slacgismo/gridlabd/wiki/Install#mac-osx-and-linux for details), you can "quickly" download and build a host-based installation from a branch using the following commands:

~~~
  host% git clone https://github.com/slacgismo/gridlabd -b _branch-name_ _work-folder_
  host% cd _work-folder_
  host% autoreconf -isf
  host% ./configure --enable-silent-rules --prefix=$PWD/install [_options_]
  host% make -j install
  host% export PATH=$PWD/install/bin:$PATH
  host% gridlabd --version
  host% gridlabd --validate
~~~

## Useful configure options
 - `--with-mysql=/usr/local` to enable support for mysql (assuming you install mysql-dev on your system)
 - `CXXFLAGS='-w -O0 -g'` to enable debugging of C++ source code (e.g., module code)
 - `CFLAGS='-w -O0 -g'` to enable debugging of C source code (e.g., core code)

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
