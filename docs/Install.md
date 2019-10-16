The repository is only for building and installing the current version of GridLAB-D from source.  The installers for previously released versions are available at http://sourceforge.net/projects/gridlab-d/files/ or https://github/gridlab-d/gridlab-d.

# Downloading and installing from the source repository

The development and master release source code can be downloaded from the GitHub project https://github.com/dchassin/gridlabd.  You can install to the system folders, in which case all users will have access to the install, the user folders in which case only the current user will have access to the install, or to a working folder, which is typical for development installations.

## Downloading the source

```
  host% git clone https://github.com/dchassin/gridlabd source
```

## Install to the system folders

```
  host% cd source
  host% autoreconf -isf
  host% ./configure
  host% make install
  host% export PATH=/usr/local/bin:$PATH
```

## Install to the user folders

```
  host% cd source
  host% autoreconf -isf
  host% ./configure --prefix=$HOME
  host% make install
  host% export PATH=$HOME/bin:$PATH
```

## Install to a working folder

```
  host% cd source
  host% autoreconf -isf
  host% ./configure --prefix=$PWD/install
  host% make install
  host% export PATH=$PWD/install/bin:$PATH
```

## Validating the installation
  host% gridlabd --validate

# Using docker

See https://github.com/dchassin/gridlabd/tree/master/utilities/docker for DockerFile and readme.

## Windows build

Windows users will need to install cygwin to enable building using autotools.
See http://gridlab-d.sourceforge.net/wiki/index.php/MinGW/Eclipse_Installation
for details.

## Eclipse Editor Setup

You can setup Eclipse as your GridLAB-D modeling editor.  See 
http://gridlab-d.sourceforge.net/wiki/index.php/Eclipse for details.

# Online Documentation

The primary manuals for GridLAB-D are online at Main Page for GridLAB-D (http://gridlab-d.sourceforge.net/wiki/index.php/Main_Page).
Documentation of local projects can be found on this project's wiki (https://code.stanford.edu/gridlabd/gridlabd/wikis/home).

# Mac OSX and Linux

The following is the basic procedure for installing GridLAB-D from this GitHub project.

1. Installing brew (Mac OSX)
~~~
% ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
% brew doctor
~~~
(some older versions of MacOS may require you to use `sudo brew doctor`)

2. Build tools:
Use `brew` on MacOSX, or the appropriate package manager on Linux (e.g., `yum`, `apt-get`).

~~~
% brew install autoconf
% brew install automake
% brew install libtool
~~~
On some system you will need to replace `sed` with the GNU version
~~~
% brew install gnu-sed
% sudo ln -s /usr/local/bin/gsed /usr/local/bin/sed
~~~
Verify that `PATH` finds `/usr/local/bin/sed` first:
~~~
% which sed
/usr/local/bin/sed
~~~
If not, change your `PATH` environment variable so it does.
~~~
% export PATH=/usr/local/bin:$PATH
~~~
3. Required libraries:
~~~
% brew install xerces-c
% brew install mysql-connector-c
% brew install armadillo
~~~

4. Download, autoreconf and customize:
~~~
% mkdir source
% cd source
% git clone https://github.com/dchassin/gridlabd .
% autoreconf -isf
% ./customize status
Enabled:
Disabled: rest comm control debug matlab network plc gismo transactive powernet blockchain
~~~
You can learn how to change which modules are enabled using `./customize help`. Then you can check the build configuration with the command:
~~~
% ./customize configure status
--enable-silent-rules
--prefix=/usr/local
--with-mysql=/usr/local
'CXXFLAGS=-w -O2'
'CFLAGS=-w -O2'
~~~
You can learn how to change the configuration using `./customize configure help`. Then you can configure using the customized configuration with the command
~~~
% ./customize configure
~~~

If using OS High Sierra (10.13) configure as: 

~~~
%./configure --prefix=/usr/local 'CXXFLAGS=-w -O2' --with-xerces=/usr/local --with-mysql=/usr/local --enable-silent-rules 'CFLAGS=-w -O2'
~~~

5. Build:
~~~
% make
~~~

Some possible issues you may encounter:

* If you used `brew` to install `xerces-c` and you find the `configure` doesn't find it, then you can use the `third_party/install_xerces` script to install `xerces-c` correctly.  Enter the command `(cd third_party ; . install_xerces)` when in your source folder.

* If you encounter an issue with the `fmin` definition in `platform.h` when used in `connection.cpp` you can replace `min(comma,semic)` with `(comma<semic?comma:semic)`.

* Some more recent MacOS users have encountered a problem with `powerflow/emissions`.  If you don't need the emissions, you can disable it by removing references to it in `powerflow/Makefile.mk` and `powerflow/init.cpp`.

6. Install and validate
~~~
% make install
% make validate
~~~