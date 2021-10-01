[[/Subcommand/Job]] - Job control subcommand

# Synposis

Shell:

~~~
host% gridlabd job [-v|--verbose] [-q|--quiet] [-d|--debug] [-j|--jobfile JOBFILE] [-w|--workdir FOLDER] [-c|--configfile CONFIG] [-m|--modifyfile MODIFY] [-T|--threadcount NTHREADS] FILE1 ...
~~~

GLM:

~~~
#job [-v|--verbose] [-q|--quiet] [-d|--debug] [-j|--jobfile JOBFILE] [-w|--workdir FOLDER] [-c|--configfile CONFIG] [-m|--modifyfile MODIFY] [-T|--threadcount NTHREADS] FILE1 ...
~~~

# Description

The `job` subcommand run multiple parametric gridlabd simulation in a working folder using a job control file.  Each simulation is run using configuration file created from the parameters that is loaded before the model files are loaded.

The default job control file name is `gridlabd-job.csv`. Each column specifies a parameter to set, and each row specifies a job.  

The default configuration file name is `gridlabd-config.glm`.

## Options

### `-c|--configfile CONFIG`

Specifies the name of the configuration file name to use instead of `gridlabd-config.glm`. The configuration file is created from the job entry in the job control file and loaded before the main GLM file is loaded.

### `-d|--debug`

Enables debugging output.

### `-j|--jobfile JOBFILE`

Specifies the name of the job control file to use instead of `gridlabd-job.csv`.

### `-m|--modifyfile MODIFY`

Specifies the name of the modify file to use instead of `gridlabd-modify.glm`. The modification file is loaded after the main GLM file is loaded.

### `-q|--quiet`

Disables all but error output.

### `-T|--threadpool NTHREADS`

Enables parallel processing of jobs using a threadpool using the specified number of threads.

### `-v|--verbose`

Enable additional output (useful to diagnose problem).

### `-w|--workdir FOLDER`

Specifies the working folder in which to run the job.

# Example

`gridlabd-job.csv`:
~~~
TEST1,TEST2,TEST3
A,B,C
D,E,F
G,H,I
J,K,L
~~~

`example.glm`:
~~~
#ifndef TEST1
#job ${modelname}
#else
#set suppress_repeat_messages=FALSE
#print TEST1=${TEST1}
#print TEST2=${TEST2}
#print TEST3=${TEST3}
#endif
~~~

To run the job from the GLM file, use the following command
~~~
host% gridlabd example.glm
Job 0: return code  0
Output:
test_job.glm(5): TEST1=A
test_job.glm(6): TEST2=B
test_job.glm(7): TEST3=C

No errors
Job 1: return code  0
Output:
test_job.glm(5): TEST1=D
test_job.glm(6): TEST2=E
test_job.glm(7): TEST3=F

No errors
Job 2: return code  0
Output:
test_job.glm(5): TEST1=G
test_job.glm(6): TEST2=H
test_job.glm(7): TEST3=I

No errors
Job 3: return code  0
Output:
test_job.glm(5): TEST1=J
test_job.glm(6): TEST2=K
test_job.glm(7): TEST3=L

No errors
~~~

To run the job from the command shell, use the following command:
~~~
host gridlabd job example.glm
Job 0: return code  0
Output:
test_job.glm(5): TEST1=A
test_job.glm(6): TEST2=B
test_job.glm(7): TEST3=C

No errors
Job 1: return code  0
Output:
test_job.glm(5): TEST1=D
test_job.glm(6): TEST2=E
test_job.glm(7): TEST3=F

No errors
Job 2: return code  0
Output:
test_job.glm(5): TEST1=G
test_job.glm(6): TEST2=H
test_job.glm(7): TEST3=I

No errors
Job 3: return code  0
Output:
test_job.glm(5): TEST1=J
test_job.glm(6): TEST2=K
test_job.glm(7): TEST3=L

No errors
~~~
