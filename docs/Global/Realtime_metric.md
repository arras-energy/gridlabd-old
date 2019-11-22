[[Realtime_metric]] -- Metric to measure the performance of the realtime simulation

# Synopsis
GLM:
~~~
#print ${realtime_metric}
~~~

# Description

The `realtime_metric` global variable has been added to allow the modeler to determine how much time the simulation is idle while it waits for the wall clock time to catch up.  Values near 0 indicates poor model performance (i.e., less idle time). Values near 1 indicate good model performance (i.e., more idle time).
