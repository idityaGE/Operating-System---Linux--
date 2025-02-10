#!/bin/bash

# path=$1

# get_path() {
#   if [[ -z $path ]]; then
#     echo "Please Provide the path to perform the operation"
#     echo "For more information try '-h'"
#   elif [[ $path == /* ]]; then
#      echo "'$path' is an absolute path"
#   elif [[ $path == ./* || $path == ../* ]]; then
#      echo "'$path' is explicitly relative to current directory"
#   else
#     echo "'$path' is relative to current directory"
#   fi

#   if [ -e "$path" ]; then 
#       if [ -w "$path" ]; then
#           echo "Path is writable"
#           return 0
#       else
#           echo "Warning: Path is not writable"
#           return 1
#       fi

#       if [ -d "$path" ]; then
#           echo "Path is a valid directory"
#           return 0
#       else
#           echo "Path exists but is neither a file nor a directory"
#           return 1
#       fi
#   else
#     echo ""
#     return 1
#   fi

# }

read_file_name() {
    local current_path="${1:-$PWD}"  # Get path as parameter
    
    # Check if directory exists
    # if [ ! -d "$current_path" ]; then
    #     echo "Error: '$current_path' is not a valid directory"
    #     return 1
    # fi
    
    echo "Scanning directory: $current_path"
    
    # Loop through items in current directory
    for file in "$current_path"/*; do
        if [[ -f "$file" ]]; then
            echo "File: $file"
        elif [[ -d "$file" ]]; then
            # echo "Directory: $file"
            # Recursive call for subdirectories
            read_file_name "$file"
        fi
    done
}

read_file_name
