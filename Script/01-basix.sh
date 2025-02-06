#!/bin/bash 
# == Shebang Line: which tells the system that this is a bash script and should be run with /bin/bash == #

#~ Bash (Bourne Again SHell)

# To run the script, you need to make it executable by running `chmod +x basix.sh`

# === inline Bash comment === #
: '
This is a multiline comment.
You can write anything here.
'

echo "Hello, Bash!"

# === Variables === #
# Variables are used to store data that can be used later in the script.
# Bash variables do not need a type (they are treated as strings by default).
# NOTE: There should be no spaces around the `=` sign when assigning variables.

name="Aditya"
age=19

echo "My name is $name and I am $age years old."
# echo '$name'        # This will print $name
# echo "$name"        # This will print Aditya
# echo "${name}"      # This will print Aditya
# echo $name          # This will print Aditya

# === User Input === #
# You can take user input using the `read` command.

echo "Enter your name: "
read username
echo "Hello, $username!"


# === Command Substitution === #
# Command substitution allows the output of a command to replace the command itself.
# Syntax: $(command)

today=$(date +%A)
echo "Today is $today."