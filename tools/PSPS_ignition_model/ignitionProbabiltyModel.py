import geopandas
from scipy.special import expit
from sklearn.metrics import RocCurveDisplay
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.datasets import make_classification
from sklearn.ensemble import RandomTreesEmbedding
from sklearn.pipeline import make_pipeline
from sklearn.linear_model import LogisticRegression
from operator import index
import pandas as pd
import numpy as np
import os
from os import path
from matplotlib import pyplot as plot
import seaborn as sns
import rasterio
from elapid import geo, models, utils

######## Load CSV Data ############

windData_path = os.path.join(os.getcwd(), 'Data/PGE_VegFire_windData.csv')
windData = pd.read_csv(windData_path)
windData = windData[~ windData.wind_speed.isna()]
windData = windData[~ windData.wind_speed_max.isna()]
windData = windData[~ windData.wind_speed.str.contains("no data")]
windData.drop(columns=['Unnamed: 0', 'Unnamed: 0.1', 'date', 'year', 'month', 'day', 'time', 'hour', 'minute', 'suppressed_by', 'suppressing_agency','facility_identification', 'other_companies', 'was_there_an_outage', 'date_1', 'time_1',  'contact_from_object', 'facility_contacted', 'unnamed_23', 'material_at_origin_comments'], inplace=True)
windData.wind_speed = windData.wind_speed.astype(float)
windData.wind_speed[windData.wind_speed > 60] = 60
# windData.to_csv(os.path.join(os.getcwd(),'Data/PGE_VegFire_windData_filtered.csv'))

abs_windData_path = os.path.join(os.getcwd(), 'Data/abs_windData.csv')
abs_windData = pd.read_csv(abs_windData_path).drop(columns='Unnamed: 0').rename(columns={'0':'wind_speed'})
abs_windData.dropna(inplace=True)
abs_windData.wind_speed[abs_windData.wind_speed > 60] = 60


num_prescence =windData.shape[0]
num_abs =abs_windData.shape[0]

# ########Load MaxEnt Raster Features#######
# vector_path = "GIS/PGE_veg_ignitionpoints.shp"
# vegCover_mask_path = os.path.join(os.getcwd(), 'GIS/treeheight_20m_mask.tif')
# vegCover_path = '/Users/kamrantehranchi/Documents/GradSchool/Research/landfire_CaliforniaVegetation/LF2016_EVH_200_CONUS/LC16_EVH_200.tif'
# output_model_path = "/home/slug/ariolimax-claifornicus-model.ela"

# # src = rasterio.open(vegCover_mask_path)
# # plot.imshow(src.read(1), cmap='pink')

# # sample the raster values for background point locations
# pseudoabsence_points = geo.sample_raster(vegCover_mask_path, count=num_abs)

# # read the raster covariates at each point location
# presence = geo.annotate(vector_path, vegCover_path)
# pseudoabsence = geo.annotate(pseudoabsence_points, vegCover_mask_path)

# presence.wind_speed[presence.wind_speed > 60] = 60


# # merge the datasets into one dataframe
# pseudoabsence['presence'] = 0
# presence['presence'] = 1
# merged = presence.append(pseudoabsence).reset_index(drop=True)
# merged.wind_speed.iloc[num_prescence:] = abs_windData['wind_speed'].values

# x = merged.drop(columns=['presence'])[['wind_speed','b1']]
# y = merged['presence']
# x.fillna(0,inplace=True)


####### Test Train Splits ############
y_true = np.ones(shape=num_prescence)
y_false = np.zeros(shape=num_abs)
y = pd.DataFrame(np.append(y_true, y_false), columns=['label'])

x_true = windData.wind_speed.values
x_false = abs_windData['wind_speed'].values
x = pd.DataFrame(np.append(x_true, x_false), columns=['wind_speed'])

X_full_train, X_test, y_full_train, y_test = train_test_split(x, y, test_size=0.7, random_state=10)

# ########### MaxEnt Model ######################
# # train the model
# MaxEnt_model = models.MaxentModel()
# MaxEnt_model.fit(X_full_train, y_full_train)
# # elapid.save_object(model, output_model_path)

######## Logistic Model #############
n_estimators = 100
max_depth = 3
random_tree_embedding = RandomTreesEmbedding(n_estimators=n_estimators, max_depth=max_depth, random_state=0)
rt_model = make_pipeline(random_tree_embedding, LogisticRegression(max_iter=1000))
rt_model.fit(X_full_train, y_full_train)

lr_model = LogisticRegression(max_iter=1000)
lr_model.fit(X_full_train, y_full_train)

####### Plotting Model Fits ##############
fig, ax = plt.subplots()
models = [
    ("RT embedding -> LogReg", rt_model),
    ("LogReg", lr_model),]

model_displays = {}
for name, pipeline in models:
    model_displays[name] = RocCurveDisplay.from_estimator(
        pipeline, X_test, y_test, ax=ax, name=name)
_ = ax.set_title("ROC curve")
plt.plot([0, 1], [0, 1], lw=2, ls=":")
plt.show()


# from sklearn.metrics import roc_auc_score
# y_prob = MaxEnt_model.predict(X_test)
# macro_roc_auc_ovo = roc_auc_score(y_test, y_prob, multi_class="ovo", average="macro")
# weighted_roc_auc_ovo = roc_auc_score(
#     y_test, y_prob, multi_class="ovo", average="weighted")
# macro_roc_auc_ovr = roc_auc_score(y_test, y_prob, multi_class="ovr", average="macro")
# weighted_roc_auc_ovr = roc_auc_score(
#     y_test, y_prob, multi_class="ovr", average="weighted")

# print(
#     "One-vs-One ROC AUC scores:\n{:.6f} (macro),\n{:.6f} "
#     "(weighted by prevalence)".format(macro_roc_auc_ovo, weighted_roc_auc_ovo)
# )

# import pickle
# # save the model to disk
# filename = os.path.join(os.getcwd(), 'Data/LogRegModel.sav')
# pickle.dump(lr_model, open(filename, 'wb'))
 







# # plot logistic fit curve
# plt.figure()
# plt.scatter(x.wind_speed.values.ravel(), y, color="black", zorder=20)
# X_test = np.linspace(-5, 80, 300)
# loss = expit(X_test * lr_model.coef_ + lr_model.intercept_).ravel()
# plt.plot(X_test, loss, color="red", linewidth=2)
# plt.ylabel("Probability of Ignition")
# plt.xlabel("WindSpeed")
# plt.show()
