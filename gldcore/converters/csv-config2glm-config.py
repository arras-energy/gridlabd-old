import csv

def convert(input_file, output_file, options={}):
    """Convert a CSV configuration list a GLM config file

    Parameters:
        input_file (str) - input file name (csv file)
        ouput_file (str) - output file name (glm file)
        options (dict)
            "prefix" (str) - prefix to use on GLM global names (defaut "")
            "relax" (bool) - specify set/define relaxation (default None)
    """
    if "prefix" in options.keys():
        prefix = options["prefix"]
    else:
        prefix = ""
    with open(input_file,"r") as csvfile:
        reader = csv.reader(csvfile)
        with open(output_file,"w") as glmfile:
            if "relax" in options.keys():
                glmfile.write(f"#set strictnames={options['relax']}\n")
            for row in reader:
                glmfile.write(f"#define {prefix}{row[0]}={row[1]}\n")
