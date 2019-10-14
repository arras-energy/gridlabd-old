# Manual GridLAB-D Installation on Linux

Because there are different variants of Linux, these instructions serve 
only as a general guide to installation on Linux platforms. The specifics
for any given platform may vary. System setup scripts for some Linux system are
available in the `build-aux` folder.

In general you must do the following to prepare your system

1. Install `autoconf`, `automake`, `libtool`, and `python3`.

2. Install the `matplotlib`, and `pandas` modules for `python3`.

3. If you are using the `mysql` module, you must install `mysql-connector-c`.

4. If you want to generate the documentation, you must install `doxgen`, 
`mono`, and `Naturals Docs`.

Once you have prepared your system, you can run the standard build process:
~~~
bash$ git clone https://github.com/<user>/<repo> gridlabd
bash$ autoreconf -isf
bash$ ./configure
bash$ make -j20 install
bash$ gridlabd --validate
~~~

To run gridlabd from a shell on your system we recommend you add the following
to your shell profile to allow runtime code to work in your models:
~~~
export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATHL:-/tmp}
~~~

