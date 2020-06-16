[[/Module/Tape/Global/Csv_header_type]] -- Module tape global variable to control CSV header formatting

# Synopsis

Shell:

~~~
bash$ gridlabd -D|--define csv_header_type={FULL,NONE,NAME}
~~~

GLM:

~~~
  #set csv_header_type={FULL,NONE,NAME}
~~~

# Description

The value of `csv_header_type` determine the type of header included in CSV output from the `tape` module. Three values are recognized:

- FULL: this value output the full GridLAB-D CSV file comment header

- NONE: this value outputs only the CSV data with no header

- NAME: this value outputs a single CSV header line with field names before outputting the CSV data

# Caveat

This value is synonymous with the global `tape::csv_data_only`, except that this value is represented as an enumeration. See [[/Module/Tape/Global/csv_data_only]] for details.

# See also

* [[/Module/Tape]]
* [[/Module/Tape/Global/Csv_data_only]]
