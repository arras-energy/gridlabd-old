[[/Developer/Module/Class/Method]] -- Method call implementation

# Synopsis

GLM:

~~~
  class my_class {
    method my_method;
  }
  object my_class {
    my_method "value";
  }
~~~

C++ Header:

~~~
  DECL_METHOD(my_class,my_method)
  class my_class : public gld_object {
  public:
    GL_METHOD(my_class,my_method);
  };
~~~

C++ Implementation:

~~~
  #include "my_class.h"
  EXPORT_METHOD(my_class,my_method);
  int my_class::my_method(char *buffer, size_t len)
  {
  	if ( buffer == NULL )
  	{
  		if ( len == 0 )
  		{
  			// return length of result only
  	  	}
  	  	else
  	  	{
  	  	  	// return non-zero if len > length of result
  	  	}
  	}
  	else if ( len == 0 )
  	{
  		// return number of characters read from buffer
  	}
  	else
  	{
  		// return number of characters written to buffer
  	}
  }
~~~

# Description

The property type `PT_method` allows object properties to be accessed using *method* calls.  Methods allow objects to receive incoming messages as a `set` operation through the property implementation and route the message directly to a function to handle the message in the class implementation. Methods can also be used to retrieve data using a `get` operation.

To implement a method in a GridLAB-D class you must do the following.

# Example

The following example illustrates how to add a method handler to a GridLAB-D class.

## Class Header File

1. Add `DECL_METHOD(class,name)` to the header file.
2. Add `GL_METHOD(class,name)` entry to the class declaration.

Example:

~~~
  DECL_METHOD(my_class,my_method)
  class my_class : public gld_object {
  public:
    GL_METHOD(my_class,my_method);
    my_class(MODULE *);
    int init(OBJECT *);
    static CLASS *oclass;
    static my_class *default;
  };
~~~

## Class Implementation file

1. Add `EXPORT_METHOD(class,name);` to the implementation file.
2. Add `PT_method, "name", get_name_offset(), PT_DESCRIPTION, "description",` to the `gl_publish_variable()` call.
3. Implement `int class::name(char *buffer, size_t len) {...}`.

Example:

~~~
  #include "my_class.h"
  EXPORT_CREATE(my_class);
  EXPORT_INIT(my_class);
  EXPORT_METHOD(my_class,my_method);
  CLASS *my_class::oclass = NULL;
  my_class *my_class::defaults = NULL;
  
  my_class::my_class(MODULE *module)
  {
  	if (oclass==NULL)
  	{
  		// register to receive notice for first top down. bottom up, and second top down synchronizations
  		oclass = gld_class::create(module,"my_class",sizeof(my_class),PC_AUTOLOCK|PC_OBSERVER);
  		if (oclass==NULL)
  			throw "unable to register class my_class";
  		else
  			oclass->trl = TRL_CONCEPT;
  		defaults = this;
  		if ( gl_publish_variable(oclass,
  				PT_method, "my_method", get_my_method_offset(), PT_DESCRIPTION, "my_method description",
  				NULL) < 1 )
  			throw "unable to publish my_class properties";
  		// TODO default defaults
  	}
  }
  
  int my_class::create(void)
  {
  	// set default values
  	return 1; /* return 1 on success, 0 on failure */
  }
  
  int my_class::init(OBJECT *parent)
  {
  	// check values set by input model
  	return 1; /* return 1 on success, 2 on defer, 0 on failure */
  }
  
  int my_class::my_method(char *buffer, size_t len)
  {
  	if ( buffer == NULL )
  	{
  		if ( len == 0 )
  		{
  			// return length of result only
  	  }
  	  else
  	  {
  	   	// return non-zero if len > length of result
  	  }
  	}
  	else if ( len == 0 )
  	{
  		// return number of characters read from buffer
  	}
  	else
  	{
  		// return number of characters written to buffer
  	}
  }
~~~

# See also

* [[/Developer/Module/Class]]
