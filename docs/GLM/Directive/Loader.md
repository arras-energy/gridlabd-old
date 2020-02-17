[[/GLM/Directive/Extension]] -- Add GLM parser extensions to the GLM loader

# Synopsis

GLM:

~~~
  extension <name>;
~~~

# Description

The GridLAB-D GLM loader can be extended at runtime using the `extension` directive.  This loads a runtime library by that name, which is called a *loader extension module*.  The runtime library can be installed in the local working folder, or in one of the system library folders in `LD_LIBRARY_PATH` (on Linux) or `DYLD_LIBRARY_PATH` (on Darwin).

# Implementation

The following function must be exported by a loader extension module.  

## int init(void)

The initialization function is called when the loader extension module is first loaded into the system.  This gives the new loader extension the opportunity to prepare the module to receive call to the <tt>parser</tt> function.

The return value is 0 on success, and non-zero on failure.  The non-zero record codes correspond to the gridlabd exit codes.

## int parser(const char *text)

The GLM load will call parser whenever it encounters a top-level directive that is not recognized.  If the loader extension can parse the text, it must return the number of characters it recognizes.  Any partially recognized text can result in error messages if appropriate.

If the loader extension cannot parse the text, it should not issue any errors or warning, and simply return 0. Negative returns values have an undefined affect on the GLM parser.

## int term(int code)

If exported, this function is called when a gridlabd main instance is terminated.

## int on_init(TIMESTAMP t0)

If exported, this function is called when a gridlabd main execution loop is started.

## int on_term(TIMESTAMP tn)

If exported, this function is called when a gridlabd main execution loop has ended.

# Example

The file `hello.c` implements the initiatization and parser:

~~~
  int init(void)
  {
    printf("initialization ok");
    return 0;
  }
  int parser(const char *text)
  {
    if ( strncmp(text,"hello",5) == 0 )
    {
      return 5;
    }
    else
    {
      return 0;
    }
  }
~~~

The file `hello.glm` calls the parser when the term `hello` is encountered:

~~~
extension hello;
hello;
~~~

# Caveat

On many linux platforms, you must change the `LD_LIBRARY_PATH` environment variable to include the folder in which the `.so` file is placed.  If you are building the loader extension at runtime, then you will have to include the command `LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}` *before* running the model in the environment in which `gridlabd` is started. Including this using `#setenv LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}` is not secure and will not work as expected.
