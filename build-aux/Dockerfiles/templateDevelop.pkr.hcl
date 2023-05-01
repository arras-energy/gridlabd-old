# template.pkr.hcl
packer {
  required_plugins {
    amazon = {
      version = ">= 0.0.2"
      source  = "github.com/hashicorp/amazon"
    }
  }
}

variable "aws_access_key" {
  type        = string
  description = "AWS access key"
}

variable "aws_secret_key" {
  type        = string
  description = "AWS secret access key"
}

variable "aws_region" {
  type        = string
  description = "AWS region"
}

variable "dev_s3_url" {
  type        = string
  description = "S3 bucket URL"
}

source "amazon-ebs" "ubuntu-22-04" {
  source_ami           = "ami-081a3b9eded47f0f3"
  region               = var.aws_region
  instance_type        = "t2.micro"
  ssh_username         = "ubuntu"
  ami_name             = "HiPAS Gridlabd Ubuntu 22.04 {{timestamp}}"
  ebs_volume_size      = 25
}


build {
  sources = ["source.amazon-ebs.ubuntu-22-04"]

  provisioner "shell" {
    inline = [
      "sudo apt-get remove unattended-upgrades -y",
      "sudo mkdir -p /usr/local/var",
      "sudo chown -R $USER /usr/local",
      "sudo apt-get update && sudo apt-get install -y apt-transport-https",
      "sudo apt-get install -y git curl nano",
      "git clone -b develop https://github.com/slacgismo/gridlabd.git",
      "cd gridlabd",
      "./install.sh -v -t -p",
      "gridlabd --version=all",
      "gridlabd --validate"
    ]
  }

  post-processor "amazon-import" {
    access_key = var.aws_access_key
    region = var.aws_region
    secret_key = var.aws_secret_key
    ami_name = "HiPAS Gridlabd Ubuntu 22.04 {{timestamp}}"
    keep_input_artifact = true
    ami_groups = ["all"] # Make the AMI publicly available
    s3_bucket_name = var.dev_s3_url
  }
}
