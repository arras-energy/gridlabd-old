The goal of the Grid Resilience Intelligence Project (GRIP) project is to assist distribution utilities in responding to grid events by:
 - Anticipate grid events using machine learning and artificial intelligence techniques with diverse data sources; 
 - Absorb grid events by employing validated control strategies for distributed energy resources; and
 - Reduce recovery time by managing distributed energy resources in the case of limited communications. 

GRIP builds on previous efforts to collect massive amounts of data to fine-tune grid operations, including the VADER project and other Grid Modernization Lab Consortium projects on distributed controls and cyber security. 

The project includes innovative application of artificial intelligence and machine learning for distribution grid resilience using predictive analytics, image recognition, increased “learning” and “problem solving” capabilities for anticipation of grid events. The GRIP project demonstrates distributed control theory with and without communications to absorb and recover from grid events. 

GRIP deploys, tests and validates emerging capabilities with multiple utility partners in North America. Anticipation analytics will be tested and validated with Southern California Edison; absorption algorithms will be tested in Vermont; and extremum seeking controls developed by LBNL will be tested with NRECA members. 

# Resilience Models

To effectively absorb and recover from grid events a grid resilience model was developed in GridLAB-D, which is coupled with python implemented control strategies. The core GridLAB-D code includes a physical failure model of the distribution power poles. Using basic parameters of poles, such as material type, pole size, pole-top equipment specifications and pole design factors, in conjunction with regional weather files, GridLAB-D simulates the conditions at which a particular pole may fail. 

In addition to the physical failure model, a degradation model addresses aging of the poles. An electrical pole is considered at the end of service life by the electrical utility company when the minimum shell thickness is less than 2 inches [Newbill1993]. Based on this assumption, the degradation model uses the difference between the inside and the outside pole’s core moment at the base of the structure where it is considered weakest. Note that the lifetime of the pole depends on the geographical location, pole treatment, and the weight of the pole-top equipment, all of which are described by the pole data and configuration. 

Applying the GridLAB-D pole model described above to GRIP resilience studies allows the simulation to capture the deficiencies of the electrical grid, given the electrical network models implemented in GridLAB-D. Utility provided cyme networks and DMS control models are used as test cases to provide realistic network conditions. The implementation of the electrical feeders start at substation level and capture components such as transformers, switches, capacitors, electrical lines and metered loads in the model. These models are load-driven and data-driven using real-world information from AMI and SCADA systems. 

The absorption component of GRIP includes strategies that allow the network to be broken down into virtual island components using GridLAB-D network modeling capabilities in the efforts to protect and restore the system, after being exposed to a vulnerability. The core implementation of GridLAB-D allows to model the conditions at which system faults occur and determine recovery tactics using algorithms developed by Packetized Energy through python modules. Using this feature, a fault is generated due to an extreme event, such as high wind, which breaks the network into a grid-isolated islands. The networks are assumed to be equipped with solar and battery technology to support the generation requirements within a virtual network island as seen in Figure. 6. The topology employs large switching devices under specific control strategies that disconnects portions of the grid based on the location of the fault of system vulnerability. 

[image:virtual_islanding_model.png]
Figure 1: Virtual Islanding test case for Grid Resilience modelling. (Courtesy of Packetized Energy)

Based on these algorithms, the tool developed for GRIP includes a UI developed by Presence PG. Behind a credentials verification system, as seen in Figure X, the GRIP UI dashboard is broken down into 3 components based on project requirements: anticipation, absorption, recovery and data. 

[image:dashboard.png]
Figure 2: Anticipation dashboard for GRIP tool. (Courtesy of Packetized Energy)

Using network model glm files as input, the anticipation tab displays the various available topologies and their vulnerability metric. By entering a particular simulation of interest, the topology of the network is shown. This section allows the user to navigate the distribution feeder and collect information regarding the network assets and weather conditions as seen in Figure X. 

[image:anticipation.png]
Figure 3: Anticipation simulation sample demonstration for GRIP tool. (Courtesy of Packetized Energy)

The absorption tab, as seen in Figure 3, identifies the fault location in the network. 
