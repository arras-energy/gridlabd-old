[[/Subcommand/Shell]] -- Open GridLAB-D shell environment

# Synopsis

Shell:

~~~
sh$ gridlabd shell
~~~

# Description

The `shell` subcommand opens a shell in the GridLAB-D environment. This can be used to run Python with the active python `venv` used for GridLAB-D.

The following environment variables are set in the shell environment:

- `CCFLAGS`: C-compiler flags
- `CFLAGS`: C-compiler flags
- `CPPFLAGS`: C++ compiler flags
- `CXXFLAGS`: C++ compiler flags
- `GLD_BIN`: binary folder
- `GLD_DOC`: documentation folder
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

# See also

* [[/Subcommand/Python]]
