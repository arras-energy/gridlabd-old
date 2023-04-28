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

source "amazon-ebs" "ubuntu-22-04" {
  ami_name_filter = "ubuntu/images/*ubuntu-focal-22.04-amd64-server-*"
  ami_virtualization_type = "hvm"
  most_recent = true
  owners = ["099720109477"]
  region = var.aws_region
  root_device_type = "ebs"
}

build {
  sources = ["source.amazon-ebs.ubuntu-22-04"]

  provisioner "shell" {
    inline = [
      "sudo apt-get update",
      "sudo apt-get install -y git curl nano",
      "git clone https://github.com/slacgismo/gridlabd.git",
      "cd gridlabd",
      "./install.sh -v -t -p",
      "gridlabd --version=all",
      "gridlabd --validate"
    ]
  }

  post-processors {
    amazon-import {
      access_key = var.aws_access_key
      ami_regions = [var.aws_region]
      ami_name = "HiPAS Gridlabd Ubuntu 22.04 {{timestamp}}"
      keep_input_artifact = true
      region = var.aws_region
      secret_key = var.aws_secret_key
    }
  }
}
