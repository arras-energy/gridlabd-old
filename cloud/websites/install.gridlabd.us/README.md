# Creating new build image

You can create a fast-install image of your build of gridlabd using the below commands. Due to the image-generating process, it is recommended that you only build the version of gridlabd you intend to package on a clean virtual machine,
as the process will compress the entire gridlabd directory with all libraries installed during the build process. It will automatically name the image based on the gridlabd version, branch, and OS used to build gridlabd.

~~~
cd your-src-directory/cloud/websites/install.gridlabd.us
./build-image.sh
~~~

After the build is complete, the tarz file will be located in your $HOME/temp directory. Images built off a branch can be uploaded to AWS s3 install-dev.gridlabd.us for testing, and images built off of master can be uploaded to install.gridlabd.us. Linux images
should be built on docker containers to keep the tar file as small as possible, and universally usable.

### Useful Commands
For connections to a virtual machine (I used parallels) where you cannot easily transfer the packaged tar file
back to your local system. (Parallel Tools couldn't always be installed unfortunately)

~~~
scp remoteuser@remoteserver:/remote/folder/remotefile.txt  localfile.txt
~~~

The above official syntax is a little deceptive, and makes you manually rename the file. 
The below SCP example is the one I use and preserves the original filename.
Since I use automated naming for standardization and proper version tracking, the below example is generally better.

~~~
scp a123456@macOS.local:/Users/a123456/temp/*  $HOME/temp
~~~

The below command is useful for copying an image built in docker to the host for upload.

~~~
docker cp <containerId>:/file/path/within/container /host/path/target
~~~

Example to copy all files from folder into home directory

~~~
docker cp c7e8c527dcb2b9caf189a18ea56886014f8c3cfe08d05339058a797d7278b546:/root/temp/. $HOME
~~~

# Installing from AWS

Installation from AWS is designed to be as simple as possible for a user. Installation requires a user to have sudo priviledges and either wget or curl installed. The '''-b branch-name''' flag is required if you want an image from a specific branch, otherwise it will default to using the master branch. If no image is found for download for your specified branch and operating system, the fast install will fail. 

## Docker
To install on a linux docker container, use the following commands:

~~~
apt-get update
apt-get install sudo
apt-get install wget
wget https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/install-dev.sh
chmod 755 install-dev.sh
./install-dev.sh -b image-branch-name
~~~

You can also use curl, if preferred. For official images, use install.sh from install.gridlabd.us.

## MACOS/Debian
To install on Mac, use the following commands:

~~~
curl -OJ https://s3.us-west-1.amazonaws.com/install-dev.gridlabd.us/install-dev.sh
chmod 755 install-dev.sh
./install-dev.sh -b image-branch-name
~~~

The O and J flags for curl tells it to utilize the provided filename, otherwise you will have to manually name the file when you download it.

## Flags
The install script offers a few select flags for selecting your install.


`-b your-branch here` is used to select a branch, otherwise the install will default to the master branch.

`-v #.#.#` is used to specify a specific version, and is used for when you want to use a past fast-install version. Otherwise, the script will default to the latest version of the branch being used.

`-h` is your standard help, and will display these options when called.
