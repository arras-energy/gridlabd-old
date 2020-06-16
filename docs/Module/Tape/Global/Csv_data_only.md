[[/Module/Tape/Global/Csv_data_only]] -- Tape module global variable to control CSV data formatting

# Synopsis

Shell:

~~~
bash$ gridlabd -D|--define csv_data_only={0,1,2}
~~~

GLM:

~~~
  #set csv_data_only={0,1,2}
~~~

# Description

The value of `csv_data_only` determine whether the legacy comments will be included in CSV output from the `tape` module. Three values are recognized:

- 0: this value output the full GridLAB-D CSV file comment header

- 1: this value outputs only the CSV data with no header

- 2: this value outputs a single CSV header line with field names before outputting the CSV data

# Caveat

This value is synonymous with the global `tape::csv_header_type`, except that this value is represented as the underlying integer. It is maintained for compatibility with previous version of GridLAB-D where this value could only be 0 or 1. See [[/Module/Tape/Global/Csv_header_type]] for details.

# See also

* [[/Module/Tape]]
* [[/Module/Tape/Global/Csv_header_type]]
