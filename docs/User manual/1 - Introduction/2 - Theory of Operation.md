During command line processing, GridLAB-D loads one or more GLM files listed and uses the directives in those files to set global variables, set global variables, run scripts, load modules, define classes, create objects, and link to external applications. Once all the objects are created and initialized, the main exec loop is started and the global clock begins to advance.

# Clock operation

GridLAB-D's main processing loop advances the global clock beginning with the `starttime` by varying time steps depending on the pending state changes of the object's defined in the model until no object reports that is has a pending state change.

Pending state state changes are called sync events. There are two types of sync event:

**Hard events**: these are events that must be handled in order for the simulation to continue. If there are no hard events pending, the simulation will stop.

**Soft events**: these are events that are only handled if they occur before a hard event. If there are only soft events pending, the simulation will stop.

The `stoptime` global variable is a special hard event if it is not set to `NEVER`. If the stoptime is set to `NEVER`, the simulation will only stop when there are no pending hard events. Otherwise the simulation will stop when the `stoptime` is reached.

# Object ranks

Objects are assigned ranks when they are created. The rank strategy varies depending on the solver being used and the parent-child relationships between the objects. Normally, the modeler only controls the parent-child relationship by either nesting objects when they are defined, or using the name property. Modelers cannot strictly control the rank of objects because modules will generally apply their own ranking rules to ensure a stable solution.

# Synchronization procedure

Objects are instances of classes. Each class has a synchronization characteristic the determines when it receives events handling signals from the main exec loop. The event signals are sent to the system, modules, classes, and objects in the following order:

- **create**: called once per object

- **init**: called until object confirms successful initiatialization

- *do until clock stops*:

    - **precommit**: called once per timestep before sync; used to setup for new timestep

    - *do until valid timestep is found*:

        - **presync**: called once per pass from the top rank down; used to prepare objects for bottom-up pass
        
        - **sync**: called once per pass from the bottom rank up; used to perform main calculations in objects
        
        - **postsync**: called once per pass from the top rank down; used to complete calculations

    - **commit**: called once per timestep after sync; used to lock-in states

- **finalize**: called once per simulation after clock stops 

- **term**: called to terminate simulation

# Parallelization and multithreading

Any iterative process that does not have interdependencies can be run in parallel using multiple thread. When the threadcount is not 1, GridLAB-D will create threads to handle a wide variety of loops that can be operated in parallel.

# Real-time operation

When running in real-time mode, GridLAB-D limits how fast the clock will advance based on the value of the realtime global variable. Real-time models are enables by setting the global `run_realtime` to a non-zero value.  The value determines how much faster than real-time the simulation runs, e.g., 1.0 is real-time, 2.0 is twice real-time, etc.

# Internal synchronization events

There are a number of internal synchronization events that take place while the main exec loop is running.

**Before precommit**:

- *Link*: links are updated first

- *Instance*: slave instances are resumed 
    
- *Random*: random variables are updated 
    
- *Schedule*: schedules are update 
    
- *Loadshape*: loadshapes are updated 
    
- *Transforms*: schedule transforms are updated 
    
- *Enduse*: enduses are updated
    
- *Heartbeat*: heartbeat signals are sent

**Before commit**:

- *Transforms*: non-schedule transforms are updated

- *Instance*: slave instances are waited on

**After commit**:

    - *Scripts*: script sync events are run

# Debugging

Debugging the main exec loop can be very challenging. The `--debugger` command line option can be used to facilitate the process. In addition, copious use of the tape module's `recorder` object, particularly with `interval` set to 0 or -1 can also be very helpful.

The `--verbose` and `--debug` options can be used to output significant additional information about processing.  The can often be used to isolate how objects are affecting the main exec loop.  Which part of the core generates messages can be controlled using the `output_message_context` global variable. Sometimes repeated messages are output and by default these are suppressed. This can be disabled using the `suppress_repeat_messages` global. Finally module `message_flags` can be used to control which modules output verbose and debug information, and the object `flags` can be set to `VERBOSE` and/or `DEBUG` to enable additional information from specific objects.

When the system crashes, the operation system debugger can be enabled using the `--lldb` or `--gdb` command line option on  MacOS and Linux, respectively.  
