#!/bin/bash

text="Aditya"

echo "${text^^}"

# base name
echo "$(basename)"
basename /usr/bin/sort         # --> "sort"
basename include/stdio.h .h    # --> "stdio"
basename -s .h include/stdio.h # --> "stdio"
basename -a any/str1 any/str2 - followed by "str2" >"str1"

# trap terminal like man page
# trap cleanup SIGINT SIGTERM

# tput civis  # == Hide Cursor
# tput cnrom  # == Restore Cursor
