# Getting Started with GridLAB-D on Mac OS X

## Catalina

Coming soon...

## Mojave

To manually build GridLAB-D under Mojave you will need to install `git` if it
is not already installed. Then do the following

~~~
bash$ brew update || ruby -e "$(curl -fsSL https://raw.githubusercontent.com/HomeBrew/install/master/install)"
bash$ brew doctor
bash$ brew install autoconf automake libtool gnu-sed python3
bash$ pip3 install numpy pandas matplolib
bash$ ln -s /usr/local/bin/gsed /usr/local/bin/sed
bash$ export PATH=/usr/local/bin:/usr/bin:/bin
bash$ git clone https://github.com/dchassin/gridlabd gridlabd
bash$ cd gridlabd
bash$ autoreconf -isf
bash$ ./configure
bash$ make -j30 install
bash$ gridlabd --version
~~~

The last command should display something like
~~~
GridLAB-D 4.2.0-191010
~~~

At this point it is recommended that you validation your installation with the following command
~~~
bash$ gridlabd --validate
~~~
This process can take several minutes.

## Older versions of Mac OS Z

Building GridLAB-D with the Auto-Toolset Under Linux and Other Unix Variants
You must have autoconf, automake, libtool, and xerces-c installed to build 
GridLAB-D in this way. With newer versions of the mac os, you will also need 
gnu sed and have it symlinked to sed.  To install the necessary tools, we 
recommend using XCode and homebrew.

1. Install XCode

In the XCode menu, select Preferences.  Select the Downloads tab.  Select 
Command Line Tools and install.

2. Install homebrew
      % sudo ruby -e "$(curl -fsSL https://raw.github.com/mxcl/homebrew/go)"
      % sudo brew doctor

3. Install autoconf, automake, and libtool
      % sudo brew install autoconf
      % sudo brew install automake
      % sudo brew install libtool

The above commands should create symlinks in /usr/local/bin to each of these
tools.  On some combinations of XCode version and Mac OSX version, the
symlinks don't get created.  If this is the case, create them with
      % sudo brew ln --force autoconf
      % sudo brew ln --force automake
      % sudo brew ln --force libtool

4. Install gnu sed
      % sudo brew install gnu-sed

Symlink gsed to sed
      % sudo ln -s /usr/local/bin/gsed /usr/local/bin/sed

Make sure symlinked sed is the first instance of sed in the path
      % which sed

If /usr/local/bin/sed is not the response, put /usr/local/bin first in the path
you can do this temporarily using:
      % export PATH=/usr/local/bin:$PATH
or permanently by editing the file /etc/paths

5. Download the source code

If you are building from a source distribution you must first run:

      % git clone https://github.com/<user>/<repo> gridlabd
      % cd gridlabd
      % autoreconf -isf
      % ./configure

If you get a warning about XercesC, you will need to build and install XercesC:

      % (cd third_party; source install_xercesc)
      % ./configure

Once everything is setup you can build and install `gridlabd`:
      % make -j20 install

You can test your installation by using the `--validate` option:
      % gridlabd --validate
      
Whenever changes have been made to `configure.ac` or any of the `makefile.am`
files, the following build steps must be executed from the source-path folder:  
      % autoreconf -isf
      % ./configure
      % make -j20 install

Make should detect changes to `configure.ac` and `makefile.am` files and
automatically rerun the first two commands as needed.  If files are added to a module,
add the filename to `makefile.am` in that module's directory and remake. If `make`
does not automatically perform the required steps, rerun `autoreconf -isf` and
`./configure` manually.

If you checkout a branch that has different files, you may have to clean the files
to match a new distribution first:
      % make distclean -i
      % autoreconf -isf
      % ./configure
      % make -j20 install

To distribute the source code, it is best to fork the project and begin work from
your own GitHub repository.

On some Mac OS X systems, the GNU libtools are prefixed with a 'g' to differentiate them
from the NeXTStep libtool.  Before performing the steps above, you may have to tell the
autotool set where to find `libtool` and `libtoolize` by setting the LIBTOOL and
LIBTOOLIZE environment variables.  The following lines are an example:

    % export LIBTOOL=/usr/bin/glibtool
    % export LIBTOOLIZE=/usr/bin/glibtoolize
    % autoreconf -isf
 
For information on the sample models, see `models/README.md`.

On some very old version of Mac OS X you may run into a problem with `rpl_malloc` being undefined
during the build.  If so, set the following environment before running `./configure`
    % export ac_cv_func_malloc_0_nonnull=yes 


