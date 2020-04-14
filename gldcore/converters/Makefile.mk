#
# INPUT CONVERTERS
#

# csv -> glm
dist_pkgdata_DATA += gldcore/converters/csv2glm.py
dist_pkgdata_DATA += gldcore/converters/csv-ceus2glm-ceus.py
dist_pkgdata_DATA += gldcore/converters/csv-ami2glm-rbsa.py
dist_pkgdata_DATA += gldcore/converters/csv-ami2glm-ceus.py
dist_pkgdata_DATA += gldcore/converters/csv-scada2glm-rbsa.py
dist_pkgdata_DATA += gldcore/converters/csv-scada2glm-ceus.py
dist_pkgdata_DATA += gldcore/converters/csv-ica2glm-ica.py

# zip -> glm
dist_pkgdata_DATA += gldcore/converters/zip2glm.py

#
# OUTPUT CONVERTERS
#

# json -> csv
dist_pkgdata_DATA += gldcore/converters/json2csv.py
dist_pkgdata_DATA += gldcore/converters/json2csv-profile.py

# json -> glm
dist_pkgdata_DATA += gldcore/converters/json2glm.py

# json -> png
dist_pkgdata_DATA += gldcore/converters/json2png.py
dist_pkgdata_DATA += gldcore/converters/json2png-oneline.py

# json -> txt
dist_pkgdata_DATA += gldcore/converters/json2txt.py

# json -> zip
dist_pkgdata_DATA += gldcore/converters/json2zip.py
dist_pkgdata_DATA += gldcore/converters/csv-modify2glm-modify.py
dist_pkgdata_DATA += gldcore/converters/csv-config2glm-config.py
