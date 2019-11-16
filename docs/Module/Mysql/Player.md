# Advanced queries

The player object supports advanced queries. The advanced query must return two columns, `t` and `x`, ordered by `t` ascending.  The syntax is
~~~~
object player {
    query "select time_value as t, double_value as x order by t asc";
}
~~~~

# Asynchronous queries

The player object support asynchronous initialization using the `maximum_threads` module global.  When `maximum_threads` is non-zero, each player object will initialize asynchronously, meaning that other objects will be allowed to initialize while the player object completes its initialization sequence.  When `maximum_threads` is positive, only that number of simultaneous initializations will be allowed.  When `maximum_threads` is negative there is no limit to the number of simultaneous initializations allowed.

Note: player objects perform queries to the mysql server(s). Most servers have either firm or practical limits on the number queries they can handle simultaneously.  It is strongly recommended that the value of `maximum_threads` be chosen with due consideration to the limits of the server(s) from which the player data is obtained.
