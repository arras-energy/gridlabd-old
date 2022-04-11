
def test_class_precommit(obj,t):
    object_event = gridlabd.get_value(obj,"object_event")
    if object_event != "FALSE":
        raise Exception(f"<object {obj}>: unexpected class_event")
    return t+300

def test_class_commit(obj,t):
    object_event = gridlabd.get_value(obj,"object_event")
    if object_event != "FALSE":
        raise Exception(f"<object {obj}>:unexpected class_event")
    return True

def test_object_precommit(obj,t):
    object_event = gridlabd.get_value(obj,"object_event")
    if object_event != "TRUE":
        raise Exception(f"<object <{obj}>:unexpected object_event")
    return t+300

def test_object_commit(obj,t):
    object_event = gridlabd.get_value(obj,"object_event")
    if object_event != "TRUE":
        raise Exception(f"<object {obj}>:unexpected object_event")
    return True