# Basic Deployment

Version information is tracked in a deployed database. This information is updated and compared utilizing two different lambda routes, version_handler and update_latest. 

The initial infrastructure is built and deployed using terraform. The terraform code has been prepared for you in the terraform directory.

In order for the lambda functions to work out-of-the-box, you will need to install the package dependencies in the app folder. You can do this using the command below, called from the repository root directory, however lambda functions run using intel processors. 

If you are on an m1 machine, you will need to run the pip install and terraform deployment on an intel ec2 instance, or run the terraform deployment and then run the deploy-images github actions workflow to update the lambda function with the pip requirements installed into the lambda package.

```pip install -r ./cloud/websites/version.gridlabd.us/lambda/requirements.txt -t ./cloud/websites/version.gridlabd.us/lambda/package```

To deploy the lambda functions, you will need to change into the terraform directory. If you have not already done so, make sure you have terraform installed, and your aws profile properly configured. If you have multiple AWS profiles, make sure to export the correct profile in the command line. Then, run ```terraform plan``` if you want to pre-check the resources being created, and ```terraform apply``` to deploy the resources.

# Post-Deployment

The entire cluster is protected within a VPC (virtual private cloud), with only the lambda functions accessible through a configured API gateway. Route53 and cloudfront will need to be configured manually if you want to create a named dns endpoint to redirect to your version service. 

Additionally, you will need to manually add two key-value pairs to your gld_database_secrets secret in aws secrets manager, that was created from terraform. These are ```developsk``` and ```mastersk```. These secrets are needed to update the latest version saved in the db, and must be included in any post request to the routes. 

# Routes

There are two routes available from this deployemnt.

```version.gridlabd.us/?v=<version>&b=<branch>``` and ```version.gridlabd.us/update_latest```.

The first route supports get requests using query string parameters. The second route supports only post requests using a json body. 

When you run ```gridlabd --version```, it will automatically query the first route, which will log the version and branch. Then, if you are on one of the two major branches (develop or master), it will compare your version and build number to the latest official version. If your version is older, then it will warn you that you are out of date and will include the latest information along with your current version information.

The first route can also be used to query version stats from the database. When using the default route, or the "by=version" query parameter, you can get the stats for all the specific version calls made. Additional grouping methods will be supported at a later date.

The second route is intended to be used in the CI/CD process to update the officially published latest version of the develop and master branches. 

However, it can be queried manually using curl. It will only store one entry for the latest version. If you attempt to update the latest version for any branch other than master or develop, you will receive a 400 error. 

If you manually run the version-check route, you will receive "ok" if your version matches the latest version OR if you are running on a non-major branch (develop or master). 

# example queries

To manually update the latest version:
```curl -X POST https://version.gridlabd.us/update_latest -H "Content-Type: application/json" -d '{"version": "#.#.#", "build": "yymmdd", "branch": "develop", "sk": "developsk-inserted-here"}'```

To manually check a version:
```curl -v "https://version.gridlabd.us/?v=4.3.1-230531&b=develop"```

To get stats:
```curl -v "https://version.gridlabd.us/"``` or ```curl -v "https://version.gridlabd.us/?by=version"```

# Maintaining the lambda functions

Over time, you may want to update the lambda functions for new functionality, or to address code maintenance issues. The CI/CD process will automatically update the app.py code and install any new packages added to requirements.txt when deployed. 

However, to add new lambda routes will require updating the terraform code itself, which is a much more advanced task and is not recommended for regular users. 