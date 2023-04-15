This page focuses on the GLM file format. A GLM file is used to synthesize a population of objects. This is in contrast to an JSON file, which is used to represent a specific instance of a population that has been synthesized. Consider the difference between the following GLM file that define a house object as

~~~
object house 
{
    floorarea random.normal(2000,500) sf;
}
~~~

meaning that a single house is to be defined, and the `floorarea` is to be normally distributed with a mean of `2000 sf`, and a standard deviation of `500 sf`.

In contrast, the relevant section of the output JSON might look like

~~~
{
    "objects" : {
        "house:0" : {
            "id" : 0,
            "class" : "house",
            "floor_area" : "2833.32 sf",
        }
    }
}
~~~

meaning that house id 0 has a floorarea of `1853.56 sf`. Notice that the JSON no longer contains any information about the distribution. However, if we were to generate a large population of houses, either by running GridLAB-D many times with this GLMhouse definition, or by defining a large number of houses in a GLM single model, we could observe that the normal distribution specified in GLM file is implicit in the JSON file that is output.

# Comments

Comments begin with a // sign. All text between the // and the end of the line is ignored by the parser.

It is important to note that the parser in GridLAB-D is quite primitive and may in certain circumstance be confused by // appearing in other contexts, such as a URL not enclosed in a quotes.

#Macros

The GLM loader allows the use of macros to control the behavior of the parser and to a limited extent also the behavior of GridLAB-D. Macros are lines that begin with a '#' sign. The following macros are available

- `#define name=value` is used to define a global variable. This allows the creation of a new global variable, in contrast to the `#set` macro which requires the global variable already exist.

- `#set name=value` is used to set a global variable. 

- `#undef` is used to remove the definition of a global variable.

- `#ifdef|ifndef name` is used to conditionally process a block of text in the GLM. For `#ifdef` and `#ifndef` the expression is simply the name of a global variable. When `#if `is used, the expression is a conditional test in the form of name op value where the operator op is one of `<`, `>`, `<=`, `>=`, `==`, or `!=`.

- `#include` file is used to include text from file in the loaded text.

- `#print` name is used to display the value of the global variable *name* at the moment it is encountered by the loader.

- `#ifexist "file"` is used to determine whether a file exists

- `#include "file"` is used to include another file during the parser load. Variants are `#include using(name=value,...) "file"`, `#include <file>`, `#include [file]`, and `#include (file)` to include a file after setting globals, including a library file, including a URL download, including the output of a shell script, respectively.

- `#insert file(name=value,...)` is used to include a file while setting globals as listed.  This is equivalent to `#include using(name=value,...) "file"`.

- `#setenv name=string` is used to set an environment variable

- `#error message` triggers a parser error condition

- `#warning message` triggers a parser warning condition

- `#option command-option` applied the command line option

- `#system command` makes an operating system call and waits for completion before continuing processing the GLM file

- `#start command` makes an operating system call and continues processing the GLM file

- `#output "filename.ext" options...` generates an output file of the type specified by the extension using the appropriate file converter

- `#input "filename.ext" options...` loads an input file of the type specified by the extension using the appropriate file converter.

- `#verbose message` outputs a verbose message.

- `#debug message` outputs a debug message.

- `#command subcommand options` executes the specified subcommand and continues only if the status is ok.

- `#exec command options` executes the specified subcommand and continues only if the status is ok. 

- `#gridlabd options` runs another instance of GridLAB-D.

- `#wget URL filename.ext` or `#curl URL filename.ext` downloads data from the URL and saves it to the specified file

- `#sleep msec` pauses the loader for the specified time in milliseconds.

- `#version spec` verifies the version of GridLAB-D.

- `#on_exit status command options ...` executes the command when the exit status matches.

- `#begin language` changes the interpreter to the specified language.

- `#for var in values ...` runs a GLM loop

- `#write filename.ext options ...` writes the model to the specified file

- `#read filename.ext options ...` read the specified file into the model

- `#subcommand options ...` runs the specified subcommand

# Modules

Static modules must be loaded before the classes they define can be used or modified. To load a static module, use `module name;`. The module loader will search the `GLPATH` environment variable to locate a file named name.dll (on linux the search will be for `libname.so`). 

Module blocks may include additional information, such as assignments of the values for module globals and specification of the version number. To set a module global variable, simply include the name and value in the module block, such as

~~~
module MyModule 
{ 
    MyStringGlobal "value";
    MyEnumGlobal A;
    MyDoubleGlobal 1.2 ft/s; 
}
~~~

To verify the module's version information, simply include the desired version in the module block:

~~~
module MyModule 
{
    major 4;
    minor 2; 
}
~~~

If the version loaded does not match the version specified, an error will be displayed and the loader will stop.

# Classes

Class blocks are used to create, modify, or verify class definitions. If a class is already defined in a static module, then a class block either modifies or verifies the definition provided by the module. Consider the following example

~~~
module MyModule;
class MyClass 
{
    char32 svalue;
    enum {A=0,B=1,C=2} evalue; 
    double dvalue[W];
}
~~~

If the properties `svalue`, `evalue`, and `dvalue` are already defined as specified, the class block will load successfully. However, if there are any differences between the class block and the module's definition of the class, then the loader will attempt to address the discrepancy as follows:

1. If the class defines a property differently than the module, then the loader will fail.

2. If the class defines a property that the module does not define, then the loader will extend the module's definition of the class to include this new property.

