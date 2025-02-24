#!/bin/bash

# === Basic Loop === #
for i in 1 2 3 4 5; do
    echo "Number: $i"
done

for i in $(seq 1 5); do
    echo "Number: $i"
done

# === Loop with Array === #
arr=("apple" "banana" "cherry")
for i in ${arr[@]}; do
    echo "Fruit: $i"
done

# === Loop with Range === #
for i in {1..5}; do
    echo "Number: $i"
done

# === Loop with Range and Step === #
for i in {1..10..2}; do
    echo "Number: $i"
done

# === Loop with Command Output === #
for i in $(seq 1 5); do
    echo "Number: $i"
done

# === Loop with Command Output and Step === #
for i in $(seq 1 2 10); do
    echo "Number: $i"
done

# === Loop with Command Output and Step === #
for i in $(seq 10 -2 1); do
    echo "Number: $i"
done

# === While Loop === #
i=1
while [ $i -le 5 ]; do
    echo "Number: $i"
    ((i++))
done

# === Break and Continue === #
for number in $(seq 1 3); do
    if [[ $number == 2 ]]; then
        continue
    fi
    echo "$number"
done

for number in $(seq 1 3); do
    if [[ $number == 2 ]]; then
        break # Skip entire rest of loop.
    fi
    echo "$number"
done

# === Nested Loop === #
for i in {1..3}; do
    for j in {1..3}; do
        echo "i: $i, j: $j"
    done
done

# === Until === #
# An until loop is the opposite of while. It keeps running until the condition becomes true.

count=1
until [ $count -gt 5 ]; do
    echo "Count: $count"
    ((count++))
done

# === Looping Through Files === #
for file in *; do
    echo "File: $file"
done

# === Looping Through Files with Extension === #
for file in *.sh; do
    echo "File: $file"
done

# cat *.sh | while read line; do
#     echo $line
# done
