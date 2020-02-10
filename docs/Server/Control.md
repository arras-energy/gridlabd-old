[[/Server/Control]] -- Server control operations

# Synopsis
HTTP:
~~~
    GET /control/resume
    GET /control/pause
    GET /control/pause_wait
    GET /control/pauseat=<timestamp>
    GET /control/shutdown
    GET /control/stop
~~~

# Description

The `control` message manages the state of the simulation main loop. The following operations are supported:

* `resume` is used to resume simulation after a pause.

* `pause`, `pause_wait`, and `pause_at` are used to pause the simulation to allow prolonged access to the current state of the model.

* `stop` is used to bring the simulation to a normal end at the current time.

* `shutdown` is used to initiate immediate emergency shutdown of the simulation without going through the normal end procedure.

*Table 1: Mainloop state machine transition matrix*
~~~
|         | Message                                                    |
|         | ---------------------------------------------------------- |
| State   | resume  | pause  | pause_wait | pause_at | shutdown | stop |
| ------- | ------- | ------ | ---------- | -------- | -------- | ---- |
| INIT    |         |        |            |          | exit()   | DONE |
| RUNNING |         | PAUSED | PAUSED     | PAUSED   | exit()   | DONE |
| PAUSED  | RUNNING | PAUSED | PAUSED     | PAUSED   | exit()   | DONE |
| DONE    |         |        |            |          | exit()   | DONE |
| LOCKED  |         |        |            |          | exit()   |      |
~~~

## `resume`

~~~
    GET /control/resume
~~~

Use the `resume` message to continue the simulation when it is paused. In this case, the simulation will run until the global `stoptime` or steady state is reached, whichever comes first.

## `pause`

~~~
    GET /control/pause
~~~

Use the `pause` message to pause the simulation at the current time but not wait for the mainloop state to acknowledge the pause.

## `pauseat`

~~~
    GET /control/pauseat=<timestamp>
~~~

Use the `pauseat` message to pause the simulation at a specified time. Note that if the simulation is already paused and the time given is later than the current time, it will resume the simulation and pause at the specified time.

## `pause_wait`

~~~
    GET /control/pause_wait
~~~

Use the `pause_wait` message to pause the simulation at the current time and wait for the mainloop state to acknowledge the pause.

## `shutdown`

~~~
    GET /control/shutdown
~~~

Use the `shutdown` message to immediately shut down the server.

## `stop`

~~~
    GET /control/stop
~~~

Use the `stop` message to end the simulation normally.

# See also

* [[/Server/REST API]]
