[[/Module/Industrial/NAICS_Data_File]] -- NAICS Data File

# Synopsis

GLM:

~~~
module industrial
{
    naics_data_file "<file-name>";
}
~~~

# Description

The NAICS data file contains the load composition for various industrial loads.

The current data support the following facilities:

| NAICS Code | Industrial Load Type       | NERC Code | Motor A | Motor B | Motor C | Motor D | Power Electronics | Constant Impedance | Constant Current |
| ---------- | -------------------------- | --------- | ------- | ------- | ------- | ------- | ----------------- | ------------------ | ---------------- |
| 0          | Industrial - Other         | IND_OTH   | 0.1     | 0.3     | 0.3     | 0       | 0.2               | 0.05               | 0.05             |
| 211        | Shale Gas Extraction Plant | IND_SHG   | 0       | 0.2     | 0.4     | 0       | 0.4               | 0                  | 0                |
| 212        | Mining                     | IND_MIN   | 0.25    | 0.25    | 0.3     | 0       | 0.2               | 0                  | 0                |
| 213        | Oil Pumping                | IND_OIL   | 0.3     | 0       | 0.4     | 0       | 0.3               | 0                  | 0                |
| 2211       | Power Plant Auxiliaries    | PPA_AUX   | 0       | 0.4     | 0.3     | 0       | 0.2               | 0.05               | 0.05             |
| 2213       | Irrigation and pumping     | AGR_IRR   | 0       | 0       | 1.0     | 0       | 0                 | 0                  | 0                |
| 311        | Food processing            | AGR_PRO   | 0.6     | 0       | 0.25    | 0       | 0.05              | 0.05               | 0.05             |
| 3221       | Paper Mill Kraft Process   | IND_PMK   | 0.1     | 0.2     | 0.3     | 0       | 0.3               | 0.05               | 0.05             |
| 3222       | Paper Mill with Refiners   | IND_PMT   | 0.05    | 0.6     | 0.15    | 0       | 0.15              | 0.02               | 0.03             |
| 325        | Petro-Chemical Plant       | IND_PCH   | 0.1     | 0.4     | 0.3     | 0       | 0.15              | 0.02               | 0.03             |
| 3311       | Steel Mill                 | IND_SML   | 0.15    | 0.35    | 0.25    | 0       | 0.15              | 0.05               | 0.05             |
| 3314       | Aluminum Smelter           | IND_ASM   | 0.05    | 0       | 0.05    | 0       | 0.05              | 0.85               | 0                |
| 333        | Liquified Natural Gas      | IND_LNG   | 0       | 0.3     | 0.2     | 0       | 0.5               | 0                  | 0                |
| 334        | Semiconductor              | IND_SCD   | 0       | 0.25    | 0.3     | 0       | 0.4               | 0                  | 0.05             |
| 336        | Car Manufacturing          | IND_CAR   | 0.15    | 0       | 0.3     | 0       | 0.3               | 0.1                | 0.15             |
| 423        | Lumber Mill                | IND_LMB   | 0.4     | 0.2     | 0.3     | 0       | 0                 | 0.05               | 0.05             |
| 482        | Transportation - Rail      | IND_RAIL  | 0       | 0       | 0.05    | 0       | 0.95              | 0                  | 0                |
| 518        | Server Farm                | IND_SRF   | 0       | 0       | 0.1     | 0       | 0.9               | 0                  | 0                |

# See also

* [[/Module/Industrial]]
* [[/Module/Industrial/Industrial]]
* [NAICS Codes](https://www.naics.com/code-search)
