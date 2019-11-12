
cloud-deploy: aws-deploy gcp-deploy azure-deploy

WEBSITES = www.gridlabd.us
SUBDOMAINS = code docs www

aws-deploy: $(foreach DOMAIN,$(WEBSITES),$(foreach SITE,$(SUBDOMAINS),$(SITE)$(SUFFIX).$(DOMAIN)))

$(foreach DOMAIN,$(WEBSITES),%.$(DOMAIN)) :
if SUFFIX
	@echo "Updating $@..."
if HAVE_AWSCLI
 	@echo "aws s3 cp cloud/websites/$@/index.html s3://$@/index.html"
 	@echo "aws s3api put-object-acl --bucket $@ --key index.html --acl public-read"
else
 	@echo "WARNING: aws-cli is not installed"
endif
else
	@echo "cannot deploy branch $$(git rev-parse --abbrev-ref HEAD) to $@"
endif

gcp-deploy:
if HAVE_GCPCLI
	@echo "WARNING: gcp-deploy is not implemented yet"
endif

azure-deploy:
if HAVE_AZCLI
	@echo "WARNING: azure-deploy is not implemented yet"
endif
