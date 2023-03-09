# Deprecated

CircleCI support has been deprecated in favor of github actions for running CI/CD Processes. This page is no longer in use.


[[/Cloud/CircleCI]] -- Cloud Continuous Integration support

# Synopsis

YML:

~~~
 version: 2
 jobs:
   build:
     docker:
       - image: gridlabd/slac-master
     steps:
       - checkout
       - run:
           name: Run simulation
           command: gridlabd model.glm
       - store_artifacts:
           path: /root/project
~~~

# Description

Cloud Continuous Integration tools can be used to run simulations on hosted systems.  The following illustrates how this is done using [CircleCI](https://circleci.com/):

1. Login to your [GitHub](https://github.com/) account and create a new repository for your model.  

2. Login to your [CircleCI](https://circleci.com/) account and set it up to follow the new GitHub repository you just created.

3. Create the `.circleci/circle.yml` file in your new GitHub repository:

~~~
         version: 2
         jobs:
           build:
             docker:
               - image: gridlabd/slac-master
             steps:
               - checkout
               - run:
                   name: Run simulation
                   command: gridlabd example.glm
               - store_artifacts:
                   path: /root/project
~~~

4. Create your model in `example.glm`in your new GitHub repository:

~~~
         clock {
             timezone "US/CA/San Francisco";
             starttime "2019-04-01 00:00:00";
             stoptime "2019-04-02 00:00:00";
         }
         module residential;
         module tape;
         object house {
             heating_setpoint "68 degF";
             cooling_setpoint "72 degF";
             object recorder {
                 property "air_temperature";
                 interval "1 min";
                 file "example.csv";
             };
         }
~~~

5. Add the YML and GLM files to your repo, commit and push them to your [GitHub](https://github.com/) repository.

6. Return to your [CircleCI](https://circleci.com/) account and look at the workflow. You should see your simulation queue up, run, and complete.  If it succeeds, click on the `Artifacts` tab and open the `/root/project` folder. You will see a link to output file the you can click on.



