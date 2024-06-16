# Cab Allocation System

## Overview

This project includes a cab allocation system implemented in C++ and Python. The main functionalities are handled by two primary files: `cab_allocation.cpp` and `cab_simulator.py`. Additionally, there are two supplementary files that demonstrate the algorithms for finding the shortest path and locating the nearest cabs to an employee.

## Setup Instructions

1. Ensure that the dataset is placed in the same directory as the code files.

## Compilation and Execution

### Step 1: Compile and Run `cab_allocation.cpp`

1. Open a terminal or command prompt.
2. Navigate to the directory containing the files.
3. Compile the C++ program using the following command:

    ```sh
    g++ cab_allocation.cpp -o cab_allocation
    ```

4. Run the compiled program:

    ```sh
    ./cab_allocation
    ```

### Step 2: Run `cab_simulator.py`

1. After running the `cab_allocation` program, execute the Python script using the following command:

    ```sh
    python ./cab_simulator.py
    ```

## Additional Files

The other two files in the project are included to demonstrate specific algorithms:

- **Shortest Path Algorithm:** Demonstrates the method for finding the shortest path.
- **Nearest Cab Algorithm:** Shows how to identify the cabs that are closest to an employee.
