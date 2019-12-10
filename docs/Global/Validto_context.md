[[Global/Validto_context]] -- Control how `valid_to` time is used

# Synopsis
~~~
  #set validto_context=SYNC|PRECOMMIT|COMMIT
~~~

# Description

The global variable [[validto_context]] may be used to control whether `precommit` and `commit` events are suppressed before the `valid_to` time is reached.

## SYNC

The `SYNC` option is the default and cannot be deactivated. It does not need to be included.

## PRECOMMIT

The `PRECOMMIT` option may be used to suppress calls to `precommit` prior to the `valid_to` time.

## COMMIT 

The `COMMIT` option may be used to suppress calls to `commit` prior to the `valid_to` time.

# Example
~~~
  class test {
      randomvar x;
      intrinsic precommit(TIMESTAMP t0, TIMESTAMP t1) {
          my->valid_to = TIMESTAMP(t0/300)*300;
          return my->valid_to;
      };
      intrinsic commit(TIMESTAMP t0, TIMESTAMP t1) {
          return my->valid_to;
      };
  #set validto_context=PRECOMMIT
  import tape;
  object test {
      x "type:normal(0,1); min:-10; max:10; refresh:1min";
      object recorder {
          property x;
          file text.csv;
          interval 1min;
      };
  }
~~~

# Caveat

This feature may have undesired effects on classes that were designed prior to its introduction.  
