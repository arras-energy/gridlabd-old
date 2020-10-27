"""Convert ICA data sheet in CSV to an ICA GLM template parameter set
"""
import csv

def convert(input_file, output_file, options={}):
    with open(input_file,"r") as csvfile:
        reader = csv.reader(csvfile)
        with open(output_file,"w") as glmfile:
            for row in reader:
                glmfile.write(f"#define {row[0]}=\"{row[1]}\"\n")
