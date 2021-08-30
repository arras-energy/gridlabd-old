dist_pkgdata_DATA += gldcore/geodata/geodata_address.py
dist_pkgdata_DATA += gldcore/geodata/geodata_census.py
dist_pkgdata_DATA += gldcore/geodata/geodata_distance.py
dist_pkgdata_DATA += gldcore/geodata/geodata_elevation.py
dist_pkgdata_DATA += gldcore/geodata/geodata_powerline.py
dist_pkgdata_DATA += gldcore/geodata/geodata_powerline_cabletypes.csv
# dist_pkgdata_DATA += gldcore/geodata/geodata_transportation.py
dist_pkgdata_DATA += gldcore/geodata/geodata_utility.py
dist_pkgdata_DATA += gldcore/geodata/geodata_vegetation.py
# dist_pkgdata_DATA += gldcore/geodata/geodata_weather.py

python_requirements += gldcore/geodata/requirements.txt

docs_targets += docs/Tutorials/Geodata.md

docs/Tutorial/Geodata.md: gldcore/geodata/docs/Geodata.ipynb
	@pandoc $< -s -t gfm | sed -n '20,$$s/^\!/bash% /;20,$$p' > $@
