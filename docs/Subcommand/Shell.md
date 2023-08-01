[[/Subcommand/Shell]] -- Open GridLAB-D shell environment

# Synopsis

Shell:

~~~
sh$ gridlabd shell [OPTIONS ...]
~~~

# Description

The `shell` subcommand opens a shell in the GridLAB-D environment. The shell used is the one currently running as specified by the `SHELL` environment variable. This can be used to run Python with the active python `venv` used for GridLAB-D, or to create a new Python virtual environment in which gridlabd can be run. The options are the same as for the shell current being used.

The following environment variables are set in the shell environment:

- `CCFLAGS`: C-compiler flags
- `CFLAGS`: C-compiler flags
- `CPPFLAGS`: C++ compiler flags
- `CXXFLAGS`: C++ compiler flags
- `GLD_BIN`: binary folder
- `GLD_ETC`: shared files folder
- `GLD_INC`: include files folder
- `GLD_LIB`: library folder
- `GLD_VER`: version folder
- `GLPATH`: GridLAB-D file search path
- `INCLUDE`: C-compiler include flags
- `LDFLAGS`: linker flags
- `LIB`: linker library path
- `PYCCFLAGS`: python C-compiler flags
- `PYLDFLAGS`: Python linker flags
- `PYTHON_CONFIG` python config executable name
- `PYTHON_VER`: python version number (major.minor)
- `PYTHONPATH`: Python search path
- `VIRTUAL_ENV`: Python virtual environment path
- `VIRTUAL_ENV_PROMPT`: Python virtual environment prompt

# Example

The following commands open a GridLAB-D shell environment and create a Python virtual environment in which GridLAB-D is installed

~~~
gridlabd shell
python$PYTHON_VER -m venv myenv
. myenv/bin/activate
python3 -m pip install $GLD_ETC/*.whl
~~~

# See also

* [[/Subcommand/Python]]
