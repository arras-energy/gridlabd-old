[[/Subcommand/Plot]] -- GridLAB-D plot subcommand

# Synopsis

Shell:

~~~
$ gridlabd plot OPTIONS
~~~

GLM:

~~~
#plot OPTIONS
~~~

or

~~~
#on_exit CODE gridlabd plot OPTIONS
~~~

# Description

The 'gridlabd plot' command uses pandas and matplotlib to create PNG files from
    CSV files.

## Options

The following options are available:

### `-d|--debug`

Enable debug output (if any)

### `-e|--exception`

Enable raising of exception instead of printing exception message and exiting with error code 3.

### `--figure:OPTIONS[=VALUE]` 

Specify matplotlib pyplot figure option. See [Matplotlib *figure*](https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.figure.html) for details.

- *figsize* (tuple): Specify figure width and height in inches. Default is `6.4,4.8`.
- *dpi* (float): Specify plot resolution in dots per inch. Default is `100.0`.
- *facecolor* (str): Specify the background color. See [Matplotlib colors](https://matplotlib.org/stable/gallery/color/named_colors.html) for details. Default is `white`.
- *edgecolor* (str): Specify the border color. See [Matplotlib colors](https://matplotlib.org/stable/gallery/color/named_colors.html) for details. Default is `white`.
- *frameon* (bool): Enable visibility of the figure frame. Default is `True`.
- *tight_layout* (bool): Enable adjustment of the padding to fit plot elements. Default is `False`.

### `-h|--help|help`

Display this help information

### `-i|--input=PATH`

Specify the input CSV pathname

### `-o|--output=PATH`

Specify the output PNG pathname (default is input with ".png")

### `-q|--quiet`

Disable error output (if any)

### `-s|--show`

Show the output

### `--plot:OPTIONS[=VALUE]`   

The following [Pandas DataFrame *plot*](https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.DataFrame.plot.html) options are available. In addition, many [Matplotlib *plot*](https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.plot.html) options may be used.

- *x* (str or int): Specify the x data. Default is `None`.
- *y* (str, int, of list of str or int): Specify the y data. Default is `None`
- *kind* (str): specify the kind of plot, i.e., "line", "bar", "barh", "hist", "box", "kde", "density", "area", "pie", "scatter", "hexbin". Default is "line".
- *subplots* (bool): Plot y data on separate subplots. Default is `False`.
- *sharex* (bool): Share x axis when using `subplots`.
- *sharey* (bool): Share y axis when using `subplots`.
- *layout* (tuple): Specify `(rows,columns)` when using `subplots`.
- *figsize* (tuple): Specify `(width,height)` of figure in inches.
- *use_index* (bool): Specify the use of the row index as ticks. Default is `True`.
- *title* (str or list of str): Specify the (sub)plot title(s). Defualt is `None`.
- *grid* (bool): Enable plotting of the grid. Default is `False`.
- *legend* (bool): Enable plotting of the legend. Default is `False`.
- *style* (str): Specify the list style. Default is `-`.
- *logx* (bool): Enable plotting *x* axis on a logarithmic scale. Default is `False`.
- *logy* (bool): Enable plotting *y* axis on a logarithmic scale. Default is `False`.
- *loglog* (bool): Enable plotting both axes on a logarithmic scale. Default is `False`.
- *xticks* (sequence): Specify *x*-tick values. Default is automatic.
- *yticks* (sequence): Specify *y*-tick values. Default is automatic.
- *xlim* (tuple): Specify *x* limits. Default is `None`.
- *ylim* (tuple): Specify *y* limits. Default is `None`.
- *xlabel* (str): Specify the *x* axis label. Default is *x* field name.
- *ylabel* (str): Specify the *y* axis label. Default is *y* field name.
- *rot* (int): Specify the rotation for tick labels. Default is 0.
- *fontsize* (int): Specify the tick label font size. Default is 10.
- *position* (float): Specify the relative alignment of bars, i.e., 0 for left/bottom to 1 for top/right. Default is 0.5 (center).
- *colormap* (str): Specify the color map to use when *colorbar* is enabled.
- *colorbar* (bool): Enable the colorbar (only for *scatter* and *hexbin* plots).
- *table* (bool): Enable placement of a data table below the plot. Default is `False`.
- *xerr* (str): Field to use for *x* error bars. Default is `None`.
- *yerr* (str): Field to use for *y* error bars. Default is `None`.
- *stacked* (bool): Enable stacked *line*, *area* and *bar* plots. Default is `False` for *line* and *bar* plots, and `True` for *area* plots.
- *sort_columns* (bool): Enable sorting of column names when ordering the plot. Default is `False`.
- *secondary_y* (bool): Enable plotting of *y* values on secondary axis if multiple *y* fields are specified. Default is `False`.
- *mark_right* (bool): Enable marking with `(right)` secondary *y*-axis fields. Default is `False`.
- *include_bool* (bool): Enable inclusion of Boolean values in plot. Default is `False`.

### `-v|--verbose`

Enable verbose output (if any)

### `-w|--warning`

Disable warning output (if any)

# Example

The following example illustrates how to generate a plot of two fields generated
by a recorder.

~~~
module tape
{
    csv_header_type NAME;
}
clock
{
    timezone "PST+8PDT";
    starttime "2020-01-01 00:00:00 PST";
    stoptime "2020-01-02 00:00:00 PST";
}
class test {
    randomvar x[MW];
    randomvar y[MWh];
}
object test
{
    x "type:normal(100,10); min:0; max:200; refresh:1h";    
    y "type:normal(100,10); min:0; max:200; refresh:1h";
    object recorder
    {
        property "x,y";
        interval -1;
        file ${modelname/glm/csv};
    };
}
#on_exit 0 gridlabd plot -i=${modelname/glm/csv} --plot:x=timestamp --plot:y=x,y --plot:rot=90 --plot:grid --plot:legend --figure:tight_layout=True
~~~
