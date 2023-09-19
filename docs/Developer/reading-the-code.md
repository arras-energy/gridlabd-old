# C++ code files

The 'PT_' preffix for variable names means "Property Type".

oclass means object class
pclass means parent class

## get_<property>_offset() 
These functions return offsets in memory location. Every object attribute is listed sequentially in memory (I believe in the order that they are declared?). This function returns the difference in the memory address of an attribute from the start of the object in memory. It's defined by a macro, `GL_ATOMIC(type, attribute name)` which generates a callable function specific to every attribute it's applied to.

The code looks like this:
`static inline size_t get_##X##_offset(void) { return (char*)&(defaults->X)-(char*)defaults; };`

`defaults` is a class attribute, a pointer to an object of the same type as the class it's a part of. Say we are looking at the `pole` class. `defaults` will be a pointer to a pole object. Confusingly, `defaults` is never inititialized to anything beyond NULL. How does this work? Back when this program was written in C, `defaults` pointed to a chunk of memory holding an object with all attributes initilized to default values. In C you could create a new object by simply copying this memory chunk. In C++ you need to declare all the attributes within an object, not just the object itself. The old method of setting defaults stopped working, so it's done differently now. The `defaults` pointer was retained as an attribute in the class declaration. The get offset function uses it for pointer math, finding the differences in memory location between the start of an object and all of it's attributes. Because a class declaration includes the type of all attributes, the compiler can figure out how much space they will take up in memory. (In C++ all types have a fixed size.)  'defaults' never needs to be initialized.