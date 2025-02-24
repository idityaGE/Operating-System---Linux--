#!/bin/bash

# Strings

# Declare a string
name="John Doe"

# Access a character
echo ${name:0:1}
echo ${name:1:1}

# String length
echo ${#name}

# Substring
echo ${name:0:4}

# Concatenate strings
greeting="Hello"
name="Adii"
combined="$greeting, $name!"
echo $combined

# Uppercase
echo ${name^}  # first letter only
echo ${name^^} # complete string

# Lowercase
echo ${name,}
echo ${name,,}

# Replace
# ${string/pattern/replacement}
echo ${name/John/Steve}

# Replace all
echo ${name//John/Steve}

# Removing Substring
str="Hello, Bash!"
new_str=${str/Bash/}
echo $new_str

# Spliting
str="apple,banana,orange"
IFS=',' read -ra fruits <<<"$str"

for fruit in "${fruits[@]}"; do
    echo "Fruit: $fruit"
done
