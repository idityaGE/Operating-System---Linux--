#!/bin/bash

# File operations

# === Create a file === #
touch myfile.txt

# === Write to a file === #
echo "Hello World" >myfile.txt

# === Append to a file === #
echo "Hello World" >>myfile.txt

# === Read from a file === #
cat myfile.txt

# === Read with while loop === #
while read line; do
    echo $line
done <myfile.txt

# === Check if file exists === #
if [ -f myfile.txt ]; then # can be done for directories as well
    echo "File exists"
else
    echo "File does not exist"
fi

# === Check if file is readable === #
if [ -r myfile.txt ]; then
    echo "File is readable"
else
    echo "File is not readable"
fi

# === Delete a file or directories === #
# rm myfile.txt
# rmdir mydir
# rm -r mydir

# === Copy a file === #
# cp myfile.txt myfile2.txt

# === Move a file === #
# mv myfile2.txt /path/to/destination/

# === Rename a file === #
# mv myfile3.txt myfile4.txt
