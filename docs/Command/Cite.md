[[/Command/Cite]] -- command line option to obtain the formal GridLAB-D citation of the current version

# Synopsis

~~~
bash$ gridlabd --cite[=FORMAT]
~~~

# Description

The `--cite` command line option is used to obtain the plain text citation for the current version of GridLAB-D. See the project README file for details on how to correctly cite GridLAB-D.

Supported formats are `json` and `bibtex`.

# Example

The following example prints the citation text for the version of GridLAB-D 4.2 built on October 9, 2019.

~~~
bash$ gridlabd --cite
Chassin, D.P., et al., "GridLAB-D 4.2.0-191009 (master) Darwin" (2019) [online]. Available at https://source.gridlabd.us/. Accessed Oct. 9, 2019
~~~

# See also

* [[/Command/Origin]]
* [[/Command/Version]]
