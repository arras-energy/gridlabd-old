
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
#import pandas as pd

def on_init(t):
    print("INITIALIZING!")
#    #If Option 1, global thresholds will have already been set through csv converter
#    
#    #If Option 2, read config file directly into this .py script. Create a global for each entry.
#    if gridlabd.get_global('ica_option') == 2:
#        config_globals = pd.read_csv("ica_config.csv")
#        for index in range(len(config_globals)):
#            gridlabd.set_global("ica_" + config_globals['Name'].iloc[index],config_globals['Value'].iloc[index])
#    
#    #Check whether any ICA-specific globals are percentages instead of fixed values 
#    global_list = gridlabd.get("globals")
#    library_list = []    
#    
#    #Key = object, Value = dictionary where key = property, value = type
#    mapping_dict = {'underground_line':{'configuration':1,'rating_summer_continuous':'rating','rating_winter_continuous':'rating'},'overhead_line':{'configuration':1,'rating_summer_continuous':'rating','rating_winter_continuous':'rating'}}
#    for g in global_list:
#        if "ica" in g:
#            val = gridlabd.get_global(g)
#            #TODO: Check that value makes sense
#            if "%" in val:
#                percent = gridlabd.get_global(g).strip('%')/100
#                global_string = g.split('.')
#                obj = global_string[0].replace('ica','')
#                prop = global_string[1]
#                obj_dict = mapping_dict.get(obj)
#                if obj_dict.get('configuration') == 0:
#                    if obj_dict.get(prop) == 'rating':
#                        pass
#                    else:
#                        pass
#                if obj_dict.get('configuration') == 1:
#                    if obj_dict.get(prop) == 'rating':
#                        pass
#                    else:
#                        pass
#                    
#
#    
#    #If the user set any thresholds as percentages, use object libraries to properly define thresholds
#    if len(library_list) > 0:
#        #QUESTION: Should we use this method to set thresholds ONLY for properties where input was a %? Yes.
#        for l in library_list:
#            global_string = l.split('.')
#            obj = global_string[0]
#            prop = global_string[1]
#            percent = gridlabd.get_global(l).strip('%')/100
#            config_list = ['underground_line','overhead_line','transformer','regulator']
#            rating_list = ['underground_line','overhead_line','transformer']
#            
#            if obj in config_list:
#                config = gridlabd.get_value(obj,'configuration')
#                if obj in rating_list:
#                    #Set max rating
#                    #TODO: Clean this up. String -> float -> string
#                    gridlabd.set_global(l,str(float(gridlabd.get_value(config,prop))*percent))
#                else:
#                    #Set max voltage
#                    #Set min voltage
#                    #QUESTION: Can set_global create, not just modify? We need to set a max AND a min. Alternatively, can use % to calculate max and min in on_commit.
#                    pass
#            else:
#                #Set max voltage
#                #Set min voltage
#                pass
#            
#            
#            
#            
#                
#    
#    #create a dictionary of properties to check for each class & their thresholds
#    #example below for a single class:
#    gen_dict = {'substation':{'current_A':swing_I_thresh,'current_B':swing_I_thresh,'current_C':swing_I_thresh}}
#
#    #create master dict
#    #key = obj; val = dict {property1:thresh1,property2:thresh2}
#    objects = gridlabd.get("objects")
#    for obj in objects:
#        #only get the classes of interest
#        obj_class = gridlabd.get_object(obj)['class']
#        if obj_class in gen_dict.keys():
#            #append to master dict
#            obj_dict.update({obj:gen_dict.get(obj_class)})

    return True
    
        

'''
ON_COMMIT
For each key in global dictionary, get the value for each property and compare it to the threshold.
If threshold is exceeded, record the object, property, and value, and exit.
'''

def on_commit(t):
    return True

