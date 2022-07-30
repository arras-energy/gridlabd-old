
def test_init(obj,t):

    x = gridlabd.get_property(obj,"x")
    gridlabd.set_double(x,gridlabd.get_double(x)+1)

    z = gridlabd.get_property(obj,"z")
    gridlabd.set_complex(z,gridlabd.get_complex(z)+(2+3j))

    i = gridlabd.get_property(obj,"i")
    gridlabd.set_int64(i,gridlabd.get_int64(i)+4)

    j = gridlabd.get_property(obj,"j")
    gridlabd.set_int32(j,gridlabd.get_int32(j)+5)

    k = gridlabd.get_property(obj,"k")
    gridlabd.set_int16(k,gridlabd.get_int16(k)+6)

    b = gridlabd.get_property(obj,"b")
    gridlabd.set_bool(b,not gridlabd.get_bool(b))

    return 0
