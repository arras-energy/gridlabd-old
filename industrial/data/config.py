enduses = {
	"Conventional Boiler Use" : {"B":0.1,"Z":0.9}, # heaters and pumps
	"CHP and/or Cogeneration Process" : {"B":0.5,"C":0.5}, # pumps and fans
	"Process Heating" : {"B":0.1,"Z":0.9}, # heaters and pumps
	"Process Cooling and Refrigeration" : {"A":0.8,"B":0.1,"C":0.1}, # compressors, pumps, and fans
	"Machine Drive" : {"B":1.0}, # machine drives
	"Electro-Chemical Processes" : {"A":0.4,"B":0.05,"C":0.05,"Z":0.5}, # compressors, pumps, fans, and heaters
	"Other Process Use" : {"B":1.0},
	"Facility HVAC (g)" : {"A":0.4,"B":0.05,"C":0.05,"Z":0.5},
	"Facility Lighting" : {"E":0.8,"I":0.2},
	"Other Facility Support" : {"E":1.0},
	"Onsite Transportation" : {"B":1.0},
	"Other Nonprocess Use" : {"D":0.1,"Z":0.8,"I":0.1},
	"End Use Not Reported" : {"D":0.1,"Z":0.8,"I":0.1},
}
mecs_csv = "../mecs_data.csv"
nerc_csv = "../nerc_data.csv"
naics_csv = "../naics_data_file.csv"