3. If the class is not defined by any loaded module, then the class is defined as a new class, and the properties are added to that new class. In this case, you may also include C++ code for the behaviors that static modules normally provide. See below for more information on runtime classes.

# Runtime Classes

If a class block is defined, and that class is not already implemented by an existing module, then you may define class behavior for each of the supported behaviors in GridLAB-D. Such classes are called runtime classes can be written in Python or C++. 

Use of the Python and C++ compilers is automatic and does not need to be explicitly configured. However, if you have installed GridLAB-D in an unusual location, you may have to set the global variable `INCLUDE` to tell C++ where the `rt/gridlabd.h` file is located, and the `pythonpath` global variable to tell Python where to find the Python module. You may also want to add the location of any files that the `#include <file.h>` macros are expected to find.

## Python Runtime Classes

There are two elements to creating a Python runtime class.  First, you must create and load your Python module, e.g.

File `example.py`:

~~~
import sys
import random
def example_sync(name,time):
    x0 = float(gridlabd.get_value(name,'x'))
    x1 = x0 + random.uniform(0,1)
    gridlabd.set_value(name,'x',str(x1))
    result = time + 3600
    print(f"{__name__}.example_sync(name='{name}',time={time}): x = {x1}, result = {result}",file=sys.stderr)
    return int(result)
~~~

Then you must link your module to the runtime class, e.g.,

File `example.glm`:

~~~
clock
{
    starttime "2020-01-01 00:00:00";
    stoptime "2020-01-02 00:00:00";
}
module example;
class example
{
    on_sync "python:example.example_sync";
    double x;
}
object example
{
    x 0.0;
}
~~~

Running this yields the following result

~~~
% gridlabd example.glm
example.example_sync(name='example:0',time=1577865600): x = 0.8056947368062789, result = 1577869200
example.example_sync(name='example:0',time=1577869200): x = 1.4092846363477993, result = 1577872800
example.example_sync(name='example:0',time=1577872800): x = 2.300014263598458, result = 1577876400
...
example.example_sync(name='example:0',time=1577944800): x = 13.19907449956815, result = 1577948400
example.example_sync(name='example:0',time=1577948400): x = 13.886053306757786, result = 1577952000
example.example_sync(name='example:0',time=1577952000): x = 13.887313687673041, result = 1577955600
~~~

The following Python event handlers are supported

- Module level
  - `on_create()`
  - `on_init(timestamp)`
  - `on_precommit(timestamp)`
  - `on_presync(timestamp)`
  - `on_sync(timestamp)`
  - `on_postsync(timestamp)`
  - `on_commit(timestamp)`
  - `on_finalize(timestamp)`
  - `on_term(timestamp)`
- Class level
  - `on_init(name,timestamp)`
  - `on_precommit(name,timestamp)`
  - `on_presync(name,timestamp)`
  - `on_sync(name,timestamp)`
  - `on_postsync(name,timestamp)`
  - `on_commit(name,timestamp)`
  - `on_finalize(name,timestamp)`
- Object level
  - `on_init(name,timestamp)`
  - `on_precommit(name,timestamp)`
  - `on_presync(name,timestamp)`
  - `on_sync(name,timestamp)`
  - `on_postsync(name,timestamp)`
  - `on_commit(name,timestamp)`
  - `on_finalize(name,timestamp)`

In addition, the `script` directive support python calls.

## C++ Runtime Classes

The supported C++ behaviors for runtime classes include:

- `create (object <parent>) { ... return [SUCCESS|FAILED];};` is used to define the object creation behavior other than the default behavior (which is to set the entire object's memory buffer to 0). Usually parent is not defined at this point. Object creation functions are called when the object's memory is allocated, while initialization isn't performed until after all objects have been created and the parent/child hierarchy established.

- `init (object <parent>) { ... return [SUCCESS|FAILED];};` is used to define object initialization behavior, which is delayed until all objects have been created and their properties set as defined in the model file. Usually initialization is a good time to adjust dependent properties and/or check for inconsistencies in the values provided.

- `[presync|sync|postsync] (TIMESTAMP t0, TIMESTAMP t1) { TIMESTAMP t2 = TS_NEVER; ... return t2;};` is used to implement presync, sync, and postsync behavior. Presyncs are called on the first top-down pass, syncs are called on the bottom-up pass, and postsyncs are called on the last top-down pass. The top-down/bottom-up order in which objects are evaluated is based on their ranks. Rank is determined primarily by the parent-child relationship, however calls to gl_set_rank() can be used to promote the rank of an object arbitrarily with respect to another object. (Objects cannot be demoted.)

- `plc (TIMESTAMP t0, TIMESTAMP t1) {TIMESTAMP t2 = TS_NEVER; ... return t2;};` is used to define the default programmable logic controller (PLC) behavior. This behavior is overridden if a plc refers to this object as its parent.

For more information on the compilers for runtime classes, see the page on Runtime compiler support.

## Exporting functions from runtime classes

You can publish a function from a runtime class by using the following syntax.

~~~
class example1 
{
    export myfunction(void *arg1, ...) 
    {
        // your code goes here
        return result; // int64 value is returned
    };
}
~~~
and access that function from another class using the syntax

~~~
class example2 
{
    function example1::myfunction;
    intrinsic sync(TIMESTAMP t0, TIMESTAMP t1)
    {
        // your code goes here
        int64 result = myfunction(arg1, arg2); return t2;
    };
}
~~~

## Debugging Runtime Classes

To debug runtime class behavior, you must install gdb on your system. See the [[/Command/Gdb]] or [[/Command/Lldb]] options.

# Objects