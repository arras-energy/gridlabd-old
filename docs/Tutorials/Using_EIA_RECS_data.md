::: {.cell .markdown}
The principal learning objectives of this tutorial are the following:

1.  Model homes that have the similar characteristics to home in the
    Energy Information Agency\'s Residential Energy Consumption Survey
    \[1\].

2.  Create models that use correlated random properties

Some the skills you will learn in this tutorial include:

1.  How to load and select EIA RECS data.

2.  How to refer to Python variables in GLM models.

3.  How to run post-simulation analysis

4.  How to embed python code in a GLM model

5.  How to refer to GLM model variables into Python code

6.  How to plot histograms of model properties

The principal source of data for this tutorial is the RECS 2015
Microdata \[2\]. It contains the raw survey data from 2015. This
microdata can be downloaded using the `eia_recs.py` module installed in
the GridLAB-D shared folder from the `python_extras` library in the
GridLAB-D source code.
:::

::: {.cell .markdown}
# Learning 1: GLM objects with a similar distribution

This first section will illustrate how to obtain a single characteristic
from the RECS microdata database and use it to generate a collection of
`house` objects that have a similar distribution.

You can load the RECS microdata for the census division that contains
the state of California using the following code:
:::

::: {.cell .code execution_count="9"}
``` python
import eia_recs
fips = eia_recs.Microdata.get_division("CA")
codes = eia_recs.Microdata.get_codes(**fips)
recs = eia_recs.Microdata()
data = recs[recs["DIVISION"]==codes["division"]]

data
```

::: {.output .stream .stdout}
    5686
:::

::: {.output .execute_result execution_count="9"}
```{=html}
<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>DOEID</th>
      <th>REGIONC</th>
      <th>DIVISION</th>
      <th>METROMICRO</th>
      <th>UATYP10</th>
      <th>TYPEHUQ</th>
      <th>ZTYPEHUQ</th>
      <th>CELLAR</th>
      <th>ZCELLAR</th>
      <th>BASEFIN</th>
      <th>...</th>
      <th>ZELAMOUNT</th>
      <th>NGXBTU</th>
      <th>PERIODNG</th>
      <th>ZNGAMOUNT</th>
      <th>FOXBTU</th>
      <th>PERIODFO</th>
      <th>ZFOAMOUNT</th>
      <th>LPXBTU</th>
      <th>PERIODLP</th>
      <th>ZLPAMOUNT</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>30</th>
      <td>10031</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>98.88</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>63</th>
      <td>10064</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>R</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>1</td>
      <td>NaN</td>
      <td>-2</td>
      <td>-2</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>5</td>
      <td>1</td>
    </tr>
    <tr>
      <th>118</th>
      <td>10119</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>99.77</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>135</th>
      <td>10136</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>85.58</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>180</th>
      <td>10181</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>84.81</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>...</th>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
      <td>...</td>
    </tr>
    <tr>
      <th>5602</th>
      <td>15603</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>84.57</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>5622</th>
      <td>15623</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>99.40</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>5637</th>
      <td>15638</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>99.45</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
    <tr>
      <th>5651</th>
      <td>15652</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>84.41</td>
      <td>1</td>
      <td>0</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>5</td>
      <td>1</td>
    </tr>
    <tr>
      <th>5679</th>
      <td>15680</td>
      <td>4</td>
      <td>9</td>
      <td>METRO</td>
      <td>U</td>
      <td>2</td>
      <td>0</td>
      <td>0</td>
      <td>0</td>
      <td>-2</td>
      <td>...</td>
      <td>0</td>
      <td>NaN</td>
      <td>-2</td>
      <td>-2</td>
      <td>137.45</td>
      <td>-2</td>
      <td>-2</td>
      <td>91.33</td>
      <td>-2</td>
      <td>-2</td>
    </tr>
  </tbody>
</table>
<p>242 rows × 759 columns</p>
</div>
```
:::
:::

