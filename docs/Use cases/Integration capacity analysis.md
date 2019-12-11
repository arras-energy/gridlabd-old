[[/Use_cases/Integration_capacity_analysis]] -- Integration capacity analysis

# Introduction

In California, Investor Owned Utilities (IOUs) are required to complete detailed ICA system wide to define the maximum node level hosting capacity to remain within power system criteria. System ICA values are used to expedite interconnection permitting for DER under the Rule 21 interconnection process (Interconnection Use Case). Results must be updated monthly and data must be mapped for review with hourly capacity limitation data available for download by the user. ICA with DER growth scenarios are also required for all annual IOU distribution system planning processes (Planning Use Case). The CPUC provided guidance to IOUs in late 2017 to complete ICA system wide in response to completed IOU demonstration projects and the DER WG final report. 

The current ICA specifications are as follows: 
 - Iterative methodology required
 - ICA results will be updated monthly for changed circuits
 - Employ 576 hourly profiles (monthly min and max day)
 - Localized load shapes to be developed based on AMI
 - ICA results shall be provided in online maps and downloadable datasets for: 
   - Uniform Generation
   - Uniform Load
   - Fixed PV 
   - Operational Flexibility Scenarios: ICA calculated with and without reverse flow constraint at SCADA devices 
 - Mapping displays will include:  Circuit ID; Circuit Load Profile; Section ID; Voltage (kV); Substation ID; Substation Load Profile; System; Customer class proportions on circuit; Existing generation (MW); Queued generation (MW); Total generation (MW); Hosting capacity for uniform generation (MW); Hosting capacity for uniform load(MW); and Hosting capacity for generic PV system (MW). 
 - Publish criteria associated with limiting ICA value
 - Voltage regulating devices are to be allowed to float where software allows 
 - Pre-existing conditions shall limit ICA when adding DER degrades the pre-existing condition, but ICA shall not be limited when adding DER improves pre-existing condition
 - ICA values should be technology agnostic with respect to DER portfolio
 - Node reduction, minimizing the number of nodes analyzed, and limitation category reduction for strong feeders are acceptable methods for computational efficiency

The CPUC is solidifying the role of ICA values in expediting interconnection permitting under Rule 21, further highlighting the need for IOUs to maintain up to date real time ICA values system wide. The CPUC also adopted the ICA use case for distribution planning, further highlighting the need for improved computational efficiency. The commission directs the IOUs to implement DER growth scenarios with ICA to complete annual planning exercises. This requires the evaluation of multiple forecasted scenarios including varying load and network configurations to determine where capacity may become limited and infrastructure may need to be upgraded. 

# Methodology

The ICA quantifies the potential to interconnect DER without violating thermal, power quality/voltage, protection, and safety/reliability criteria throughout the distribution system. The DER interconnection use case is required to be completed using iterative dynamic power flow modeling system-wide rather than streamlined heuristic calculations. ICA must be conducted assuming "technology agnostic" generation with multiple scenarios considering operational flexibility limitations.

The requirement for near real time ICA accuracy and for multiple scenario evaluations across all circuits is extremely computationally intensive. There are currently no independent third party validation tools approved to assess IOU developed processes. GridLAB-D and HIPAS offer significant benefit in efficiently conducting iterative ICA system wide due to these large IOUs provided public access to system-wide ICA values at the end of 2018. Utilities have successfully implemented technology agnostic ICA with 576 hourly profiles with simulation tools which accounts for operation of voltage regulators and pre-existing conditions. 

Utilities have noted particular challenges related to the following: 
 - Data Integrity & Merging: Datasets from multiple independent systems with different formats were mapped to a common format for simulations. 
 - Distribution Data: wires, generation, equipment 
 - Operational Data: circuit configuration, AMI & SCADA profiles, 
 - Smart Inverter Integration: Continued evaluation of configurations and functional assumptions need to be considered. These include Volt-VAR control on various circuit topologies and interaction of different control types across traditional and smart inverters. 
 - Computational Efficiency: The average processing time exceeds 10 hours per circuit for initial implementation. Node reduction is being explored to improve simulation performance, especially with respect to monthly updates. 
 - Network Updates: ICA triggers will identify network updates, but will likely require independent engineering verification and manual engineering override to process
 - Coverage: secondary systems and single phase lines do not have sufficient data to model 

These challenges mirror the ongoing long term refinements which continue to be recommended by the DER WG. The DER WG also highlighted uncertainty related to the implementation of the planning use case definition, including DER forecast and project deferral methods. 
