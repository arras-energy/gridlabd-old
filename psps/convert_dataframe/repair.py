import numpy as np 
import matplotlib.pyplot as plt
import pandas as pd
import gridlabd

print('Hello from Gridlabd repair file')

globals = gridlabd.get('globals')
modules = gridlabd.get('modules')
classes = gridlabd.get('classes')
objects = gridlabd.get('objects')

# for object_name in objects:
#     try:
#         object = gridlabd.get_object(object_name)
#         object_to = object['to']
#         object_from = object['from']
#         if object_to is not None and object_from is not None:
#             object_class = object['class']
#             print(object_name,object_class,object_to,object_from)
#     except:
#         pass
   
# print('finished')