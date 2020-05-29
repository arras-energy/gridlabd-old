
'''
There are 3 ways to set thresholds.
1) Read the user-inputted value from the config.csv
    a. Into main .py function using csv converter -OR-
    b. Directly into this .py file (can tailor csv more, but inaccessible outside)
2) Read a default values from the config.csv

Load default values first. Load modifies (if necessary). Just different csv files. This is an overwrite, not delete.
first do input defaults, then input user settings.
3) Read in defaults based on library properties

These will be TWO choices in the config.csv
'''



'''
ON_INIT
1) Based on user-selected option, defines thresholds for each parameter.
2) Creates a general dictionary of properties to check for each class and their thresholds.
3) Populates a global master dictionary with each object, properties to check, and their thresholds.
'''
import pandas as pd

def on_init(t):
    print("initializing")
    #If Option 1, global thresholds will have already been set through csv converter
    #TODO: Add 'ica_' tag to all globals. Option on converter to set a prefix.
  
    #If Option 2, read config file directly into this .py script. Create a global for each entry.
    # config_globals = pd.read_csv("config/ica_config.csv")
    # for index in range(len(config_globals)):
    #     gridlabd.set_global("ica_" + config_globals.iloc[index, 0], str(config_globals.iloc[index, 1]))
  
    # #Create a dict of all ICA globals with info on how to find library values
    # #RATING: set the threshold as a % of the max rating. DEVIATION: set the threshold as a +-% from the nominal rating.
    # ica_globals_dict = {'ica_underground_line':{'configuration':1, 'rating_summer_continuous':'rating','rating_winter_continuous':'rating'},\
    #                 'ica_overhead_line':{'configuration':1, 'rating_summer_continuous':'rating', 'rating_winter_continuous':'rating'},\
    #                 'ica_transformer':{'configuration':1, 'power_rating':'rating', 'powerA_rating':'rating', 'powerB_rating':'rating', 'powerC_rating':'rating', 'primary_voltage':'deviation', 'secondary_voltage':'deviation'},\
    #                 'ica_regulator':{'configuration':1, 'primary_voltage':'none', 'secondary_voltage':'none'},\
    #                 'ica_substation':{'configuration':0, 'nominal_voltage':'deviation'},\
    #                 'ica_triplex_meter':{'configuration':0, 'nominal_voltage':'deviation'},\
    #                 'ica_meter':{'configuration':0, 'nominal_voltage':'deviation'}}
   

    # global_list = gridlabd.get("globals")
   
  
    # for g in global_list:
    #     #Only consider globals specific to ICA tests
    #     if 'ica_' in g: #TODO: restrict ica_ to the front of the string
    #         val = gridlabd.get_global(g)
    #         #TODO: Check that value makes sense
    #         #Only consider globals where user has input a %
    #         if '%' in val: #TODO: restrict to end of string
    #             #QUESTION: Can you call a property gridlabd.get_value(transformer, power_in.real)? Would need to parse it into something py recognizes. Might be code for this.
    #             global_string = g.split('.', maxsplit=1)
    #             obj = global_string[0]
    #             prop = global_string[1]

    #             percent = gridlabd.get_global(g).strip('%')/100    
    #             obj_dict = ica_globals_dict.get(obj)
    #             #TODO: Make sure obj names all either do or do not have ica_
               
    #             #First, get the library value of the given property.
    #             if obj_dict.get('configuration') == 0:
    #                 #QUESTION: More efficient way than str->float->str?
    #                 lib_prop = float(gridlabd.get_value(obj, prop))
                   
    #             else:
    #                 config = gridlabd.get_value(obj, 'configuration')
    #                 lib_prop = float(gridlabd.get_value(config, prop))

    #             #Then, modify the threshold to be a % or a +- range of that property.
    #             if obj_dict.get(prop) == 'rating':
    #                 gridlabd.set_global(obj, str(lib_prop*percent))
                   
    #             elif obj_dict.get(prop) == 'deviation':
    #                 gridlabd.set_global(obj+'max', str(lib_prop*(1.0+percent)))
    #                 gridlabd.set_global(obj+'min', str(lib_prop*(1.0-percent)))
                   
    #             else:
    #                 print('%s should not have a percentage as a threshold.' % obj)


           
    # #Get a list of all objects used in model               
    # objects = gridlabd.get("objects")
    # #obj_dict will contain all properties/threshold to check for an object 
    # obj_dict = {}
    # #master_dict will contain all objects as keys, with associated obj_dicts as values
    # master_dict = {}
 
    # for obj in objects:
    #     obj_dict.clear()
    #     obj_class = gridlabd.get_object(obj)['class']
    #     #Create a list of globals (obj_global_list) that contain this class name
    #     #For each global, append a key:value (property:threshold) within that obj's dict
    #     for g in obj_global_list:
    #         obj_dict.update({'property':'threshold'})
          
    #     master_dict.update({obj:obj_dict})

    return True
      

'''
ON_COMMIT
For each key in global dictionary, get the value for each property and compare it to the threshold.
If threshold is exceeded, record the object, property, and value, and exit.
'''

def on_commit(t):
    return True

