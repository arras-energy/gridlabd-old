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

variable "prod_s3_url" {
  type        = string
  description = "S3 bucket URL"
}

source "amazon-ebs" "ubuntu-22-04" {
  source_ami           = "ami-014d05e6b24240371"
  region               = var.aws_region
  instance_type        = "t2.micro"
  ssh_username         = "ubuntu"
  ami_name             = "HiPAS Gridlabd Ubuntu 22.04 {{timestamp}}"
  ami_groups            = ["all"] // This will make the AMI publicly available

  launch_block_device_mappings {
    device_name           = "/dev/sda1"
    volume_size           = 25
    volume_type           = "gp2"
    delete_on_termination = true
  }

  tags = {
    Name        = "HiPAS Gridlabd Ubuntu 22.04 {{timestamp}}"
    CreatedBy   = "Packer"
    Environment = "Prod"
  }
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
      "cd /usr/local/src",
      "export INSTALL_SOURCE=https://install-dev.gridlabd.us",
      "curl -sL http://install.gridlabd.us/install.sh | sudo sh",
      "sudo chown -R $USER /usr/local",
      "gridlabd --version=all"
    ]
  }
}
