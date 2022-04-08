import pandas as pd 
import string
import math
import re
import gridlabd
import numpy as np

"""Convert XLS SPIDAcalc pole data to geodata

SYNOPSIS

	GLM:
		#convert SPIDACALC.xls GEODATA.csv -f xls-spida -t csv-geodata

DESCRIPTION

	This converter extracts pole geodata from a SPIDAcalc pole asset report spreadsheet
	and generates a GriDLAB-D geodata CSV file.
	
"""
import pandas

default_options = {
	"precision" : 2,
}

def convert(input_file, output_file, options={}):

	# Round to nearest hundreth decimal place if value has more decimal places than that.

	for name, value in default_options.items():
		globals()[name] = value
	for name, value in options.items():
		if name not in default_options.keys():
			raise Exception("option '{name}={value}' is not valid")
		globals()[name] = valuent

	# Read all the sheets in the .xls file 
	df = pd.read_excel(input_file, sheet_name=None,)  

	# First do operations on the sheet 'Design - Pole.'
	df_current_sheet = df['Design - Pole'].copy()
	new_header_index = df_current_sheet.iloc[:, 0].first_valid_index()
	new_header = df_current_sheet.iloc[new_header_index-1]
	df_current_sheet = df_current_sheet[new_header_index:]
	df_current_sheet.columns = new_header
	df_current_sheet.index = range(len(df_current_sheet.index))
	df_current_sheet.columns.name = None

	# Drop unneeded columns 
	df_current_sheet.drop(['Owner', 'Foundation', 'Ground Water Level',],axis=1,inplace=True)

	# Parse necessary columns into a format supported by Gridlabd.
	parse_column(df_current_sheet, 'Lean Angle', parse_angle)
	parse_column(df_current_sheet, 'Lean Direction', parse_angle)
	parse_column(df_current_sheet, 'Length', parse_length)
	parse_column(df_current_sheet, 'GLC', parse_circumference_to_diamater)
	parse_column(df_current_sheet, 'AGL', parse_length)
	parse_column(df_current_sheet, 'Effective Allowable Stress', parse_pressure) # for sec data
	parse_column(df_current_sheet, 'GPS Point', check_lat_long)


	# Prepare GPS Point column for splitting and split value into lat and long. 
	df_current_sheet['GPS Point'] = df_current_sheet['GPS Point'].apply(lambda x: str(x).replace('', ',') if str(x) == '' else str(x))
	df_current_sheet[['latitude','longitude']] = df_current_sheet['GPS Point'].str.split(',', expand=True)

	# Subtract agl from length to get depth. 
	for row in range(0,len(df_current_sheet['AGL'])):
		try: 
			df_current_sheet.at[row,'AGL'] = subtract_length_columns(str(df_current_sheet.at[row,'Length']), str(df_current_sheet.at[row,'AGL']), 'Length', 'AGL', row)

		except ValueError as e: 
			df_current_sheet.at[row,'AGL'] = ""

	# Rename columns to its corresponding column name in Gridlabd.
	# I believe class in the file is referring to grade, so it is renamed. 
	df_current_sheet.rename(columns = {np.nan : 'name', 'Lean Angle': 'tilt_angle', 
		'Lean Direction': 'tilt_direction', 'Effective Allowable Stress': 'fiber_strength',\
		 'Length' : 'pole_length', 'GLC' : 'ground_diameter', 'AGL' : 'pole_depth',\
		  'Class': "grade"}, inplace=True) # for sec data

	# Split GPS Point into longitude and latitude, then parse.
	# Remove original GPS Point column
	df_current_sheet.drop(columns = {'GPS Point'},axis=1,inplace=True) # sce data

	# Split the dataframe based on properties of pole_config and pole_library.
	df_pole_config = df_current_sheet[['pole_length', 'pole_depth', 'ground_diameter', 'fiber_strength']].copy()
	df_pole_library = df_current_sheet[['tilt_angle', 'tilt_direction', 'latitude', 'longitude']].copy()

	# Specify class of the properties.
	df_pole_config.loc[:,'class'] = 'powerflow.pole_configuration'
	df_pole_library.loc[:,'class'] = 'powerflow.pole'

	# Additional properties for each class. These values are just for testing purposes for now. 
	pole_configuration_name = []
	pole_name = []
	for i in range(len(df_current_sheet["name"])):
		pole_configuration_name.append(f"pole_configuration_{df_current_sheet['name'][i]}")
		pole_name.append(f"pole_{df_current_sheet['name'][i]}")

	df_pole_config.loc[:,'class'] = 'pole_configuration'
	df_pole_library.loc[:,'class'] = 'pole'
	df_pole_config.loc[:,'name'] = pole_configuration_name
	df_pole_library.loc[:,'configuration'] = pole_configuration_name
	df_pole_library.loc[:,'name'] = pole_name
	df_pole_library.loc[:,'wind_speed'] = '0 m/s'
	df_pole_library.loc[:,'flags'] = 'NONE'
	df['Design - Pole']= pd.concat([df_pole_config, df_pole_library], axis=0, ignore_index=True)	


	# Keep track of final df to output at the end. 
	df_final = df['Design - Pole'].copy()

	# Move class column to the first column. May not be necessary. 
	class_column = df_final.pop('class')
	df_final.insert(0, 'class', class_column)

	# Create the intermediate csv files. May not be necessary. 

	df_final.reset_index(drop=True, inplace=True)

	# Create final csv file. 
	df_final.to_csv(output_file, index=False)

