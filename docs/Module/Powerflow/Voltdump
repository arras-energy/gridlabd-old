# Voltdump 

The voltdump object produces a dump of the voltages at the nodes. This can be used to determine the cause of convergence problems.

~~~
object voltdump {
	filemode "a";
	filename "output/volt_dump.csv";
	interval 60;
	maxcount 0;
}
~~~

New features added are: 
~~~
filemode
~~~
- `"w"` writes file 
- `"a"` appends to the end of file as voltdump with a new timestamp is generated

~~~
interval 
~~~
- allows to set an interval at which the volt/curr dump should be appended with 
- Note, use the filemode `"a"` to facilitate interval dump

