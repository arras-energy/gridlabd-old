[[/GLM/Macro/Version]] -- Version check macro

# Synopsis
GLM:
~~~
#version [-{lt,le,eq,ge,gt,ne}] {<major>.<minor>.<patch>,<number>,<branch>} ...
~~~

# Description

The `#version` macro checks the version number. If the version that is running does not satisfy the criteria given by the macro, the load fails with the error
~~~
ERROR [INIT]: version <current-version-info> does not satisfy the requirement <given-version-info>
~~~

Three aspects of the version can be checked. They are the version number, e.g., `4.2.1`, the build number, e.g., `191226`, or the branch name, e.g., `develop`.  The check can require the current version be strictly less that (`-lt`), less than or equal (`-le`), equal (`-eq`), greater than or equal (`-ge`), strictly greater than (`gt`), or not equal (`-ne`).  If omitted the test is `-eq`.  

Multiple tests may be specified, in which case the tests conjunction is the `or` operation, i.e., either one may be true for the test to succeed. To achieve an `and` operation, simply use the macro multiple times.

# Example
The following example checks that the version $\ge 4.2.1$ or the build number is $\gt 191225$ and the branch is $develop$.
~~~
#version -ge 4.2.1 -gt 191225
#version develop
~~~

# See also
* [[/Global/Version_major]]
* [[/Global/Version_minor]]
* [[/Global/Version_patch]]
* [[/Global/Version_build]]
* [[/Global/Version_branch]]
