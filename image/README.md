# Downloadable Image Creation

Images are created by running the script

~~~
$ sh image/create.sh
~~~

The image is createing in the current folder and should have the name given by
the output of the command `gridlabd --version=name`. The image can be
uploaded using the command

~~~
aws s3 cp image/$(gridlabd --version=name).tarz s3://install.gridlabd.us/ --acl public-read
~~~

