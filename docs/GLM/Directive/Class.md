[[/GLM/Directive/Class]] -- Class definition

# Synopsis

~~~
class
{
    <type> <name>; 
    <type> <name>[<units>];
    <type> <specification> <name>; 
    <event> "<script>";
    <event> "python:<module>.<name>";
}
~~~

# Description

Types can be verified or added to a class using the `<type> ...` declaration.  If the type is already defined, then the declaration can serve as a verification of the previous declaration.  If the declarations differs, the loader will fail. If the type is not already defined, the declaration will add a new property to the class.

Event handlers can be defined in the same manner as for individual objects. When an event handler is defined for a class, then all objects of that class will implement the event handler.  If an object subsequently defines its own event handler, the new event handler will be only be used for that object, and will be called in place of the class's event handler.

# See also

* [[/GLM/Object/Events]]