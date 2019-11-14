
cloud-deploy: aws-deploy gcp-deploy azure-deploy

WEBSITES = code$(SUFFIX).gridlabd.us docs$(SUFFIX).gridlabd.us www$(SUFFIX).gridlabd.us

aws-deploy: $(WEBSITES)

$(WEBSITES) :
if HAVE_AWSCLI
if SUFFIX
	@echo "deploying $@ for SUFFIX='$(SUFFIX)'..."
else
	@echo "cannot deploy $@ for SUFFIX='$(SUFFIX)'"
endif
endif
# if SUFFIX
#  	@echo "aws s3 cp cloud/websites/$@/index.html s3://$@/index.html"
#  	@echo "aws s3api put-object-acl --bucket $@ --key index.html --acl public-read"
# endif

gcp-deploy:
if HAVE_GCPCLI
	@echo "WARNING: gcp-deploy is not implemented yet"
endif

azure-deploy:
if HAVE_AZCLI
	@echo "WARNING: azure-deploy is not implemented yet"
endif
