# Phase Field Code for 2D Cahn Hilliard

This repository contains a phase field code for simulating the Cahn Hilliard equation 2D. The code utilizes the phase field method to model phase transitions and interface dynamics in materials.


## Prerequisites

Before running the code, make sure you have the following dependencies installed:

- VTK 9.2
- FFTW3

## Compilation

To compile the code, simply use the provided Makefile:

```
make
```

## How to Run
Modify the input.txt file to specify the desired simulation parameters. Adjust the values according to your requirements.

Run the code using the following command:
```
./phase_field input.txt
```


## Input parameters description

| Parameter | Description | Default Value |
| --------- | ----------- | ---------------- |
| CALC_TYPE | Type of calculation to be performed(1: Cahn Hillard 2D(qualitative), 2: Cahn Hillard 2D (alloy)). | |
| FLUCTUATION | Initial random fluctuation of the microstructure. | 0.0001 |
| AVG_COMP |Average composition of the material. | 0.5 |
| SIZE_X, SIZE_Y and SIZE_Z | Size of the system in the x, y, and z directions. | 1 |
| MOBILITY | Coefficient controlling phase transformation rate. | 1 |
| DELTA_T | Time interval for the simulation. | 1 |
| KAPPA | Coefficient determining gradient energy influence. | 1 |
| DELTA_X, DELTA_Y and DELTA_Z | Grid size in the x, y, and z directions. | 0.1 |
| OUT_TIME_INTERVAL | Timestep for generating output. | 10 |
| TOTAL_TIME | Total duration of the calculation. | 200 |
| OUTPUT_FORMAT | Output microstructure format (VTI or PNG). | VTI |


## Input file example
Here is an example of an input.txt file:

```
CAL_TYPE = 1
FLUCTUATION = 0.0001
AVG_COMP = 0.4
SIZE_X = 200
SIZE_Y = 200
MOBILITY = 1 
DELTA_T = 1
KAPPA = 1
DELTA_X = 0.1
DELTA_Y = 0.1
OUT_TIME_INTERVAL = 10
TOTAL_TIME = 50
OUTPUT_FORMAT = PNG
```
