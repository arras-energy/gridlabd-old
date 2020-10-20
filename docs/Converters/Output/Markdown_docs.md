[[/Converters/Output/Mardown_docs]] -- Markdown documents

# Synopsis

~~~
#output "filename.md" [--no_{classes,globals,modules,objects}|--with_{classes,contents,hyperlinks}]
~~~

# Description

The markdown document output converter generates a markdown document that tabulates information in the GLM's corresponding JSON file.

## Output options

### `--{no,with}_classes`

The markdown document excludes/includes class information.  By default class information is included if available.  If no objects are present in the file, all classes are included by default unless `--no_classes` is used.  If objects are present, only referenced classes are included by default, unless `--no_classes` is used, in which case no class output is generated, or `--with_classes` is used in which case all class output is generated.

### `--no_globals`

The markdown document excludes global data information.

### `--no_modules`

The markdown document excludes module information.

### `--no_objects`

The markdown document excludes object information.

### `--with_contents`

The markdown document includes a table of contents.

### `--with_hyperlinks`

The markdown document includes hyperlinks to known entities.

# See also

* [[/GLM/Macro/Output]]
