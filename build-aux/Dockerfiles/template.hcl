packer {
  required_plugins {
    amazon = {
      version = ">= 0.0.2"
      source  = "github.com/hashicorp/amazon"
    }
  }
}

source "amazon-ebs" "ubuntu-22-04" {
  ami_name_filter = "ubuntu/images/*ubuntu-focal-22.04-amd64-server-*"
  ami_virtualization_type = "hvm"
  most_recent = true
  owners = ["099720109477"]
  region = "{{user `aws_region`}}"
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
      access_key = "{{user `aws_access_key`}}"
      ami_regions = ["{{user `aws_region`}}"]
      ami_name = "HiPAS Gridlabd Ubuntu 22.04 {{timestamp}}"
      keep_input_artifact = true
      region = "{{user `aws_region`}}"
      secret_key = "{{user `aws_secret_key`}}"
    }
  }
}

