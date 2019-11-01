[[solver_ml.conf]] -- Machine learning powerflow solver configuration

# Synopsis

/usr/local/share/gridlabd/solver_ml.conf:
~~~
maximum_metric <real>
method [none|basic]
busdump <filename>
branchdump <filename>
on_dump <event-handler>
logfile <filename>
loglevel <integer>
import_path <path>
import python:<module>
on_dump [python:]<command>
~~~

# Description

The main Newton-Raphson (NR) powerflow solver can be matched with a secondary machine learning (ML) solver that models and mimics how the NR solver functions.  When the ML solver sees a problem that is "close" to problems it has learned, it proposes a solution to that problem and skips running the NR solver. The [[solver_ml.conf]] configuration controls how the machine learning solver behaves.

The most basic method saves each observed solution for future use.  Any new problem is compared to previous problems using a distance metric, and the solution to the closest problem is used if the distance metric is less than the `maximum_metric`.  If no solution is found, the new solution is saved for future use.

## maximum_metric *real*

The `maximum_metric` parameter control how "close" the new problem must be from one of the existing known solution before it proposes a solution.

## method [none|basic]

The `method` parameter control which machine learning method is applied. The current valid values are `none` to disable the ML solver, and `basic` to enable the simple solution copying method.

## busdump *filename*

The `busdump` parameter sets the bus dump filename when `on_dump` is enabled.

## branchdump *filename*

The `branchdump` parameter sets the branch dump filename when `on_dump` is enabled.

## on_dump *event-handler*

The `on_dump` parameter enables external calls when a problem is presented to the NR solver.  The bus and branch dumps are performed before the `on_dump` command is executed.

## logfile *filename*

The `logfile` parameter sets the name of the file that contains the ML solver log.  By default this is `/usr/local/var/gridlabd/solver_ml.log`.

## loglevel *integer*

The `loglevel` parameter sets the level of log message recorded in the log.  This value can range from 0 to 9, with 0 being the least logging and 9 the most logging.

## import_path *path*

Add `path` to the module import search path.

## import *module*

Load `module`.  If loading a Python module, use the syntax `import python:filename`.

## on_dump *command*

Call the method `command` when a bus/branch dump is performed. If the command is defined in a Python module, use the syntax `on_dump python:command`.
    