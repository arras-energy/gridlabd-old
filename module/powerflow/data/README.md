# Loadshape data preparation

The source commercial building loadshape data is stored in `CEUS`. To update
the loadshape data stored in `../default_loadshapes.csv` do the following.

1. Run `python3 extract_ceusdata.py` if `ceus_loadshapes.csv` is outdated.
2. Run `python3 update_loadshapes.py` if `../building_loadshapes.csv` is outdated.
3. Verify the PNG files to validate the loadshapes.
