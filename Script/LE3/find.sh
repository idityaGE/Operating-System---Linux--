#!/bin/bash

# Write a shell script which works as “find” command. This script should be able to find
# out files/ directory/ or a string representing some portion of directory or file names.
# I will be happy to see if you can also implement regular expressions.

if [[ $# -eq 0 ]]; then
  echo "Search arguments is not provided !"
  echo "Usage: $0 <args> [directory]"
  exit 1
fi

args=$1
start_dir="${2:-$PWD}"

if [[ ! -d "$start_dir" ]]; then
  echo "Error: '$start_dir' is not a valid directory"
  exit 1
fi

process_directory() {
  local current_path="$1"
  local file
  # echo -e "Searching in : $current_path\n"

  for file in "$current_path"/*; do
    [[ -e $file ]] || continue

    if echo "$file" | grep -q -i "$args"; then
      # printing with color highlighting for better ux
      echo "$file" | grep --color=auto -i "$args"
    fi

    if [[ -d "$file" && -r "$file" ]]; then
      process_directory "$file"
    fi
  done
}

echo "Searching for '$args' in '$start_dir'..."
process_directory "$start_dir"
