[[/Subcommand/Json-get]] -- Extract values from JSON data

# Synopsis

GLM:

~~~
#json-get [keys ...] [-k|--keys] [-j|--json|-r|--raw|-c|--csv] [-i|--input INPUT]
 [-o|--output OUTPUT]
~~~

Shell:

~~~
$ gridlabd json-get [keys ...] [-k|--keys] [-j|--json|-r|--raw|-r|--csv] [-i|--input INPUT]
 [-o|--output OUTPUT] 
~~~

# Description

The `json-get` subcommand extract elements from JSON data.  The `keys` are used to specify which elements are extracted.  Multiple keys will descend the JSON hierarchy recursively extracting data elements.

By default the data is extracted in JSON format.  If `-r` or `--raw` is specified, the data will be extract in raw Python `str` format.  If `-c` or `--csv` is used, the data will be extract as comma-separated values. 

# Caveat

Not all data structures will format well as CSV.  In particular, nested dictionaries and lists may not yield valid or useful CSV data.  In general CSV formats are best suited for unnested dictionaries, e.g., `{"1":"a","2":"b","3":"c"}` yields

~~~
1,a
2,b
3,c
~~~

# Example

~~~
$ echo '{"id":"1","data":{"name":"test","value":"example"}}' | gridlabd json-get id
1
PC98798:gridlabd dchassin$ echo '{"id":"1","data":{"name":"test","value":"example"}}' | gridlabd json-get data
{
    "name": "test",
    "value": "example"
}
$ echo '{"id":"1","data":{"name":"test","value":"example"}}' | gridlabd json-get data name
test
$ echo '{"id":"1","data":{"name":"test","value":"example"}}' | gridlabd json-get data value
example
 echo '{"id":"1","data":{"name":"test","value":"example"}}' | gridlabd json-get data --csv
name,test
value,example
$ 
~~~
