GridLAB-D Pull Requests Completed for Period 04/01/2023 - 04/30/2023 


# Update #1276: Fix install links 

Completed 04/23/2023 

 A number of `autoreconf -isf; ./configure; make system` errors occur. For example, building branch `develop_user_manual` on Darwin x86_64:

- [ ] `ld: warning: directory not found for option '-L/usr/local/opt/gridlabd/4.3.1-221021-develop_fix_directory_struct-Darwin-x86_64/lib`
- [ ] `ln: /usr/local/opt/gridlabd/4.3.1-230413-develop_user_manual-Darwin-x86_64/bin/pkgenv/lib/python3.10: Directory not empty`
- [x] `ln: /usr/local/opt/gridlabd/4.3.1-230413-develop_user_manual-Darwin-x86_64/bin/pkgenv/bin/python3-config: File exists
ln: /usr/local/opt/gridlabd/4.3.1-230413-develop_user_manual-Darwin-x86_64/bin/pkgenv/include/python3.10: File exists
ln: /usr/local/opt/gridlabd/4.3.1-230413-develop_user_manual-Darwin-x86_64/bin/pkgenv/lib/libpython3.10.dylib: File exists
ln: /usr/local/opt/gridlabd/4.3.1-230413-develop_user_manual-Darwin-x86_64/bin/pkgenv/lib/python3.10: File exists`

# Update #1278: Add create_schedule tool 

Completed 04/26/2023 

 This tool creates a schedule GLM from a CSV data file contain a time-series of values in a single column. Options allows controlling how pandas loads the CSV file.

# Update #1272: Add loaddata subcommand 

Completed 04/26/2023 

 The `loaddata` retrieves building load data from NREL Resstock and Comstock data lakes.  See `gridlabd loaddata help` for details.

- [x] Implement `loaddata` subcommand.
- [x] Implement Resstock and Comstock autotests.
- [x] Write documentation.

The primary use-case for this is downloading load data for buildings of various types at any location in the US and connecting the data to existing network models where the load buses have known customer types.

# Update #1216: Add advanced load model class to powerflow 

Completed 04/26/2023 

 Add fast and efficient quasi-steady load models to the powerflow module.
- [x] buildings
  - [x] thermal model
  - [x] DER volt/var control standards compliance
  - [x] technical support notebook
  - [x] residential buildings
    - [x] occupancy (defaults and docs)
    - [x] load composition from NERC (defaults and docs)
    - [x] loadshapes from EIA (defaults and docs)
  - [x] commercial buildings
    - [x] occupancy (defaults and docs)
    - [x] load composition from NERC (defaults and docs)
    - [x] loadshapes from CEUS (defaults and docs)
  - [x] validation
- [x] industrial loads
- [x] public service loads
- [x] agricultural loads

# Update #1279: Fix build warnings 

Completed 04/24/2023 

 - [x] Darwin20-x86_64 build
- [x] Darwin21-x86_64 build
- [x] Darwin21-arm64 build
- [x] Debian 11 x86_64/arm64 build
- [x] Ubuntu 20 x86_64/arm64 build
- [x] Ubuntu 22 x86_64/arm64 build

# Update #1281: Fixes for duplicate admittance error for incorrect single-phase transformers 

Completed 04/23/2023 

 Fix the issue documented in #1277
Adds checks for non-SPCT transformers to check for connections to triplex devices or having phase S.
Migration of fix from https://github.com/gridlab-d/gridlab-d/issues/1432

Added extra autotest to ensure this errors, though it could still technically pass on the previous failure.
