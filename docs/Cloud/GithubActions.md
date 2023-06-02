[[/Cloud/Git]] -- Cloud Continuous Integration and Deployment Support

# Synopsis

YML:

~~~
name: simulation

on: [push]

jobs:
  simulate:

    runs-on: ubuntu-latest
    container: arras-energy/gridlabd:master

    steps:
    - uses: actions/checkout@v3

      # this is to fix GIT not liking owner of the checkout dir
    - name: Set ownership
      run: |
        chown -R $(id -u):$(id -g) $PWD
    - name: Run Simulation
      run: |
        gridlabd model.glm
    - name: Upload artifacts
      uses: actions/upload-artifact@v2
      with:
        name: artifact-results
        path: |
          /root/project
~~~

# Description

Cloud Continuous Integration tools can be used to run simulations on hosted systems.  The following illustrates how this is done using [CircleCI](https://circleci.com/):

1. Login to your [GitHub](https://github.com/) account and create a new repository for your model.  

3. Create the `.github/workflows/your-workflow.yml` file in your new GitHub repository:

~~~
name: simulation

on: [push]

jobs:
  simulate:

    runs-on: ubuntu-latest
    container: arras-energy/gridlabd:master

    steps:
    - uses: actions/checkout@v3

      # this is to fix GIT not liking owner of the checkout dir
    - name: Set ownership
      run: |
        chown -R $(id -u):$(id -g) $PWD
    - name: Run Simulation
      run: |
        gridlabd example.glm
    - name: Upload artifacts
      uses: actions/upload-artifact@v2
      with:
        name: artifact-results
        path: |
          /root/project
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

6. Go to the `Actions` tab and look at your workflows. You should see your simulation queue up, run, and complete.  If it succeeds, click on the `Artifacts` tab and open the `/root/project` folder. You will see a link to output file the you can click on.



