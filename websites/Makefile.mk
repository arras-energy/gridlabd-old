cloud-deploy: aws-deploy gcp-deploy azure-deploy

aws-deploy:
if HAVE_AWSCLI
	@echo "Uploading files to AWS websites"
	@aws s3 cp websites/docs.gridlabd.us/index.html s3://docs.gridlabd.us/index.html
	@aws s3 cp websites/code.gridlabd.us/index.html s3://code.gridlabd.us/index.html
	@aws s3 cp websites/www.gridlabd.us/index.html s3://www.gridlabd.us/index.html
	@echo Granting public access to AWS websites...
	@aws s3api put-object-acl --bucket docs.gridlabd.us --key index.html --acl public-read
	@aws s3api put-object-acl --bucket code.gridlabd.us --key index.html --acl public-read
	@aws s3api put-object-acl --bucket www.gridlabd.us --key index.html --acl public-read
endif

gcp-deploy:
if HAVE_GCPCLI
	@echo "WARNING: gcp-deploy is not implemented yet"
endif

azure-deploy:
if HAVE_AZCLI
	@echo "WARNING: azure-deploy is not implemented yet"
endif