def parse_angle(cell_string, current_column, current_row):
	"""Parse a string to get a string with angle in a unit that is supported by Gridlabd

	Additional acceptable units can be added to angle_units. Invalid inputs raise a ValueError. 

    Keyword arguments:
    cell_string -- the string to be parsed (presumably from a cell)
    current_column -- the column of the cell it is parsing. For more descriptive ValueErrors
    current_row -- the row of the cell it is parsing. For more descriptive ValueErrors
    """
	angle_units = {	
    "°" : "deg",
    "deg" : "deg",
    "rad" : "rad",
    "grad" : "grad",
    "quad" : "quad",
    "sr" : "sr"
	}
	value = re.search("\d+[\.]?[\d+]*", cell_string)
	if cell_string == "nan":
		raise ValueError(f'The cell column: {current_column}, row {current_row} is empty. Please enter a value.')
	output_unit = ""
	for unit in angle_units.keys():
		if unit in cell_string:
			output_unit = angle_units[unit]
			break
	if output_unit == "": 
		raise ValueError(f'Please specify valid units for {cell_string} in column: {current_column}, row {current_row}.')
	elif value == None: 
		raise ValueError(f'Please specify valid value for {cell_string} in column: {current_column}, row {current_row}.')
	else:
		return value.group() + " " + output_unit
	

def parse_length(cell_string, current_column, current_row):
	"""Parse a string to get a string with length in a unit that is supported by Gridlabd

	Additional acceptable units can be added to length_units and length_conversion with its corresponding conversion value to degrees. 
	Supports multiple units in one string. Invalid inputs raise a ValueError. 

    Keyword arguments:
    cell_string -- the string to be parsed (presumably from a cell)
    current_column -- the column of the cell it is parsing. For more descriptive ValueErrors
    current_row -- the row of the cell it is parsing. For more descriptive ValueErrors
    """
	INCH_TO_FEET = 0.0833
	UNIT_TO_UNIT = 1.0 
	YARD_TO_FEET = 3.0
	MILE_TO_FT = 5280.0 
	FEET_TO_INCH = 12.0
	YARD_TO_INCH = 36.0
	MILE_TO_INCH = 6360.0
	MILE_TO_YARD = 1760.0
	# Handle values and units in different orders.
	length_units = {	
	"'" : "ft",
    '"' : "in",
    "in" : "in",
    "inch" : "in",
    "inches" :'in',
    "feet" : "ft",
    "ft" : "ft",
    "foot" : "ft",
    "yd" : "yd",
    "yard" : "yd",
    "mile" : "mile",
    "mi" : "mile"
    }
    # Map to a dictionary of conversion rates of different units to the key of length_conversions.
	length_conversions = {	
	"ft" : {"in" : INCH_TO_FEET, "ft" :  UNIT_TO_UNIT, "yd" : YARD_TO_FEET, "mile" : MILE_TO_FT},
    "in" : {"in" : UNIT_TO_UNIT,"ft": FEET_TO_INCH,  "yd": YARD_TO_INCH, "mile" : MILE_TO_INCH},
    "yd" : {"in": 1/YARD_TO_INCH, "ft": 1/YARD_TO_FEET, "yd": UNIT_TO_UNIT, "mile" : MILE_TO_YARD},
    "mile" : {"in": 1/MILE_TO_INCH, "ft": 1/MILE_TO_FT, "yd": 1/MILE_TO_YARD, "mile" : UNIT_TO_UNIT}
    }
	if cell_string == "nan":
		raise ValueError(f'The cell column: {current_column}, row {current_row} is empty. Please enter a value.')

	# Keeps track of the units that are in the cell. Whichever unit is listed first in length_units AND is present in the string is the unit that will be used as the output.
	cell_units = []
	units_positions = []
	num_strings = []
	for key in length_units.keys():
		if key in cell_string:
			cell_units.append(length_units[key])
			units_positions.append(cell_string.find(key))
	last_pos = 0
	for units_position in units_positions:
		num_strings.append(cell_string[last_pos:units_position])
		last_pos = units_position+1

	if len(cell_units) == 0: 
		raise ValueError(f'Please specify valid units for {cell_string} in column: {current_column}, row {current_row}.')

	if len(num_strings) != len(cell_units):
		raise ValueError(f'Please make sure there are the same number of numbers and units for value {cell_string} in column: {current_column}, row {current_row}')

	cell_numbers = re.findall('\d+[\./]?[\d+]*', cell_string)

	# Convert all the units and values in the cell to one unit and value.
	total_cell_value = 0
	convert_to = length_conversions[cell_units[0]]
	for i in range(len(num_strings)):
		num_unit_strings = re.findall('\d+[\./]?[\d+]*', num_strings[i])
		total_num_value = 0
		for num_unit_string in num_unit_strings:
			if "/" in num_unit_string:
				total_num_value += float(num_unit_string.split('/')[0])/float(num_unit_string.split('/')[1])
			else:
				total_num_value += float(num_unit_string)
		total_cell_value += total_num_value * convert_to[cell_units[i]]
		

	return str(round(total_cell_value,precision)) + " " + cell_units[0]

