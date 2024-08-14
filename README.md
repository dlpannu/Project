# Monte Carlo Simulation for Barrier Option Pricing using CUDA

# Overview

This project implements a Monte Carlo simulation for pricing barrier options using NVIDIA's CUDA toolkit. Barrier options are a type of exotic option where the payoff depends on whether the underlying asset's price reaches a certain barrier level. The Monte Carlo method is used here to estimate the price of these options by simulating numerous paths of the underlying asset price.
Features

    CUDA Acceleration: Utilizes CUDA to parallelize the Monte Carlo simulation, significantly speeding up the computation.
    Barrier Option Pricing: Supports various types of barrier options including knock-in and knock-out options.
    Flexible Configuration: Allows easy adjustment of parameters such as number of simulations, time steps, and option characteristics.

# Requirements

    Hardware: NVIDIA GPU with CUDA Compute Capability (e.g., GTX 1060, RTX 2080, or newer).
    Software:
        CUDA Toolkit (version 11.0 or higher)
        NVIDIA Driver (compatible with the installed CUDA version)
        CMake (for building the project)
        A compatible C++ compiler (e.g., GCC for Linux, MSVC for Windows)

# Parameters:

    <option_type>: Type of the option (e.g., "call", "put").
    <barrier_type>: Type of the barrier (e.g., "knock-in", "knock-out").
    <S0>: Initial stock price.
    <K>: Strike price.
    <barrier>: Barrier level.
    <T>: Time to maturity (in years).
    <sigma>: Volatility (standard deviation of the stock return).
    <r>: Risk-free interest rate.
    <N>: Number of simulations.
    <M>: Number of time steps per simulation.

# Results

The output of the simulation will provide the estimated price of the barrier option based on the Monte Carlo simulation.
Code Structure

    src/: Contains source code files for the CUDA kernel and host code.
    include/: Contains header files with function declarations and definitions.
    CMakeLists.txt: CMake configuration file for building the project.
    README.md: This documentation file.

# Contributing

Feel free to contribute to this project by submitting issues or pull requests. Ensure to follow the coding standards and include test cases where applicable.

# License

This project is licensed under the MIT License. See the LICENSE file for more details.
# Acknowledgements

    NVIDIA CUDA Toolkit: For GPU acceleration.
    Monte Carlo Simulation Techniques: For financial option pricing methods.

# Contact

For any questions or feedback, please contact:

    Author: Devi Lal
    Email: devilpannu96@gmail.com
    GitHub: dlpannu

