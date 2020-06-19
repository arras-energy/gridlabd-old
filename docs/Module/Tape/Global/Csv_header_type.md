[[/Module/Tape/Global/Csv_header_type]] -- Module tape global variable to control CSV header formatting

# Synopsis

Shell:

~~~
bash$ gridlabd -D|--define tape.csv_header_type={FULL,NONE,NAME}
~~~

GLM:

~~~
  module tape {
    csv_header_type "{FULL|NONE|NAME}";
  }
~~~

# Description

The value of `csv_header_type` determines the type of header included in CSV output from the `tape` module. Three values are recognized:

- FULL: output the full GridLAB-D CSV file comment header

- NONE: output only the CSV data with no header

- NAME: output a single CSV header line with field names before outputting the CSV data

# Caveat

This global `tape::csv_data_only` is synonymous with the global `tape::csv_data_only`, except that `csv_data_only` is represented as an enumeration. See [[/Module/Tape/Global/csv_data_only]] for details.

# See also

* [[/Module/Tape]]
* [[/Module/Tape/Global/Csv_data_only]]