::: {.cell .markdown}
**Skill 1**: The EIA RECS microdata is available from the `eia_recs.py`
module that is installed as part of GridLAB-D Python Extras. The
`Microdata` class provides direct access to the entire survey dataset,
which includes roughly 750 columns of data for roughly 5700 residential
buildings across the US. The `get_division()` and `get_codes()` methods
determine the census region, division, and FIPS codes to the state
given. These codes can be used to select records, as shown in the pandas
selection used to obtain the `data`.
:::

::: {.cell .markdown}
The `data` object now contains all the records needed to obtain home
statistics for census division 9, e.g., the parameters of a log-normal
distribution of floor area:
:::

::: {.cell .code execution_count="2"}
``` python
import numpy
avg = numpy.log(data["TOTSQFT_EN"]).mean()
std =  numpy.log(data["TOTSQFT_EN"]).std()

data["TOTSQFT_EN"].plot(kind="hist",bins=range(0,10000,500),grid=True)
avg,std
```

::: {.output .execute_result execution_count="2"}
    (7.4068847429917275, 0.5372963497324936)
:::

::: {.output .display_data}
![](c096bbb1ab1e56c7afda83e786c45b1ed8e313e4.png)
:::
:::

::: {.cell .markdown}
These parameters can then be used to generate a collection of `house`
objects with the same distribution, e.g., 1000 homes with the same
geometric average and standard deviation:
:::

::: {.cell .raw}
```{=ipynb}
module residential;
object house:..1000
{
    floor_area random.lognormal(${PYTHON avg},${PYTHON std});
}
```
:::

::: {.cell .markdown}
**Skill 2**: You can embed a python value in your GLM model using the
syntax `${PYTHON name}` where `name` is the python object you wish to
embed. One caveat is that many objects are represented in formats that
are not compatible with GridLAB-D\'s GLM format. As a result, generally
only numbers are represented correctly by default. Other objects such as
strings and lists require special formatting to be represented correctly
in GLM files. This is an advanced topic that will be addressed in
another tutorial.
:::

::: {.cell .markdown}
We can verify the result by outputing this model to a JSON file and
running a python script to generate a histogram of the result.
:::

::: {.cell .raw}
```{=ipynb}
#set savefile=${modelname/.glm/.json}
#on_exit 0 python3 ${modelname/.glm/.py}
```
:::

::: {.cell .markdown}
**Skill 3**: When a GLM file is loaded, you can use
`#on_exit CODE python3 FILENAME.py` command to schedule a
post-processing run of a python script. If the code `0` is used, the
script will run only when the GLM file is loaded (and run if that is
required) successfully. The value `-1` allows you to run the scripts if
the GLM file load fails. Positive codes refers to specific abnormal exit
conditions, such as an error with the command line (1), an error
starting the simulation environment (2), a runtime error (5), and an
initialization error (6).
:::

::: {.cell .markdown}
Let\'s put all this together now, and run the GLM model.
:::

::: {.cell .markdown}
First, here\'s what the GLM file looks like
:::

::: {.cell .code execution_count="3"}
``` python
!cat eia_recs_floorarea.glm
```

::: {.output .stream .stdout}
    // this prevents the simulation from start a clock
    #set compileonly=TRUE
    #set warn=FALSE
    
    // load the RECS data and calculate the floor area statistics
    #begin python
    import eia_recs, numpy
    fips = eia_recs.Microdata.get_division("${region}")
    codes = eia_recs.Microdata.get_codes(**fips)
    recs = eia_recs.Microdata()
    data = recs[recs["DIVISION"]==codes["division"]]
    avg = numpy.log(data["TOTSQFT_EN"]).mean()
    std =  numpy.log(data["TOTSQFT_EN"]).std()
    #end
    
    // create the same number of homes with a similar log-normal distribution
    module residential;
    object house:..${PYTHON len(data)}
    {
        floor_area random.lognormal(${PYTHON avg},${PYTHON std});
    }
    
    // save and plot the results 
    #set savefile=${modelname/.glm/.json}
    #on_exit 0 python3 ${modelname/.glm/.py}
:::
:::

::: {.cell .markdown}
**Skill 4**: The python code is embedded in the GLM file using the
`#begin python` \... `#end` syntax. The body of the code between these
two statement is compiled into GridLAB-D\'s main python context and
available for use during the loading, initialization, and simulation
processes.

