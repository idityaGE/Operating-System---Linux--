#!/bin/bash

# === Conditional Statements === #

# if statement
echo "If statement:"

if [[ 1 -eq 1 ]]; then
    echo "1 is equal to 1"
fi

echo "Enter a number:"
read num

if [[ $num -gt 10 ]]; then
    echo "The number is greater than 10"
fi


# if-else statement
echo "If-else statement:"

if [[ $num > 10 ]]; then
    echo "The number is greater than 10"
elif [[ $num -eq 10 ]]; then
    echo "The number is equal to 10"
else
    echo "The number is less than 10"
fi


# === String Comparision === #
: '
[[ -z STR ]]        ->	Empty string
[[ -n STR ]]        ->	Not empty string
[[ STR == STR ]]    ->	Equal
[[ STR = STR ]]     ->	Equal (Same above)
[[ STR < STR ]]     ->	Less than (ASCII)
[[ STR > STR ]]     ->	Greater than (ASCII)
[[ STR != STR ]]    ->	Not Equal
[[ STR =~ STR ]]    ->	Regexp
'

echo "Enter your name:"
read name

if [ "$name" = "Alice" ]; then
    echo "Hello, Alice!"
else
    echo "You're not Alice."
fi


# === File Comparision === #
: '
[[ -e FILE ]]	    ->  Exists
[[ -d FILE ]]	    ->  Directory
[[ -f FILE ]]	    ->  File
[[ -h FILE ]]	    ->  Symlink
[[ -s FILE ]]	    ->  Size is > 0 bytes
[[ -r FILE ]]	    ->  Readable
[[ -w FILE ]]	    ->  Writable
[[ -x FILE ]]	    ->  Executable
[[ f1 -nt f2 ]]	  ->  f1 newer than f2
[[ f1 -ot f2 ]]	  ->  f2 older than f1
[[ f1 -ef f2 ]]	  ->  Same files
'

file="test.txt"

if [ -f "$file" ]; then
    echo "$file exists"
else
    echo "$file does not exist"
fi

# === Logical Operators === #
: '
[[ ! EXPR ]]	    ->  Not
[[ X && Y ]]	    ->  And
[[ X || Y ]]	    ->  Or
'

if [[ -f "$file" && -r "$file" ]]; then
    echo "$file is a readable file"
fi

: '
[[ X -a Y ]]  -> And
[[ X -o Y ]]  -> Or
'

# if [ "$1" = 'y' -a $2 -gt 0 ]; then
#     echo "yes"
# fi
# if [ "$1" = 'n' -o $2 -lt 0 ]; then
#     echo "no"
# fi