# AWS EC2 Setup Procedure

1. Create a new t2micro EC2 instance
2. Open a shell and run the following command
   
   curl https://code.gridlabd.us/${BRANCH}/cloud/servers/version.gridlabd.us/install.sh | bash

By default the script pulls from the master branch. To change
which branch is used for the source set the BRANCH environment
variable to the name of the source branch, e.g.,

   export BRANCH=develop
   curl https://code.gridlabd.us/${BRANCH}/cloud/servers/version.gridlabd.us/install.sh | bash

# Reading version check log

The version check log may be read at https://version.gridlabd.us/access.csv.
