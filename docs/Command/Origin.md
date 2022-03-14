[[/Command/Origin]] -- comnmand line option to display the build origin information

# Synopsis

~~~
bash$ gridlabd --origin
~~~

# Description

The `--origin` command line option displays information about the source code from which the currently installed version of GridLAB-D is built.  The purpose of this option is to facilitate troubleshooting when a system has multiple installations with local changes. The information includes the following data collected at the time of the build
- Output from the command `git -remote -v`
- Output from the command `git log -n 1`
- Output from the command `git status -s`
- Output from the command `git diff`

# Example

The following is the output for a clean build of a branch

~~~
bash$ gridlabd --origin
# https://github.com/slacgismo/gridlabd/commits/d2fca77b4985e28452e72e259ce6bf77d2d454b1
~~~

The following example includes diff output resulting from local changes to the code

~~~
# https://github.com/slacgismo/gridlabd/commits/ff655ab92b650b2373d02b44bf884334d40faf06
#  M source/Makefile.mk
#  M utilities/build_number
# ?? utilities/update_origin.sh
diff --git a/source/Makefile.mk b/source/Makefile.mk
index 45a36610..b2e627ed 100644
--- a/source/Makefile.mk
+++ b/source/Makefile.mk
@@ -155,4 +155,4 @@ source/build.h: buildnum
 
 buildnum: utilities/build_number
 	/bin/bash -c "source $(top_srcdir)/utilities/build_number $(top_srcdir) source/build.h"
-	(git remote -v ; git log -n 1 ; git status -s ; git diff ) > origin.txt
+	/bin/bash -c "source utilities/update_origin.sh" > origin.txt
diff --git a/utilities/build_number b/utilities/build_number
index cc6f1281..ead1fb0e 100755
--- a/utilities/build_number
+++ b/utilities/build_number
@@ -24,4 +24,3 @@ if [ "${LAST_STAMP:-none}" != "$THIS_STAMP" ]; then
 	echo "#define BUILD_STATUS $(git status -b --porcelain | sed -e 's/^/\"/g;s/$/\\n\"\\/g';echo \"\")" >> $2
 	cd -
 fi
-#ls -l $2
~~~

# See also

* [[/Command/Version]]
* [[/Subcommand/Version]]
