#
# INPUT CONVERTERS
#

python_requirements += gldcore/converters/requirements.txt

# csv -> glm
dist_pkgdata_DATA += gldcore/converters/csv-ami2glm-ceus.py
dist_pkgdata_DATA += gldcore/converters/csv-ami2glm-rbsa.py
dist_pkgdata_DATA += gldcore/converters/csv-ceus2glm-ceus.py
dist_pkgdata_DATA += gldcore/converters/csv-ica2glm-ica.py
dist_pkgdata_DATA += gldcore/converters/csv-noaa-weather2glm-weather.py
dist_pkgdata_DATA += gldcore/converters/csv-onpoint-weather2glm-weather.py
dist_pkgdata_DATA += gldcore/converters/csv-scada2glm-ceus.py
dist_pkgdata_DATA += gldcore/converters/csv-scada2glm-rbsa.py
dist_pkgdata_DATA += gldcore/converters/csv-schneider2glm-player.py
dist_pkgdata_DATA += gldcore/converters/csv-table2glm-library.py
dist_pkgdata_DATA += gldcore/converters/csv-table2glm-object.py
dist_pkgdata_DATA += gldcore/converters/csv-visualcrossing-weather2glm-weather.py
dist_pkgdata_DATA += gldcore/converters/csv2glm.py

# mdb -> glm
dist_pkgdata_DATA += gldcore/converters/mdb2glm.py
dist_pkgdata_DATA += gldcore/converters/mdb-cyme2glm.py

# omd -> glm
dist_pkgdata_DATA += gldcore/converters/omd2glm.py

# tmy3 -> glm
dist_pkgdata_DATA += gldcore/converters/tmy32glm.py

# txt -> glm
dist_pkgdata_DATA += gldcore/converters/txt2glm.py
dist_pkgdata_DATA += gldcore/converters/txt-cyme2glm.py

# zip -> glm
dist_pkgdata_DATA += gldcore/converters/zip2glm.py

#
# OUTPUT CONVERTERS
#

# glm-> omd
dist_pkgdata_DATA += gldcore/converters/glm2omd.py

# json -> csv
dist_pkgdata_DATA += gldcore/converters/json2csv.py
dist_pkgdata_DATA += gldcore/converters/json2csv-profile.py

# json -> html
dist_pkgdata_DATA += gldcore/converters/json2html.py
dist_pkgdata_DATA += gldcore/converters/json2html_config.py

# json -> glm
dist_pkgdata_DATA += gldcore/converters/json2glm.py

# json -> md
dist_pkgdata_DATA += gldcore/converters/json2md.py

# json -> png
dist_pkgdata_DATA += gldcore/converters/json2png.py
dist_pkgdata_DATA += gldcore/converters/json2png-oneline.py

# json -> txt
dist_pkgdata_DATA += gldcore/converters/json2txt.py

# json -> zip
dist_pkgdata_DATA += gldcore/converters/json2zip.py

#
# SUPPORT MODULES
#
dist_pkgdata_DATA += gldcore/converters/cyme.py
dist_pkgdata_DATA += gldcore/converters/csv-modify2glm-modify.py
dist_pkgdata_DATA += gldcore/converters/csv-config2glm-config.py
