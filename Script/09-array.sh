#!/bin/bash

# Arrays

# Declare an array
fruits=('Apple' 'Banana' 'Orange')

# Access an element
echo ${fruits[0]}
echo ${fruits[1]}

# Array length
echo "Length of array: ${#fruits[@]}"

# Access all elements
echo ${fruits[@]}

# Access all elements with index
for i in ${!fruits[@]}; do
    echo "$i: ${fruits[$i]}"
done

# Access all elements
for fruit in ${fruits[@]}; do
    echo $fruit
done

# Add an element
fruits[3]='Mango'
echo ${fruits[@]}

fruits+=('Pineapple')
echo ${fruits[@]}

# Remove an element
unset fruits[2]
echo ${fruits[@]}


# === Associative arrays === #
declare -A capitals
capitals=([India]='New Delhi' [USA]='Washington' [UK]='London')
echo "${capitals[India]}"

declare -A fruits_price
fruits_price=(["Apple"]=100 ["Banana"]=50 ["Orange"]=80)
fruits_price+=([Mango]=150)
echo "${#fruits_price[@]}"