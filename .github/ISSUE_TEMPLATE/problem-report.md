---
name: Problem report
about: Report a problem for us to resolve
title: "(Provide a one-line synopsis of the problem)"
labels: bug
assignees: dchassin

---

## Problem description

<<<Write a clear and concise description of what the bug is.>>>

## Steps to reproduce

<<<Explain how to reproduce the behavior, e.g.,>>>
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

## Expected behavior

<<<Provide a clear and concise description of what you expected to happen.>>>

## System information

* GridLAB-D version: <<<Insert output of `gridlabd --version=all` here>>>
* Operating system: <<<Insert output of `uname -a` here>>>

## Supporting information

<<<Add any other context about the problem here.>>>

## Attachments

<<<If applicable, attach screenshots, supporting files, and other data need to help explain and reproduce the problem.>>>

<<<If your encountered a program fault or internal error, please attach the output of the following command:>>>
~~~
bash$ gridlabd gdb all your command line arguments go here...
...alot of output, including the fault message...
(gdb) where
(gdb) list
~~~

<<<If you are running on macOS, you must use `lldb` instead of `gdb` and use the `frame` command instead of `where`.>>>
