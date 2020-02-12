[[/Module/Powerflow/Regulator_configuration]] -- Class regulator_configuration

# Synopsis

GLM:

~~~
  object regulator_configuration {
    connect_type "{CLOSED_DELTA,OPEN_DELTA_CABA,OPEN_DELTA_BCAC,OPEN_DELTA_ABBC,WYE_WYE,UNKNOWN}";
    band_center "<decimal> V";
    band_width "<decimal> V";
    time_delay "<decimal> s";
    dwell_time "<decimal> s";
    raise_taps "<integer>";
    lower_taps "<integer>";
    current_transducer_ratio "<decimal> pu";
    power_transducer_ratio "<decimal> pu";
    compensator_r_setting_A "<decimal> V";
    compensator_r_setting_B "<decimal> V";
    compensator_r_setting_C "<decimal> V";
    compensator_x_setting_A "<decimal> V";
    compensator_x_setting_B "<decimal> V";
    compensator_x_setting_C "<decimal> V";
    CT_phase "[CBA]";
    PT_phase "[CBA]";
    regulation "<decimal>";
    control_level "{BANK,INDIVIDUAL}";
    Control "{REMOTE_NODE,LINE_DROP_COMP,OUTPUT_VOLTAGE,MANUAL}";
    reverse_flow_control "{LOCK_CURRENT_POSITION,LOCK_NEUTRAL,LOCK_NONE}";
    Type "{B,A}";
    tap_pos_A "<integer>";
    tap_pos_B "<integer>";
    tap_pos_C "<integer>";
  }
~~~

# Description

TODO

## Properties

### `connect_type`

~~~
  enumeration {CLOSED_DELTA, OPEN_DELTA_CABA, OPEN_DELTA_BCAC, OPEN_DELTA_ABBC, WYE_WYE, UNKNOWN} connect_type;
~~~

Designation of connection style

### `band_center`

~~~
  double band_center[V];
~~~

Band center setting of regulator control

### `band_width`

~~~
  double band_width[V];
~~~

Band width setting of regulator control

### `time_delay`

~~~
  double time_delay[s];
~~~

Mechanical time delay between tap changes

### `dwell_time`

~~~
  double dwell_time[s];
~~~

Time delay before a control action of regulator control

### `raise_taps`

~~~
  int16 raise_taps;
~~~

Number of regulator raise taps, or the maximum raise voltage tap position

### `lower_taps`

~~~
  int16 lower_taps;
~~~

Number of regulator lower taps, or the minimum lower voltage tap position

### `current_transducer_ratio`

~~~
  double current_transducer_ratio[pu];
~~~

Primary rating of current transformer

### `power_transducer_ratio`

~~~
  double power_transducer_ratio[pu];
~~~

Potential transformer rating

### `compensator_r_setting_A`

~~~
  double compensator_r_setting_A[V];
~~~

Line Drop Compensation R setting of regulator control (in volts) on Phase A

### `compensator_r_setting_B`

~~~
  double compensator_r_setting_B[V];
~~~

Line Drop Compensation R setting of regulator control (in volts) on Phase B

### `compensator_r_setting_C`

~~~
  double compensator_r_setting_C[V];
~~~

Line Drop Compensation R setting of regulator control (in volts) on Phase C

### `compensator_x_setting_A`

~~~
  double compensator_x_setting_A[V];
~~~

Line Drop Compensation X setting of regulator control (in volts) on Phase A

### `compensator_x_setting_B`

~~~
  double compensator_x_setting_B[V];
~~~

Line Drop Compensation X setting of regulator control (in volts) on Phase B

### `compensator_x_setting_C`

~~~
  double compensator_x_setting_C[V];
~~~

Line Drop Compensation X setting of regulator control (in volts) on Phase C

### `CT_phase`

~~~
  set {C, B, A} CT_phase;
~~~

Phase(s) monitored by CT

### `PT_phase`

~~~
  set {C, B, A} PT_phase;
~~~

Phase(s) monitored by PT

### `regulation`

~~~
  double regulation;
~~~

Regulation of voltage regulator in %

### `control_level`

~~~
  enumeration {BANK, INDIVIDUAL} control_level;
~~~

Designates whether control is on per-phase or banked level

### `Control`

~~~
  enumeration {REMOTE_NODE, LINE_DROP_COMP, OUTPUT_VOLTAGE, MANUAL} Control;
~~~

Type of control used for regulating voltage

### `reverse_flow_control`

~~~
  enumeration {LOCK_CURRENT_POSITION, LOCK_NEUTRAL, LOCK_NONE} reverse_flow_control;
~~~

Type of control used when power is flowing in reverse through the regulator

### `Type`

~~~
  enumeration {B, A} Type;
~~~

Defines regulator type

### `tap_pos_A`

~~~
  int16 tap_pos_A;
~~~

Initial tap position of phase A

### `tap_pos_B`

~~~
  int16 tap_pos_B;
~~~

Initial tap position of phase B

### `tap_pos_C`

~~~
  int16 tap_pos_C;
~~~

Initial tap position of phase C

# Example

~~~
  object regulator_configuration {
    connect_type "0";
    band_center "0.0";
    band_width "0.0";
    time_delay "0.0";
    dwell_time "0.0";
    raise_taps "0";
    lower_taps "0";
    current_transducer_ratio "0.0";
    power_transducer_ratio "0.0";
    compensator_r_setting_A "0.0";
    compensator_r_setting_B "0.0";
    compensator_r_setting_C "0.0";
    compensator_x_setting_A "0.0";
    compensator_x_setting_B "0.0";
    compensator_x_setting_C "0.0";
    CT_phase "0";
    PT_phase "0";
    regulation "0.0";
    control_level "0";
    Control "0";
    reverse_flow_control "0";
    Type "0";
    tap_pos_A "0";
    tap_pos_B "0";
    tap_pos_C "0";
  }
~~~

# See also

* [[/Module/Powerflow]]