def parse_pressure(cell_string, current_column, current_row):
	"""Parse a string to get a string with pressure in a unit that is supported by Gridlabd

	Additional acceptable units can be added to pressure_units. Invalid inputs raise a ValueError. 

    Keyword arguments:
    cell_string -- the string to be parsed (presumably from a cell)
    current_column -- the column of the cell it is parsing. For more descriptive ValueErrors
    current_row -- the row of the cell it is parsing. For more descriptive ValueErrors
    """
	pressure_units = {
	"psi" : "psi",
	"lb/in²" : "psi",
	"bar" : "bar",
	"atm" : "atm", 
	}
	value = re.search("\d+[\.]?[\d+]*", cell_string)
	if cell_string == "nan":
		raise ValueError(f'The cell column: {current_column}, row {current_row} is empty. Please enter a value.')
	output_unit = ""
	for unit in pressure_units.keys():
		if unit in cell_string:
			cell_string = cell_string.replace(unit,pressure_units[unit])
			output_unit = pressure_units[unit]
			break
	if output_unit == "": 
		raise ValueError(f'Please specify valid units for {cell_string} in column: {current_column}, row {current_row}.')
	elif value == None: 
		raise ValueError(f'Please specify valid value for {cell_string} in column: {current_column}, row {current_row}.')
	else:
		return  value.group() + " " + output_unit
	
def parse_column(df_current_sheet, current_column, parsing_function):
	"""Parse each cell in the column with a function 
	For invalid inputs, resulting cell will be ''.
	Allows empty cells ('nan'). Can remove the if statement if an error should be raised for empty cells. 
    Keyword arguments:
    current_column -- the string to be parsed (presumably from a cell)
    parsing_function -- the function to be called for each cell
    """
	for row in range(len(df_current_sheet[current_column])):
		current_string = str(df_current_sheet.at[row,current_column])
		if (current_string == 'nan'):
			df_current_sheet.at[row,current_column] = ''
		else:
			try: 
				df_current_sheet.at[row,current_column] = parsing_function(str(df_current_sheet.at[row,current_column]),current_column,row)
			except ValueError as e: 
				df_current_sheet.at[row,current_column] = ''		

