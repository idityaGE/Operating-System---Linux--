#!/bin/bash

# === Pass Arguments to a Script === #
# You can pass arguments to a script when running it from the command line.
# These arguments are stored in variables with a number sign ($).
# Accessing Arguments:
# $0 - Name of the script
# $1, $2, $3, ... - First, second, third, ... arguments
# $@ - All arguments as separate strings
# $# - Number of arguments
# $* - All arguments as a single string

echo "Script Name: $0"
echo "First Argument: $1"
echo "Second Argument: $2"
echo "All Arguments: $@"
echo "Number of Arguments: $#"
echo "All Arguments as a Single String: $*"

# To run the script with arguments, use the following command:
# bash args.sh Aditya 19

# Output:
# Script Name: args.sh
# First Argument: Aditya
# Second Argument: 19
# All Arguments: Aditya 19
# Number of Arguments: 2
# All Arguments as a Single String: Aditya 19

# === Using Flags with Arguments === #
# You can use flags with arguments to make your script more versatile.
# num1=$1
# num2=$2
# sum=$((num1 + num2))
# echo "Sum of $num1 and $num2 is $sum."
