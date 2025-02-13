#!/bin/bash

path=${1:-$PWD} # by default its the current working directory
echo -e "Processing Path: $path\n"

# validate path
validate_path() {
  if [[ ! -d "$path" ]]; then
    echo "Error: '$path' is not a valid directory"
    exit 1
  fi
  if [[ ! -w "$path" ]]; then
    echo "Warning: Path is not writable"
    exit 1
  fi
}

convert_to_uppercase() {
  : ' Example :
file     :	/home/user/documents/report.pdf
dir      :	/home/user/documents
base     :	report.pdf
ext      :	pdf
name     :	report
'

  local file="$1"
  local dir=$(dirname -- "$file")
  local base=$(basename -- "$file")
  # `##`` is used to remove the longest matching prefix before the last . (dot).
  local ext="${base##*.}"
  # `%`` is used to remove the shortest matching suffix after the last . (dot).
  local name="${base%.*}"

  # there is ext of the file
  [[ $base == $ext ]] && ext=""

  # Convert name to uppercase
  local new_name="${name^^}"
  local count=1

  # Add version number if file already exists
  # what if the filename exist with the counter
  while [[ -e "$dir/$new_name.$ext" ]]; do
    formatted_count=$(printf "%02d" "$count")
    new_name="${name^^}.$formatted_count"
    ((count++))
  done

  echo "$dir/$new_name.$ext"
}

# *.txt --> .tex
convert_txt_to_tex() {
  local file="$1"
  local dir=$(dirname -- "$file")
  local base=$(basename -- "$file")

  if [[ "$base" == *.txt ]]; then
    echo "$dir/${base%.txt}.tex"
  else
    echo "$file"
  fi
}

process_directory() {
  local current_path="$1"

  for file in "$current_path"/*; do
    if [[ -f "$file" ]]; then
      new_name=$(convert_to_uppercase "$file")
      new_name=$(convert_txt_to_tex "$new_name")
      mv "$file" "$new_name"
      echo "Renamed: $file -> $new_name"
    elif [[ -d "$file" ]]; then
      process_directory "$file" # Recursive call for subdirectories
    fi
  done
}

validate_path
process_directory "$path"
