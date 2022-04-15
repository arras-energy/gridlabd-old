[[/Converters/Output/Html_files]] -- HTML document converter

# Synopsis

~~~
#output "filename.html" [options]
~~~

# Description

The HTML output converter generates an interactive `folium` map in HTML.

## Options

### `-c|--cluster`

Disables marker clustering.

### `-g|--glyphs`

Selects the icon family from which glyphs are selected. Valid values are `fa` and `glyphicon`.

### `-s|--show` 

Immediately open the HTML file in the default browser.

### `-t|--tiles TILES`

Specifies which background tiles are to be used. Valid values are `street`, `terrain`, `lineart`, 

### `-z|--zoomlevel LEVEL`

Specify the initial zoomlevel. The default is `auto`.

# Example

The following example converts the single node to an interactive map and opens it in the default browser.

~~~
class node
{
    string address;
}
object node
{
    address "2575 Sand Hill Rd., Menlo Park CA";
    latitude "37.4205";
    longitude "-122.2047";
}
#output "test_json2html.html" --show
~~~

The following example compiles the IEEE-123 model into an interactive map and opens it in the default browser:

~~~
$ gridlabd -W converters/autotest -I IEEE-123.glm -o /tmp/IEEE-123.html -D html_save_options="--show --cluster"
~~~

# See also

* [[/Subcommand/Geodata]]

