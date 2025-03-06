#!/bin/bash

# Write a shell script which works as “find” command. This script should be able to find
# out files/ directory/ or a string representing some portion of directory or file names.
# I will be happy to see if you can also implement regular expressions.

args=$1
if [[ -z $args ]]; then
  echo "Please provide the arguments to find !"
  exit 1
fi

process_directory() {
  local current_path="$1"
  # echo -e "Searching in : $current_path\n"

  for file in "$current_path"/*; do
    [[ -e $file ]] || continue

    if echo "$file" | grep -i "$args"; then
      echo "$file"
    fi

    if [[ -d "$file" && -r "$file" ]]; then
      process_directory "$file"
    fi
  done
}

process_directory "$PWD"
