[[/Glm/Macro/Include]] -- Macro to include another GLM

# Synopsis
GLM:
~~~
#include "file-name"
#include <library-name>
#include [url]
#include using(<name1>=<value1>,<name2>=<value2>,...,<nameN>=<valueN>) <location>
~~~

# Description

The `#include` macro inserts the file found at `<location>` into the current GLM stream processed by the loader.  If the `using()` option is included, the parameter list specified is parse to set global variables used while processing the include file.

The `#include using(<parameters>) "file-name"` syntax is equivalent to `#insert file(<parameters>)`

# Examples

The following includes `my-file.glm` in the current GLM loader stream.
~~~
#include "my-file.glm"
~~~

# See also
* [[/Glm/Macro/Insert]]

