Software developers and engineers are welcome to contribute to HiPAS GridLAB-D. The source code to HiPAS is available to the public through the [GitHub SLAC Gismo GridLAB-D project](https://github.com/hipas/gridlabd). 

## Developer Resources

  - [Source code](https://github.com/hipas/gridlabd)
  - [Issues](https://github.com/hipas/gridlabd/issues)
  - [Pull requests](https://github.com/hipas/gridlabd/pulls)
  - [Projects](https://github.com/hipas/gridlabd/projects)
  - [Wiki](https://github.com/hipas/gridlabd)
  - [Team Discussions](https://github.com/hipas/teams/gridlab-d/discussions)

Developers should consult the [[/Developer/README]] for information about modifying and contributing to HiPAS GridLAB-D.

# Installing HiPAS GridLAB-D from source code

You can set up your system to host development and build GridLAB-D from source code using the installation script. 

~~~
bash$ git clone https://github.com/hipas/gridlabd
bash$ cd gridlabd
bash$ ./install.sh
~~~

This script supports the most common Linux/Unix platforms.  If your platform is not supported, please consult the developer guide at [[/Developer/README]].

# Validating a HiPAS GridLAB-D installation

Once you have completed installation, you should validate your system before beginning software development.

~~~
bash$ gridlabd --validate
~~~

If you observe any validation failures, please consult the [[/Developer/README]] for further guidance.
