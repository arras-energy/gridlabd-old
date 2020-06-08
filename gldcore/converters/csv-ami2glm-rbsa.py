"""Convert AMI data in CSV to an RBSA GLM object
"""
import pandas

def convert(input_file, output_file, options={}):
    with open(output_file,"w") as glm:
        glm.write("""object rbsa {
    floor_area "0.0";
    filename "%s";
    composition "";
    total_real_power "0.0";
    total_reactive_power "0.0";
    total_power_A "0+0i";
    total_power_B "0+0i";
    total_power_C "0+0i";
    weather "";
    tariff "";
}
""" % input_file)