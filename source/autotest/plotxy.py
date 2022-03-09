import sys, pandas, json;
if len(sys.argv) >= 3:
    a=pandas.read_csv(sys.argv[1],header=None);
    if len(sys.argv) == 4:
        b = eval("a.plot(%s)"%sys.argv[3]);
    else:
        b = a.plot();
    b.get_figure().savefig(sys.argv[2])
else:
    raise Exception("Syntax: python3 plotxy.py <csv> <png> [<options>]")