# HeatFlowFEM
HeatFlowFEM is an advanced program designed for simulating heat flow in metal walls using the Finite Element Method (FEM). 

## Features
- **Heat Flow Simulation**: Conducts heat flow simulations in metal walls based on specified dimensions.
- **Finite Element Method (FEM)**: Utilizes FEM for accurate modeling of heat transfer phenomena.
- **Time-Dependent Calculations**: Computes heat flow over time to capture dynamic thermal behavior.
- **VTK formal Compatibility**: Generates results in .vtk format compatible with ParaView and other tools for easy visualization and analysis.
## Running the Program
To run HeatFlowFES and simulate heat flow in metal walls, follow these steps:

1. **Prepare Your Wall Grid File**: Create a file containing the necessary input parameters and wall geometry data as described in the provided format. Ensure that the file includes information such as simulation time, material properties, node coordinates, element connectivity, and boundary conditions.

2. **Update File Path in main.cpp**:
   - Open the `main.cpp` file in your preferred text editor.
   - Locate the section where the file path to the wall grid data is specified.
   - Replace the existing file path with the path to your prepared wall grid file.

3. **Compile the Code**:
   - Compile the HeatFlowFES source code to generate the executable binary.
   - Depending on your development environment and compiler, use the appropriate commands or build configurations to compile the code.
   - Ensure that there are no compilation errors before proceeding to the next step.

4. **Run the Program**:
   - Execute the compiled HeatFlowFES executable from the command line or terminal.
   - Depending on your operating system and compiler settings, you may need to specify additional command-line arguments or options.
   - Monitor the program output for any messages, warnings, or errors during the simulation process.

5. **Analyzing Results**:
   - After the simulation completes, analyze the results using ParaView or other visualization tools that are compatible with the .vtk format.
   - Load the output data generated by HeatFlowFES into ParaView to visualize temperature distributions, heat fluxes, and other relevant parameters.

## Preparing the Wall Grid file
1. Define the simulation Parameteres
   - SimulationTime [seconds]: Specify the total length of simulated time.
   - SimulationStepTime [seconds]: Determine the length of time between returned wall states.
2. Material Properties:
   - Conductivity [W/m·K]: Define the thermal conductivity of the wall material.
   - Alfa [W/m²·K]: Specify the heat transfer coefficient for the wall.
   - Tot [℃]: Set the ambient temperature of the environment in which the wall is placed.
   - InitialTemp [℃]: Specify the initial temperature of the wall.
   - Density [kg/m³]: Define the density of the wall material.
   - SpecificHeat [J⋅kg⁻¹⋅K⁻¹]: Specify the specific heat of the wall material.
4. Node and Element Definition:
The wall geometry serves as the foundation for simulating heat flow in MetalHeatSim and consists of nodes (vertices) and elements (quadrilaterals). Follow these steps to prepare the wall geometry effectively:
   - Define the number of nodes the wall grid is comprised of.
   - Define the number of elements the wall grid is comprised of.
   - Define the nodes of the wall grid by specifying their coordinates in the two-dimensional space as floating points. Nodes represent the vertices of the elements and serve as reference points for defining the geometry of the wall.
   - Define the elements of the wall grid by connecting the nodes. Each element is a quadrilateral formed by four nodes and represents a portion of the wall surface. Ensure that elements are defined in a sequential manner to maintain the integrity of the grid structure.
6. Boundary Conditions:
   - List the edge nodes used for calculating the boundary condition flow.
  
Ensure that the wall grid data is saved in a .txt file consistent with the provided structure. Double-check the accuracy of all parameters and configurations before proceeding with the simulation. Also, check the examplary test grids for working examples.
Your file should look like this:

```
SimulationTime [seconds]  
SimulationStepTime [seconds]
Conductivity [W/m·K]
Alfa [W/m²·K] 
Tot [℃] 
InitialTemp [℃]
Density [kg/m³] 
SpecificHeat [J⋅kg⁻¹⋅K⁻¹] 
Nodes number [X] //Number of nodes of the grid
Elements number [X] //Number of elements of the grid
*Node
[Node ID], [X coordinate], [Y coordinate]
*Element, type=DC2D4
[ID] [Node ID 1], [Node ID 2], [Node ID 3], [Node ID 4],
*BC  // List of edge nodes for calculating the Boundry Condition flow
[Node ID 1], [Node ID 2] ... [Node ID X]
```

## Contributing
Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.