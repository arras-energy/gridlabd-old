[[/Module/Tape]] -- Module tape

# Synopsis

GLM:

~~~
  module tape {
    gnuplot_path "<string>";
    flush_interval "<integer>";
    csv_data_only "<integer>";
    csv_keep_clean "<integer>";
    csv_header_type "<string>";
    delta_mode_needed "<string>";
  }
~~~

# Description

TODO

## Globals

### `gnuplot_path`

~~~
  gnuplot_path "<string>";
~~~

TODO

### `flush_interval`

~~~
  flush_interval "<integer>";
~~~

TODO

### `csv_data_only`

~~~
  csv_data_only "<integer>";
~~~

TODO

### `csv_keep_clean`

~~~
  csv_keep_clean "<integer>";
~~~

TODO

### `csv_header_type`

~~~
  csv_header_type "<string>";
~~~

The value of `csv_header_type` determines the type of header included in CSV output from the `tape` module. Three values are recognized:

 - FULL: output the full GridLAB-D CSV file comment header

 - NONE: output only the CSV data with no header

 - NAME: output a single CSV header line with field names before outputting the CSV data
 
 [[/Module/Tape/Global/Csv_header_type]]

### `delta_mode_needed`

~~~
  delta_mode_needed "<string>";
~~~

TODO

# See also

* [[/Module/Tape/Player]]
* [[/Module/Tape/Shaper]]
* [[/Module/Tape/Recorder]]
* [[/Module/Tape/Multi_recorder]]
* [[/Module/Tape/Collector]]
* [[/Module/Tape/Histogram]]
* [[/Module/Tape/Group_recorder]]
* [[/Module/Tape/Violation_recorder]]
* [[/Module/Tape/Metrics_collector]]
* [[/Module/Tape/Metrics_collector_writer]]

