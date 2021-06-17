[[/GLM/Macro/Write]] -- Generate a data file from the current model

# Synopsis

~~~
#write FILENAME.EXT CLASSLIST:PROPERTYLIST
~~~

# Description

The `write` macro dumps the current model to a CSV, JSON, or GLM file based on
the filename extension, i.e., where `EXT` is one of `csv`, `json`, or `glm`,
respectively.  Data is only be dumped from the classes specified in `CLASSLIST`
and `PROPERTYLIST` where classes and properties are comma delimited.

Data is dumped with `id`, `class`, and `name` properties.

# Caveat

Note that CSV data creates columns for all properties but uses empty cells for
properties that are not defined to the specified class.  In contrast, JSON and
GLM files will omit any properties that are not defined for the given class.  
This implies that erroneous properties are not detected and will not result in
an error or warning message.

# Example

The following example generates a CSV, JSON, and GLM file containing the

~~~
class test1
{
    randomvar x;
}
class test2
{
    randomvar x;
    randomvar y;
}
object test1:..5
{
    x "type:uniform(0,1)";
}
object test2:..5
{
    x "type:normal(0,1)";
    y "type:uniform(0,1)";
}
#write /tmp/write.csv test1,test2:x,y
~~~

The CSV output looks something like this:
~~~
id,class,x,y
0,test1,"test1:0",0.970795,
1,test1,"test1:1",0.053650,
2,test1,"test1:2",0.845886,
3,test1,"test1:3",0.024933,
4,test1,"test1:4",0.111084,
5,test2,"test2:5",-0.696906,0.222839
6,test2,"test2:6",0.825553,0.745361
7,test2,"test2:7",1.151177,0.632141
8,test2,"test2:8",0.213561,0.701630
9,test2,"test2:9",-0.747831,0.709808
~~~

The JSON output looks something like this:
~~~
[
	{
		"name" : "test1:0",
		"class" : "test1",
		"id" : "0",
		"x" : "0.970795"
	}
	{
		"name" : "test1:1",
		"class" : "test1",
		"id" : "1",
		"x" : "0.053650"
	}
	{
		"name" : "test1:2",
		"class" : "test1",
		"id" : "2",
		"x" : "0.845886"
	}
	{
		"name" : "test1:3",
		"class" : "test1",
		"id" : "3",
		"x" : "0.024933"
	}
	{
		"name" : "test1:4",
		"class" : "test1",
		"id" : "4",
		"x" : "0.111084"
	}
	{
		"name" : "test2:5",
		"class" : "test2",
		"id" : "5",
		"x" : "-0.696906",
		"y" : "0.222839"
	}
	{
		"name" : "test2:6",
		"class" : "test2",
		"id" : "6",
		"x" : "0.825553",
		"y" : "0.745361"
	}
	{
		"name" : "test2:7",
		"class" : "test2",
		"id" : "7",
		"x" : "1.151177",
		"y" : "0.632141"
	}
	{
		"name" : "test2:8",
		"class" : "test2",
		"id" : "8",
		"x" : "0.213561",
		"y" : "0.701630"
	}
	{
		"name" : "test2:9",
		"class" : "test2",
		"id" : "9",
		"x" : "-0.747831",
		"y" : "0.709808"
	}
]
~~~

The GLM output looks something like this:

~~~
// writefile(char *fname='/tmp/write.glm', char *specs='test1,test2')
// modelname /tmp/test.glm
modify test1:0.x "0.970795";
modify test1:1.x "0.053650";
modify test1:2.x "0.845886";
modify test1:3.x "0.024933";
modify test1:4.x "0.111084";
modify test2:5.x "-0.696906";
modify test2:5.y "0.222839";
modify test2:6.x "0.825553";
modify test2:6.y "0.745361";
modify test2:7.x "1.151177";
modify test2:7.y "0.632141";
modify test2:8.x "0.213561";
modify test2:8.y "0.701630";
modify test2:9.x "-0.747831";
modify test2:9.y "0.709808";
~~~

# See also

* [[/GLM/Macro/Read]]
