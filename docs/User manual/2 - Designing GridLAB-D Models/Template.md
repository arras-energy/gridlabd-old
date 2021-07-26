# Templates

Templates are special purpose GLM models that perform specific analysis tasks on other models.  For example, a residential template can be loaded after a network model to automatically attach houses to a network.  

The general syntax for using a template is

~~~
% gridlabd MODEL.glm TEMPLATE.glm
~~~

Note that the template is always loaded *after* the model to which it is applied.  We will see why in a moment.  

Public templates can be downloaded from the GridLAB-D template repository on GitHub.  The [[/Subcommand/Template]] is used to download a template from the GitHub repository.  Private template can also be created and install on systems.  The minimum requirements for creating a template are the creation of a GLM file. Many template also have a corresponding Python file, and possible other ancillary files. 

# Examples

For the following examples, we will use the following network model, which includes 5 single-phase load busses.

File `network.glm`:

~~~
module powerflow;
#for PHASE in A B C
object triplex_load:..5
{
  nominal_voltage 120V;
  phases ${PHASE}S;
}
#done
~~~

## Weather template

The following example illustrates how to create a template for loading weather data.

~~~
#weather get "CA-SanFrancisco_Intl_Ap.tmy3"
#input "CA-SanFrancisco_Intl_Ap.tmy3"
~~~

## Clock template

The following example illustrates how to create a template for setting the simulation clock.

File `clock_template.glm`:

~~~
clock 
{
  timezone "PST+8PDT";
  starttime "2021-01-01 00:00:00 PST";
  stoptime "2022-01-01 00:00:00 PST";
}
~~~

## Object template

The following example illustrates how a template can be used to automatically attach single-phase meters and houses to load objects in the network model.  

File `house_template.glm`:

~~~
module residential
#for LOAD in ${FIND class=triplex_load}
object triplex_meter
{
  parent ${LOAD};
  nominal_voltage @${LOAD};
  phases @${LOAD};
  object house
  {
  };
}
#done
~~~

The `#for LOAD in ${FIND class=triplex_load}` searches the model for all the `triplex_meter` object and loops through them assigning the objects' names sequentially to the `LOAD` variable. The `@` prefix in the `triplex_meter` object `nominal_voltage` and `phases` properties copy the same properties of the `triplex_load` object.

