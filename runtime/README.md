# Runtime

This `runtime` folder contains all the files needed by GridLAB-D at runtime. To add a new file, you must list it in the `Makefile.mk` file by adding a line such as

~~~
dist_pkgdata_DATA += runtime/FILENAME.EXT
~~~
