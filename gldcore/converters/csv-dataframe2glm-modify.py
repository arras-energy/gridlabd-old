"""Convert a CSV configuration list a GLM config file
"""
import csv, pandas

def convert(input_file, output_file, options={}):
    data = pandas.read_csv(input_file)
    print(data)
    with open(output_file,"w") as glmfile:
        for obj in data.iteritems():
            print(obj)
            for field in data.columns:
                if field not in ["id","class","name"]:
                    glmfile.write(f"modify {obj['name']}.{field} \"{obj[field]}\";\n")

if __name__ == '__main__':
    convert("/tmp/read.csv","/tmp/read.glm")
