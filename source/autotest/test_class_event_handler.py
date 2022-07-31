import sys, gridlabd

def on_init(t):
    print(gridlabd.version(),file=sys.stderr)
    return True

def test_init(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"1")
    return False

def test_precommit(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"2")
    return gridlabd.NEVER

def test_presync(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"3")
    return gridlabd.NEVER

def test_sync(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"4")
    return gridlabd.NEVER

def test_postsync(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"5")
    return gridlabd.NEVER

def test_commit(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"6")
    return gridlabd.NEVER

def test_finalize(obj,t):
    gridlabd.set_value(obj,"n",gridlabd.get_value(obj,"n")+"7")
    return gridlabd.NEVER

