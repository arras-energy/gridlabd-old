[[CircleCI]] - Continuous Integration using [CircleCI](https://circleci.com/)

CI support can be used to run simulations on hosted systems.  Here's a quick rundown of how it's done:

1. Login to your [GitHub](https://github.com/) account and create a new repository for your model.  
1. Login to your [CircleCI](https://circleci.com/) account and set it up to follow the new GitHub repository you just created.
1. Create the `.circleci/circle.yml` file:
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
           command: gridlabd my_test.glm
       - store_artifacts:
           path: /root/project
~~~
1. Create your model in `my_test.glm`:
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
     file "my_test.csv";
   };
 }
~~~
1. Add the YML and GLM files to your repo, commit and push them to your [GitHub](https://github.com/) repository.
1 Return to your [CircleCI](https://circleci.com/) account and look at the workflow. You should see your simulation queue up, run, and complete.  If it succeeds, click on the `Artifacts` tab and open the `/root/project` folder. You will see a link to output file the you can click on.



