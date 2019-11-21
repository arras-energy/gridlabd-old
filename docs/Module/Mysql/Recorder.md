[[/Module/Mysql/Recorder]] -- MySQL recorder class 

# Synopsis 
~~~
module mysql;
object recorder {
   property "<property-name>";
   trigger "<condition>";
   table|file "<source-table>";
   mode {"w","w+","a","a+"};
   limit "<max-rows>";
   interval "<seconds>";
   connection "<database-object-name>";
   options "[PURGE|UNITS|NOADD]";
   datetime_fieldname "t"; 
   recordid_fieldname "id"; 
   header_fieldnames ""; 
}
~~~

# Description 

The `mysql` recorder is designed to be compatible the `tape` recorder object so that when the `mysql` module is used in place of the tape module, there are few changes, if any, required to the recorder objects.

# Properties

## `connection`
~~~
   object connection;
~~~

Specifies the database object used to connect to MySQL.  If none is provided, the last database defined is used.

## `datetime_fieldname` 
~~~
   char32 datetime_fieldname;
~~~

Specifies the field name to use for the date-time field in stored records.  The default is "t". 

## `file` 
~~~
   char1024 file;
~~~

A synonym for table for compability with recorderrecorder file.

## `header_fieldnames` 
~~~
   char1024 header_fieldnames;
~~~

Specifies the header data to store in each record inserted.  The default is none (i.e., "").  Valid fields are "name", "class", "latitude", "longitude".  The values are comma-delimited.  Name is limited to 64 characters, class is limited to 32 characters, and latitude/longitude are doubles conforming to ISO Standard 6709. 

## `interval` 
~~~
   double interval[s];
~~~

Specifies the sampling interval.  By default the interval is specifies in seconds, but if units are specifies conversion will be automatic.

## `mode` 
~~~
   char32 mode;
~~~

Specifies the read mode to use, which may be either "w", "w+", "a", or "a+".  If "w" or "w+" is used, the table is purged before data from the current run is added. If "a" or "a+" is used, the data from the current run is appended.

## `options` 
~~~
   set {PURGE,UNITS,NOADD} options;
~~~

Specifies the recorder options, as follows:

### `PURGE` 

Purges the table of all existing data (by dropping and recreating the table) before storing new data.

### `UNITS` 

Adds the internal units to the field name when the property does not already specify the desired units.

### `NOADD`

Prevents the recorder from automatically adding columns that are missing.

## `property` 
~~~
   char1024 property;
~~~

Specifies the property (or properties) that are to be recorded.  The property name can include units, in which case conversion is automatically performed when the value is posted. 
Properties may also include object names, in which case the parent is ignored and the property is read from the specified object instead. 
Parts of complex properties can be referenced using the object.part syntax, and will be stored as object_part in the database. 

## `recordid_fieldname` 
~~~
   char32 recorderid_fieldname;
~~~

Specifies the record id field name to use when adding records to a table.  The default is "id". 

## `table` 
~~~
   char1024 table;
~~~

Specifies the source table from which data is read.

## `trigger`
~~~
   char32 trigger;
~~~

Specifies the triggering condition for starting the recording.  See recorderrecorder trigger for details.

# Caveats 

* The `limit` parameter is not enforced until at least one row is added. Therefore, it is likely that after a given run, the number of rows is increased by 1 even though the maximum number of rows has been reached.

# See also 

* [[/Module/Mysql]]
* [[/Module/Mysql/Database]]
* [[/Module/Tape/Recorder]]
