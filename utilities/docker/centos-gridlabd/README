To create a docker image run the command

   % docker build https://raw.githubusercontent.com/dchassin/gridlabd/master/utilities/docker/centos-gridlabd/Dockerfile -t centos-gridlabd

or from the local Dockerfile:

    % docker build -f Dockerfile -t centos-gridlabd .
   
If the build succeeds and passes validation, the container will be created.

To save the docker image use the following command

   % docker save gridlabd -o centos-gridlabd

To run gridlabd using a volume to map a local folder to a docker folder and exposing the host network (to let GridLAB-D connect to MySQL), use the following command.  <localpath> can be subsituted with "$(pwd)"

   % docker run -it --network host -v <localpath>:/model centos-gridlabd gridlabd -W /model <your-glm-file>
   
To run a shell instead of gridlabd, use the following command

   % docker run -it -v <localpath>:/model centos-gridlabd bash
   
If you want to use an alias, try this one:

   % alias gridlabd='docker run -it -v $(pwd):/gridlabd centos-gridlabd gridlabd -W /gridlabd'
