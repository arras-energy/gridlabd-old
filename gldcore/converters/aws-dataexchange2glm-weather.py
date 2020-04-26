
def convert(input,output,options={}):
	if 'region' not in options.keys():
		raise Exception("region must be specified in options")
	source = input.replace('.aws','')
	region = options["region"]
	arn = f'arn:aws:dataexchange:{region}::data-sets/{source}'
	
	with open(output,"w") as glm:
		glm.write(f'// ARN: {arn}\n')
		glm.write(f'module climate;\n')
		glm.write(f'object climate\n')
		glm.write('{\n')
		glm.write('}\n')

