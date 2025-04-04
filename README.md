# Option Pricing using Crank-Nicolson Scheme

## Overview

This project implements a numerical solution for option pricing using the Crank-Nicolson scheme. It supports both European and American options, and includes functionality for calculating the 
Greeks (Delta, Gamma, Rho, Vega, and Theta) and visualizing them in an excel file. The project is written in C++20 and uses CMake with Ninja as the build system.

## Features

- Pricing of European and American options
- Support for call and put options
- Calculation of option Greeks
- Crank-Nicolson finite difference method for solving the Black-Scholes PDE
- Excel interface for easy use

## Installation

### Prerequisites

- C++17
- CMake

### Building the Project

1. Clone the repository
2. create a build folder and naviagte to it:
 ```
 mkdir build 
 cd build
 ```
3. then run the Cmake command:
```
cmake ..
```
### Cmake targets

There are two targets one can build:

1. **main**: This target is used for testing the Crank-Nicolson scheme. It compares the Crank-Nicolson price with the explicit formula of the Black-Scholes model and demonstrates the equivalence of American and European options for calls when the interest rate \( r > 0 \).

2. **non_regression_tests**: This target includes non-regression tests using Google Test.

To build these targets, you can use the following commands:

- To build the `main` or `non_regression_tests` targets:
```
make <target name>
```

## Usage

### Command Line Interface

After building the project and the targets, you can run the ``` main.exe ``` executable to price an option. (one could change the parameters of the option object in the main.)

### Excel Interface

The project includes an Excel interface for easy use. This allows you to use the option pricing and Greeks calculation functionality directly from Excel.

## Documentation

The project uses Doxygen for documentation. To generate the documentation, run the following command in the ``` doxygen ``` directory:
```
doxygen doxyfilerc.txt
```


