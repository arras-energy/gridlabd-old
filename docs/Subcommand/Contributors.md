[[/Subcommand/Contributors]] -- Get a list of contributors

# Synopsis

GLM:

~~~
#contributors [OPTIONS]
~~~

Shell:

~~~
sh% gridlabd contributors [OPTIONS]
~~~

# Description

The `contributors` subcommand obtains a list contributors to the latest version of GridLAB-D. Contributors are listed in descending order of the number of commits.

## Options

### `-h|--help`

Get the command line help list.

### `-k|--known`

Restrict output to github user with names.

### `-l|--login`

Output only the user login id.

### `-q|--quotes <str>`

Enclose names quote `<str>`.

### `-s|--separate <str>`

Separate names using `<str>`.

### `-t|--token <str>`

Use the GitHub access token `<str>`.

# Caveat

This query may be rate-limited by GitHub.  To increase the rate-limit use a github access token on the command line or add your github access token to the file `$HOME/.github/access-token`. Note, you may need to create the folder `./github` and file `access-token` within it that contains the github generated token. 

# See also

* [[/Command/Cite]]
