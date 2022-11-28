[[/GLM/General/Expansion]] -- Property value expansion

# Synopsis

GLM:

~~~
object <class>
{
	name `expression`;
}
~~~

# Description

Expansion variables are used in GLM code to embed context-dependent values into property values and code. Expansion variables are always enclosed in single curly braces, e.g., `{variable}`. To enable expansion variable syntax, property values must be embedded in back-quotes

The recognized expansion variables are

- `{class}` embeds the class name
- `{cpu}` embeds the CPU id (see pstatus) that is running the code Template
- `{fileext}` embeds the file extension (no path or name)
- `{filename}` embeds the base file name (no path and no extension)
- `{filepath}` embeds the path to the file (no file name or extension)
- `{file}` embeds the full GLM file name
- `{global}` embeds the value from a global variable Template
- `{gridlabd}` embeds the path to the gridlabd installation
- `{hostaddr}` embeds the host IP address of the machine running the code Template
- `{hostname}` embeds the host name of the machine running the code Template
- `{id}` embeds the object id number
- `{masteraddr}` embeds the master server simulation host IP address Template
- `{mastername}` embeds the master server simulation host name Template
- `{masterport}` embeds the master server port number Template
- `{namespace}` embeds the namespace
- `{parent}` The name of the parent object
- `{pid}` embds the process id (see pstatus that is running the code Template
- `{port}` embeds the port number of the server (if any) Template
- `{property}` embeds the property from the current object

# Example

~~~
module residential;
object house:..5 {
	name `house_{id}`;
}
~~~
