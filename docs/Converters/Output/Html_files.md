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

### `-s|--show` 

Immediately open the HTML file in the default browser.

### `-t|--tiles TILES`

The output PNG is a oneline diagram of the powerflow network.

### `-z|--zoomlevel LEVEL`

Specify the initial zoomlevel. The default is `auto`.

# Example

The follow example convert the single node to a map and opens it in the default browser.

~~~
class node
{
    string address;
}
object node
{
    address "2757 Sand Hill Rd., Menlo Park CA";
    latitude "37.4205";
    longitude "-122.2047";
}
#output "test_json2html.html" --show
~~~

# See also

* [[/Subcommand/Geodata]]

