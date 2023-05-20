import sys 
import difflib
MODEL_NAME = sys.argv[1]
# MODEL_NAME="meters-withloads nodes-withloads notnodes-withloads links-to-meter links-to-triplexmeter link-to-notnode links-to-meter-to-child links-to-triplexmeter-to-child"

FAILED=0
input_file_1=''
input_file_2=''

# for model in MODEL_NAME.split() : 
	# with open('../'+model+'-meters.glm', 'r') as f1, open(model+'-meters.glm', 'r') as f2:
	# 	for line1, line2 in zip(f1, f2):
	# 		line1 = line1.strip()
	# 		line2 = line2.strip()

	# 		if line1.startswith('guid') or line1.startswith('//'):
	# 			continue

	# 		if line2.startswith('guid') or line2.startswith('//'):
	# 			continue

	# 		if line1 != line2:
	# 			FAILED=FAILED+1
	# 			print('FILE '+ '../'+model+'-meters.glm : ' + line1)
	# 			print('FILE '+ model+'-meters.glm : ' + line2)

def filter_lines(lines):
    skip_prefixes = ['guid', '//', 'solver_py_config', '#define pythonpath', '#set pythonpath',
                     '#define tmp', '#set tmp', '#define kmlhost', '#set kmlhost', '#define datadir',
                     '#set datadir', 'name', 'global enumeration validto_context', '#set validto_context',
                     '#define browser', '#set browser']
    return [line for line in lines if not any(line.strip().startswith(prefix) for prefix in skip_prefixes)]

for model in MODEL_NAME.split() : 
	with open('../'+model+'-meters.glm', 'r') as file1, open(model+'-meters.glm', 'r') as file2:
	    filtered_lines1 = filter_lines(file1.readlines())
	    filtered_lines2 = filter_lines(file2.readlines())

	    diff = difflib.unified_diff(filtered_lines1, filtered_lines2, lineterm='')
	    diff_lines = list(diff)

	if diff_lines : 
		for line in diff_lines:
		    print(line)
		sys.exit(1)
