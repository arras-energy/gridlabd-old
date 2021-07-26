# Templates

Templates are special purpose GLM models that perform specific analysis tasks on other models.  For example, a residential template can be loaded after a network model to automatically attach houses to a network.  

The general syntax for using a template is

~~~
% gridlabd MODEL.glm TEMPLATE.glm
~~~

Note that the template is always loaded *after* the model to which it is applied.  We will see why in a moment.  

Public templates can be downloaded from the GridLAB-D template repository on GitHub.  The [[/Subcommand/Template]] is used to download a template from the GitHub repository.  Private template can also be created and install on systems.  The minimum requirements for creating a template are the creation of a GLM file. Many template also have a corresponding Python file, and possible other ancillary files. 

# Examples

The following example illustrates how a template can be used to automatically attach meters and houses to network load objects.  

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

File `houses.glm`:

~~~
module residential
#for LOAD in ${FIND class=load}
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
