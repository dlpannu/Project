#!/bin/bash

# Ask for the path of the executable file
read -p "Enter the name of the source file: " source_file
read -p "Enter the desired executable file name: " executable_name

# Compile the source file
nvcc -pg -o "$executable_name" "$source_file"

# Run the executable
./"$executable_name"

# Generate profiling data and output analysis
gprof "$executable_name" gmon.out > analysis.txt

# Clean up profiling files (optional)
#rm gmon.out
