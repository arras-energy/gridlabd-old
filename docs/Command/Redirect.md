[[/Command/Redirect]] -- Redirects an output to stream to a file (or null)

# Synopsis

~~~
bash$ gridlabd --redirect <stream>[:<file>]                            
~~~

# Description

Redirects an output to stream to a file (or null). Valid streams are `output`, `error`, `warning`, `debug`, `verbose`, `profile`, and `progress`.  The default filenames are `gridlabd.{out,err,wrn,dbg,inf,pro,prg}`, respectively.

# Examples 

~~~
bash$ gridlabd --redirect all
~~~
This will redirect all streams to gridlabd.* files
