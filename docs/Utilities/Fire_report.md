[[/Tools/Fire_report]] - Fire Incident Report Tool

# Synopsis

Shell:

~~~
$ gridlabd fire_report -s|--state=STATE -y|--year=YEARS -u|--utility=UTILITY [-o|--output=CSVFILE] [--refresh] [--help]
~~~

GLM:

~~~
#fire_report -s|--state=STATE -y|--year=YEARS -u|--utility=UTILITY [-o|--output=CSVFILE] [--refresh] [--help]
~~~

Python:

~~~
>>> import fire_report
>>> fire_report.main(state=STATE,year=YEARS,utility=UTILITY,output=CSVFILE,refresh=BOOLEAN,help=BOOLEAN)
~~~

# Description

The `fire_report` tool retrieves annual fire report data delivered to state
regulators.  The current tool only supports California Public Utility Commission
reporting (`state=CA`) for Pacific Gas and Electric (`utility=PGE`), Southern
California Edison (`utility=SCE`), and San Diego Gas and Electric (`utility=SDGE`).

The data is available for reported years, starting in 2014. The `YEARS` parameters may
be specified as a sequence of comma-delimited years or range of years, e.g.,
`2014,2015,2018,2019,2020,2021` or `2014,2015,2018-2021`.

# Example

The following downloads the complete fire report record for PG&E from 2014 through 2021:

~~~
$ gridlabd fire_report -y=2014-2021 -u=PGE -s=CA
datetime,latitude,longitude,material_at_origin,land_use_at_origin,size,suppressed_by,suppressing_agency,facility_identification,other_companies,voltage_volts_,equipment_involved_with_ignition,type,was_there_an_outage,date_1,time_1,suspected_initiating_event,equipment_facility_failure,contact_from_object,facility_contacted,contributing_factor,unnamed_23,voltage,material_at_origin_comments
2014-06-10 15:14.00,40.20599,-122.25333,Vegetation,Rural,< 0.25 Acres,Fire Agency,Red Bluff FD,101545144,,12000,Conductor,Overhead,Yes,2014-06-10 15:14:00,2014-06-10 15:14:00,Contact From Object,N.A.,Other,Electric Facility,Human Error,Conveyor belt contact,,
2014-06-12 06:30.00,38.04374999,-122.74623999999999,Vegetation,Rural,10 - 99 Acres,Fire Agency,Cal Fire et al,102230371,AT&T,12000,Conductor,Overhead,Yes,2014-06-12 06:30:00,2014-06-12 06:30:00,Equipment/ Facility Failure,Crossarm,N.A.,N.A.,Unknown,,,
2014-06-13 09:19.00,36.93347999,-121.388549,Vegetation,Rural,< 0.25 Acres,Fire Agency,Hollister FD,101703071,AT&T,21000,Conductor,Overhead,Yes,2014-06-13 09:19:00,2014-06-13 09:19:00,Contact From Object,N.A.,Vehicle,Pole,Human Error,,,
...
2021-12-24 03:20:00,37.148326700000005,-121.99329170000001,Vegetation,Conifer Forest,<3 meters of linear travel,Utility - PG&E,,100554483,N,,Conductor - Primary,Overhead,Y,2021-12-24,03:17:00,Contact from Object,,Vegetation,Conductor - Primary,,,12000,
2021-10-14 07:00.00,40.070370000000004,-122.2334,Other,Unknown,<0.25,Unknown,,,N,,Switch,Overhead,N,,,Equipment - PG&E,Unknown,,,,,60000,Pole
2021-06-28 07:18:00,38.9999673839,-122.75278890690001,Vegetation,Unknown,<0.25,Cal Fire,,102156370,Y,,Conductor - Open Wire Secondary,Overhead,Y,2021-06-28,07:18:00,Contact from Object,,Vegetation,Conductor - Open Wire Secondary,,,0-750,
~~~

# See also

* [California CPUC Wildire and Safety Reports](https://www.cpuc.ca.gov/industries-and-topics/wildfires)
