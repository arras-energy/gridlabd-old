# Voltdump 

The voltdump object produces a dump of the voltages at the nodes. This can be used to determine the cause of convergence problems.

# Currdump
The currdump object produces a dump of the currents at the nodes. This can be used to determine the cause of convergence problems.

A use case example of each below: 

~~~
object voltdump {
	filemode "a";
	filename "output/volt_dump.csv";
	interval 60;
	maxcount 0;
}
~~~

~~~
object currdump {
	filemode "a";
	filename "output/curr_dump.csv";
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

