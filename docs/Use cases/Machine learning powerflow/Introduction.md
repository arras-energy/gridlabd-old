[[/Use cases/Machine learning powerflow/Introduction]]

Power system simulation engines are essential tools for power grid operation and planning by utility companies and academic researchers. 
Power system modeling is inherently challenging due to the wide range of timescales of grid operation. 
For example, voltage regulation happens on the timescale of seconds to minutes, while seasonal temperature variation occurs on the timescale of several months. 
Therefore, simulations are often run over a long horizon with a small simulation timestep, which may require millions of solver iterations. 
This can be extremely computationally expensive, especially for scenario-based analysis, and may require users to utilize high performance computing resources.

GridLAB-D uses quasi-static timeseries (QSTS) simulation, where the inputs to the powerflow solver vary from one timestep to another, but the powerflow at each timestep is assumed to be static. 
The bus voltages at a specific timestep is only dependent on the power injections at that timestep and not any previous timestep. 
Powerflow solutions must be performed chronologically because of the time-dynamics of equipment connected to the network (thermostatically controlled loads, voltage regulators, and capacitor banks), which prevents sequential solutions from being parallelized.

QSTS powerflow solvers utilize either Newton Raphson or fixed point iterative methods to solve the powerflow equations. 
These methods have been highly optimized and are already very efficient. 
Therefore, recent work on accelerating powerflow simulations has focused primarily on either reducing the number of powerflow solutions, reducing the problem complexity, or replacing the powerflow solver altogether. 
Approaches include reducing feeder size, parallelizing solutions through geographic or temporal decomposition, reducing the total number of powerflow solutions through variable timestepping, linearizing the powerflow equations, vector quantization, and approximating the powerflow mappings using machine learning-based methods. 
Temporal decomposition and variable timestepping can lead to errors in modeling the dynamic response of controllable devices to changes in voltage. 
Methods such as vector quantization, geographic decomposition, feeder simplification, linearization of the powerflow equations, and machine learning-based approximation can potentially lead to errors in the estimated voltage profile.

Past studies have shown that machine learning-based methods in particular can result in highly accurate approximation of the powerflow equations. 
These studies have focused primarily on linear regression, support-vector regression, and neural networks with relatively simple architectures. 
These data-driven methods would be particularly useful in applications where real-time simulation is required. 
These models can also be used in cases where no network model is available but there is significant historical network data.

The objective of this use-case is to develop a machine learning-based powerflow solver that can replace or be used in tandem with a traditional powerflow solvers to accelerate simulation speed. 
This requires development and implementation in GridLAB-D of data-driven methods for powerflow approximation, validation of model accuracy, scalability, robustness, and computational complexity on a variety of network scenarios. 

# Publications

* [http:Buechler et al, Machine Learning-Based Power Flow Solver, 2020.pdf]
* [http:Powell et al, Fast Solutions in Power System Simulation, submitted to HICSS, 15-June-2019.pdf]
