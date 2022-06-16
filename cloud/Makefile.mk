
cloud-deploy: aws-deploy gcp-deploy az-deploy

WEBSITES=$(shell find cloud/websites/*.gridlabd.us -type d -prune)

FORCE: 

aws-deploy: $(WEBSITES)

AWS_TARGET=$(if $(subst master,,$(shell git rev-parse --abbrev-ref HEAD)),-dev,)

%.gridlabd.us: FORCE
	@echo "syncing $(subst .gridlabd.us,$(AWS_TARGET).gridlabd.us,$(notdir $@)) with AWS_OPTIONS='$(AWS_OPTIONS)' ..."
	@aws s3 sync $@ s3://$(subst .gridlabd.us,$(AWS_TARGET).gridlabd.us,$(notdir $@)) $(AWS_OPTIONS) --acl public-read

gcp-deploy:
	@echo "WARNING: gcp-deploy is not implemented yet"

az-deploy:
	@echo "WARNING: azure-deploy is not implemented yet"
