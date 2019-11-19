[[/Cloud/AWS]] -- Amazon Web Services support

# Synopsis

AWS Login:
~~~
user@localhost$ gridlabd aws configure
~~~

EC2:
~~~
user@localhost$ gridlabd ec2 create-image [ec2-createimage-options]
user@localhost$ gridlabd ec2 copy-image [ec2-copyimage-options]
user@localhost$ gridlabd ec2 start-instances [ec2-startinstances-options]
user@localhost$ gridlabd ec2 run-instances [ec2-runinstances-options]
user@localhost$ gridlabd ec2 describe-instances [ec2-describeinstances-options]
user@localhost$ gridlabd ec2 reboot-instances [ec2-rebootinstances-options]
user@localhost$ gridlabd ec2 stop-instances [ec2-stopinstances-options]
user@localhost$ gridlabd ec2 terminate-instances [ec2-terminateinstances-options]
~~~

S3:
~~~
user@localhost$ gridlabd aws s3 mb bucket-name [s3-mb-options]
user@localhost$ gridlabd aws s3 cp from to [s3-cp-options]
user@localhost$ gridlabd aws s3 sync local-folder bucket-name [s3-sync-options]
~~~

# Description

## aws ec2

The following commands are used to manage GridLAB-D images and instances.

### aws ec2 create-image

Creates a GridLAB-D AWS image. See `aws ec2 create-image help` for details. See [`aws ec2 create-image help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/create-image.html) for details.

### aws ec2 copy-image

Copies a GridLAB-D AWS image.  See [`aws ec2 copy-image help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/copy-instance.html) for details.

### aws ec2 start-instances

Starts one or more GridLAB-D AWS instances. See [`aws ec2 start-instances help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/start-instance.html) for details.

### aws ec2 run-instances

Runs one or more GridLAB-D AWS instances. See [`aws ec2 run-instances help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/run-instances.html) for details.

### aws ec2 describe-instances

Describes one or more GridLAB-D AWS instances. See [`aws ec2 describe-instances help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/describe-instances.html) for details.

### aws ec2 reboot-instances

Reboots one or more GridLAB-D AWS instances.See [`aws ec2 reboot-instances help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/reboot-instances.html) for details.

### aws ec2 stop-instances

Stops one or more GridLAB-D AWS instances. See [`aws ec2 stop-instances help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/stop-instances.html) for details.

### aws ec2 terminate-instances

Terminates one or more GridLAB-D AWS instances. See [`aws ec2 terminate-instances help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/terminate-instances.html) for details.

## aws s3 

The following commands are used to manage and access data.

### aws s3 mb

Creates an S3 bucket. See [`aws s3 mb help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/s3/mb.html) for details.

### aws s3 cp

Copies files to and from an S3 bucket. See [`aws s3 cp help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/s3/cp.html) for details.

### aws s3 sync

Synchronizes local files with an S3 bucket. See [`aws s3 sync help`](https://docs.aws.amazon.com/cli/latest/reference/opsworks/s3/sync.html) for details.

# Examples

The following command connects to an EC2 instance and installs GridLAB-D:
~~~
user@localhost$ ssh -i my-key.pem ec2-user@my-instance-ip
ec2-user@my-instance-ip$ sudo yum install git -y
ec2-user@my-instance-ip$ git clone https://github.com/dchassin/gridlabd gridlabd
ec2-user@my-instance-ip$ gridlabd/install.sh
~~~

The following GLM includes a standard library file:
~~~
#include [http://library.gridlabd.us/CA/SLAC/wood_pole.glm]
~~~