def subtract_length_columns(minuend_string, subtrahend_string, minuend_column, subtrahend_column,current_row):
	"""Subtract the lengths of strings contained in two cells 
	
	Assumes the units are the same for the two columns. 

    Keyword arguments:
    minuend_string -- the string containing the minuend 
    subtrahend_string -- the string containing the subtrahend 
    minuend_column -- the name of the column containing the minuend cell. For more descriptive ValueErrors
    subtrahend_column -- the name of the column containing the subtrahend cell. For more descriptive ValueErrors
    current_row -- the row number containing the minuend cell. For more descriptive ValueErrors
    """
	length_units = {	
	"'" : "ft",
    '"' : "in",
    "in" : "in",
    "inch" : "in",
    "feet" : "ft",
    "ft" : "ft",
    "foot" : "ft",
    "yd" : "yd",
    "yard" : "yd",
    "mile" : "mile",
    "mi" : "mile"
    }
	if minuend_string == "nan":
		raise ValueError(f'The cell column: {minuend_column}, row {current_row} is empty. Please enter a value.')
	elif subtrahend_string == "nan": 
		raise ValueError(f'The cell column: {subtrahend_column}, row {current_row} is empty. Please enter a value.')
	output_unit = ""
	for unit in length_units.keys():
		if unit in minuend_string and unit in subtrahend_string:
			output_unit = length_units[unit]
			break
	if output_unit == "": 
		raise ValueError(f'Please provide {minuend_column} and {subtrahend_column}, row {current_row} with the same units.')
	else:
		return str(round(float(re.search("^[1-9]\d*(\.\d+)?", minuend_string).group()) - float(re.search("^[1-9]\d*(\.\d+)?", subtrahend_string).group()),precision)) + " " + output_unit


def check_lat_long(cell_string, current_column, current_row):
	"""Parse the lat/long value
	
    Keyword arguments:
    cell_string -- the string to be parsed (presumably from a cell)
    current_column -- the column number containing the cell_string. For more descriptive ValueErrors
    current_row -- the row number containing the cell_string. For more descriptive ValueErrors
    """
	CONTIGUOUS_US_LOWER_LAT = 23
	CONTIGUOUS_US_UPPER_LAT = 50
	CONTIGUOUS_US_LOWER_LONG = -130
	CONTIGUOUS_US_UPPER_LONG = -64
	ALASKA_LOWER_LAT = 55
	ALASKA_UPPER_LAT = 71
	ALASKA_LOWER_LONG = -168
	ALASKA_UPPER_LONG = -130
	if cell_string == "nan":
		raise ValueError(f'The cell column: {current_column}, row {current_row} is empty. Please enter a value.')
	else:
		lat_long = re.findall('[\-]?\d+[\.]?[\d+]*', cell_string)
		if (len(lat_long) != 2):
			raise ValueError(f'The cell value {cell_string} in {current_column}, row {current_row} is not valid. Please provide GPS Point in this format: lat , long')	
		try: 
			lat = float(lat_long[0])
			longitude = float(lat_long[1])
		except ValueError: 
			raise ValueError(f'The cell value {cell_string} in {current_column}, row {current_row} is not valid. Please provide GPS Point in this format: float , float')
		if ((lat < CONTIGUOUS_US_UPPER_LAT and longitude > CONTIGUOUS_US_LOWER_LONG) and (lat > CONTIGUOUS_US_LOWER_LAT and longitude < CONTIGUOUS_US_UPPER_LONG) or 
		((lat > ALASKA_LOWER_LAT and  longitude < ALASKA_UPPER_LONG ) and (lat < ALASKA_UPPER_LAT and longitude > ALASKA_LOWER_LONG))):
			return str(lat) + ","  + str(longitude)
		else:
			raise ValueError(f'The cell value {cell_string} in {current_column}, row {current_row} is not in North America. Please enter a valid value.')
		 

def parse_circumference_to_diamater(cell_string, current_column, current_row):
	"""Parse the circumference value into its diameter value 

    Keyword arguments:
    cell_string --  the string to be parsed (presumably from a cell)
    current_column -- the column number containing the cell_string. For more descriptive ValueErrors
    current_row -- the row number containing the cell_string. For more descriptive ValueErrors
    """
	try: 
		string_value = parse_length(cell_string, current_column, current_row).split(" ")
		return str(round(float(string_value[0])/math.pi, precision)) + " " + string_value[1]
	except ValueError as e: 
		raise e

def parse_space_to_underscore(cell_string, current_column, current_row):
	if cell_string == "nan":
		raise ValueError(f'The cell column: {current_column}, row {current_row} is empty. Please enter a value.')
	return cell_string.replace(' ','_')
