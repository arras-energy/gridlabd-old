[[/Module/Optimize/Simple]] -- Simple optimizers

# Synopsis

GLM:

~~~
object simple 
{
	objective "<object>.<property>";
	variable "<object>.<property>";
	constraint "<object>.<property> <relation> <value>";
	delta <real>;
	epsilon <real>;
	trials <integer>;
	goal [EXTREMUM|MINIMUM|MAXIMUM];
}
~~~

# Description

The `simple` optimizer performs a linear one-dimensional search from the current state to bring the value of `objective` to within `epsilon` of the extremum, by changing `variable` in increments of `delta`.

# Example

The following example find the minimum value of `object_1.output` to within `0.001` of the minimum by changing `object_1.input` in increments of `0.01`.

~~~
class example
{
	double input;
	double output;
	intrinsic sync(TIMESTAMP t0, TIMESTAMP t1)
	{
		output = input*input + input + 1;
		return TS_NEVER;
	};
}
object example
{
	name "object_1";
	input 0;
}
object simple
{
	goal MINIMUM;
	objective "object_1.output";
	variable "object_1.input";
	delta 0.01;
	epsilon 0.001;
}

~~~

# See also

* [[/Module/Optimize]]
