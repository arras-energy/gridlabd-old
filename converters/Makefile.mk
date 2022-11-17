#
# INPUT CONVERTERS
#

python_requirements += $(top_srcdir)/converters/requirements.txt

# csv -> glm
dist_pkgdata_DATA += converters/csv-ami2glm-ceus.py
dist_pkgdata_DATA += converters/csv-ami2glm-rbsa.py
dist_pkgdata_DATA += converters/csv-ceus2glm-ceus.py
dist_pkgdata_DATA += converters/csv-geodata2kml.py
dist_pkgdata_DATA += converters/csv-ica2glm-ica.py
dist_pkgdata_DATA += converters/csv-noaa-weather2glm-weather.py
dist_pkgdata_DATA += converters/csv-onpoint-weather2glm-weather.py
dist_pkgdata_DATA += converters/csv-scada2glm-ceus.py
dist_pkgdata_DATA += converters/csv-scada2glm-rbsa.py
dist_pkgdata_DATA += converters/csv-schneider2glm-player.py
dist_pkgdata_DATA += converters/csv-table2glm-library.py
dist_pkgdata_DATA += converters/csv-table2glm-object.py
dist_pkgdata_DATA += converters/csv-visualcrossing-weather2glm-weather.py
dist_pkgdata_DATA += converters/geometry.py
dist_pkgdata_DATA += converters/idfparse.py
dist_pkgdata_DATA += converters/csv2glm.py

# gz -> glm
dist_pkgdata_DATA += converters/gz2glm.py

# mdb -> glm
dist_pkgdata_DATA += converters/mdb2glm.py
dist_pkgdata_DATA += converters/mdb-cyme2glm.py
dist_pkgdata_DATA += converters/mdb-table2glm-object.py
dist_pkgdata_DATA += converters/mdb-table2glm-player.py

# omd -> glm
dist_pkgdata_DATA += converters/omd2glm.py

# tmy3 -> glm
dist_pkgdata_DATA += converters/tmy32glm.py

# txt -> glm
dist_pkgdata_DATA += converters/txt2glm.py
dist_pkgdata_DATA += converters/txt-cyme2glm.py

# zip -> glm
dist_pkgdata_DATA += converters/zip2glm.py

#
# OUTPUT CONVERTERS
#

# glm-> omd
dist_pkgdata_DATA += converters/glm2omd.py

# json -> csv
dist_pkgdata_DATA += converters/json2csv.py
dist_pkgdata_DATA += converters/json2csv-profile.py

# json -> html
dist_pkgdata_DATA += converters/json2html.py
dist_pkgdata_DATA += converters/json2html_config.py

# json -> glm
dist_pkgdata_DATA += converters/json2glm.py

# json -> md
dist_pkgdata_DATA += converters/json2md.py

# json -> png
dist_pkgdata_DATA += converters/json2png.py
dist_pkgdata_DATA += converters/json2png-oneline.py

# json -> txt
dist_pkgdata_DATA += converters/json2txt.py

# json -> zip
dist_pkgdata_DATA += converters/json2zip.py

# xls -> csv
dist_pkgdata_DATA += converters/xls2csv.py
dist_pkgdata_DATA += converters/xls-spida2csv-geodata.py

# xlsx -> csv
dist_pkgdata_DATA += converters/xlsx2csv.py
dist_pkgdata_DATA += converters/xlsx-workbook2csv-table.py

#
# SUPPORT MODULES
#
dist_pkgdata_DATA += converters/cyme.py
dist_pkgdata_DATA += converters/csv-modify2glm-modify.py
dist_pkgdata_DATA += converters/csv-config2glm-config.py
