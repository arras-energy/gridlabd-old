# Converters

This folder contains the automatic file converters for Arras Energy.

Converters are implement in a two-level paradigm. This top level implements
the file format converter based on the files' extensions. The second level
implements the semantic conversion based on the type information provided to
the command line of the format converters.

For example, a conversion from from CSV to GLM will always be delivered to the
`csv2glm` converter, which will used the from and to type information to
identify the semantic converter used, e.g., AMI to CEUS will be delivered to
the `csv-ami2glm-ceus` converter.
