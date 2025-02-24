#!/bin/bash

# === Function === #

# Define a function
function greet() {
  echo "Hello, World!"
}
# OR
greet() {
  echo "Hello, $1!"
}

# Call the function
greet Adii

# === Function with Return === #
add_number() {
  sum=$(($1 + $2))
  echo $sum
}
result=$(add_number 10 20)
echo "Sum: $result"

check_odd_even() {
  if [[ $(($1 % 2)) == 0 ]]; then
    return 0 # Even
  else
    return 1 # Odd
  fi
}
check_odd_even 4
if [[ $? == 0 ]]; then # $? stores the exit status of the last command, where 0 typically indicates success.
  echo "Even"
else
  echo "Odd"
fi

# === Function with Default Parameter === #
greet() {
  name=${1:-"World"}
  echo "Hello, $name!"
}
greet
greet Adii
