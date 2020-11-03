[[/Subcommand/Openfido]] -- OpenFIDO subcommand

# Synopsis

Shell commands:
~~~
host$ gridlabd openfido [OPTIONS] help
host$ gridlabd openfido [OPTIONS] index [PATTERN]
host$ gridlabd openfido [OPTIONS] info NAME
host$ gridlabd openfido [OPTIONS] install [-d|--dry-run] NAME [...]
host$ gridlabd openfido [OPTIONS] remove [-d|--dry-run] NAME [...]
host$ gridlabd openfido [OPTIONS] run NAME INPUTFILES OUTPUTFILES
host$ gridlabd openfido [OPTIONS] update [-d|--dry-run] NAME [...]
~~~

# Description

The `openfido` subcommand provide direct access to the OpenFIDO product API.  OpenFIDO products provide
data processing libraries using pipelines and workflows.  A pipeline is a data processing unit that accepts
one of more input files and produces one or more output files.  A workflow is network of pipelines, and also
accepts one of more input files and produces one or more output files.

## Options

## Patterns

## Input files

## Output files

# Example

The following command installs the `filestat` pipeline from the OpenFIDO library.

~~~
host% gridlabd openfido install filestat
~~~

The following command runs the `filestat` pipeline to output a CSV file contain the file status of the 
listed input files.

~~~
host% gridlabd openfido run filestat file1,file2,file3 files.csv
~~~

# See also

* [[/GLM/Macro/Subcommand]]
