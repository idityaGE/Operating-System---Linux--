#!/bin/bash

# Piping and redirection

# === Redirection === #
# 1. Output redirection
# > - overwrites the file
# >> - appends to the file
echo "This is a test" >output.txt
echo "This is an appended line" >>output.txt

# 2. Input redirection
# < - read from a file
cat <output.txt

# === Piping === #
# |
cat output.txt | grep -i "test"
ps aux | grep "bash" | ws -l
