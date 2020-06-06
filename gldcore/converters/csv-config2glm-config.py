"""Convert a CSV configuration list a GLM config file
"""
import csv

def convert(input_file, output_file, options={}):
    if "prefix" in options.keys():
        prefix = options["prefix"]
    else:
        prefix = ""
    with open(input_file,"r") as csvfile:
        reader = csv.reader(csvfile)
        with open(output_file,"w") as glmfile:
            for row in reader:
                glmfile.write(f"#define {prefix}{row[0]}={row[1]}\n")
