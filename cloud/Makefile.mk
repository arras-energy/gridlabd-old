# cloud/websites/Makefile.mk
#
# Cloud deployment makefile
#
#   $ make cloud-deploy
#
# AWS - Update AWS S3 buckets *.gridlabd.us
#
#   $ make aws-deploy
#
#   The *-dev.gridlabd.us websites are updated for all
#   branches except "master", which updates *.gridlabd.us
#
#   Options:
#
#     AWS_OPTIONS=--dryrun    do not update websites
#
# GCP - No GCP resources are updated at this time
#
#   $ make gcp-deploy
#
# AZ - No AZ resources are updated at this time
#
#   $ make az-deploy
#

#
# Cloud deployment platforms
#

cloud-deploy: aws-deploy gcp-deploy az-deploy

#
# Get a list of files that have to be updated
#

WEBSITES=$(shell find cloud/websites/*.gridlabd.us -type d -prune)

#
# Target that forces updates
#

FORCE: 

#
# AWS hosts the main websites to sync
#
aws-deploy: $(WEBSITES)

#
# Target *-dev.gridlabd.us for all branches except "master"
#

AWS_TARGET=$(if $(subst master,,$(shell git rev-parse --abbrev-ref HEAD)),-dev,)

#
# Websites that should be sync'd
#

%.gridlabd.us: FORCE
	@echo "syncing $(subst .gridlabd.us,$(AWS_TARGET).gridlabd.us,$(notdir $@)) with AWS_OPTIONS='$(AWS_OPTIONS)' ..."
	@aws s3 sync $@ s3://$(subst .gridlabd.us,$(AWS_TARGET).gridlabd.us,$(notdir $@)) $(AWS_OPTIONS) --acl public-read

#
# GCP updates
#
gcp-deploy:

azure-deploy:
if HAVE_AZCLI
	@echo "WARNING: azure-deploy is not implemented yet"
endif

release:
	@echo "WARNING: make release not implemented yet"