**Skill 5**: The python call to
`eia_recs.Microdata.get_division("${region}")` uses the string
`"${region}"`. This syntax causes the GLM loaded to substitute the GLM
global variable `region` in place so that Python receives the value
`"CA"` (assuming the `region` is California).
:::

::: {.cell .markdown}
The python file contains the following code:
:::

::: {.cell .code execution_count="4"}
``` python
!cat eia_recs_floorarea.py
```

::: {.output .stream .stdout}
    import sys, json
    area = []
    with open(sys.argv[0].replace(".py",".json"),"r") as f:
        glm = json.load(f)
        for name, data in glm["objects"].items():
            area.append(float(data["floor_area"].split()[0]))
    import matplotlib.pyplot as plt
    plt.hist(area,20,range=(0,10000))
    plt.xlabel("Floor area (sf)")
    plt.ylabel("Number of homes")
    plt.title(f"Floor areas (N={len(area)})")
    plt.grid()
    plt.savefig(sys.argv[0].replace(".py",".png"))
:::
:::

::: {.cell .markdown}
**Skill 6**: The JSON files generated by the
`#set savefile=FILENAME.json` command above has a specific structure
that allows python scripts to access any property of any object in the
model. In this case, we can all the objects and extract the `floor_area`
property to construct a list that we can plot as a histogram using
`matplotlib`.
:::

::: {.cell .markdown}
The output PNG file is generated and viewed using the following command
:::

::: {.cell .code execution_count="5"}
``` python
!gridlabd eia_recs_floorarea.glm
from PIL import Image
Image.open("eia_recs_floorarea.png")
```

::: {.output .execute_result execution_count="5"}
![](56ee2017acdfa22676bc4fc53f09ddb40e7dd283.png)
:::
:::

::: {.cell .markdown}
# Learning 2: GLM objects with correlatved distributions
:::

::: {.cell .markdown}
In this section we will learn how to generate and verify a correlated
distribution of properties from the RECS dataset, specifically the floor
area and the thermal integrity. Because this is a discrete property, a
discrete distribution method must be used.
:::

::: {.cell .code execution_count="22"}
``` python
import eia_recs, numpy
fips = eia_recs.Microdata.get_division("CA")
codes = eia_recs.Microdata.get_codes(**fips)
recs = eia_recs.Microdata()
data = recs[recs["DIVISION"]==codes["division"]]
ins1 = data[data["ADQINSUL"]==1]
ins2 = data[data["ADQINSUL"]==2]
ins3 = data[data["ADQINSUL"]==3]
avg1 = numpy.log(ins1["TOTSQFT_EN"]).mean()
std1 = numpy.log(ins1["TOTSQFT_EN"]).std()
avg2 = numpy.log(ins2["TOTSQFT_EN"]).mean()
std2 = numpy.log(ins2["TOTSQFT_EN"]).std()
avg3 = numpy.log(ins3["TOTSQFT_EN"]).mean()
std3 = numpy.log(ins3["TOTSQFT_EN"]).std()

ins1["TOTSQFT_EN"].plot(kind="hist",bins=range(0,10000,500),grid=True,title="Insulation levels by square footage")
ins2["TOTSQFT_EN"].plot(kind="hist",bins=range(0,10000,500),grid=True)
ins3["TOTSQFT_EN"].plot(kind="hist",bins=range(0,10000,500),grid=True).legend(["Low","Normal","High"])
```

::: {.output .execute_result execution_count="22"}
    <matplotlib.legend.Legend at 0x11fffff70>
:::

::: {.output .display_data}
![](407cf9a97c98d62036a4ac53e19457c986bb7639.png)
:::
:::

::: {.cell .markdown}
# References

\[1\] Energy Information Agency, \"Residential Energy Consumption Survey
(RECS)\", accessed 29 August 2021. URL:
<https://www.eia.gov/consumption/residential/>.

\[2\] Energy Information Agency, \"RECS 2015 Microdata Version 4\",
accessed 29 August 2021. URL:
<https://www.eia.gov/consumption/residential/data/2015/csv/recs2015_public_v4.csv>
:::

::: {.cell .code}
``` python
```
:::
