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

<<<If your encountered a program fault or internal error, and you have built GridLAB-D with `-O0 -g`, then on Linux systems you can attach the output of the following command:>>>
~~~
bash$ ulimit -c unlimited
bash$ gridlabd <your command options...>
... crash dumps core ...
bash$ gdb -batch -ex "backtrace" $(gridlabd --version=install) ./core
~~~

<<<If you are running on macOS, you must use `lldb` instead of `gdb` and use the `frame` command instead of `where`.>>>
