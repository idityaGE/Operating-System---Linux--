#!/bin/bash

# Que : 1. Write down a shell script which lists top 10 processes using highest amount of main memory.

# === Version 1 === #

echo "Top 10 process with high memory usages"
#printf "%-10s %-10s %-15s %-s\n" "PID" "USER" "MEM" "COMMAND"
echo "-------------------------------------------"
ps -eo pid,user,%mem,cmd --sort=-%mem | head -11

# === Version 2 === #

# trap cleanup SIGINT SIGTERM

# cleanup() {
#   echo "Exiting..."
#   tput cnorm  # Restore cursor
#   exit 0
# }

# tput civis

# while true; do
#   text=$(ps -eo pid,user,%mem,cmd --sort=-%mem | head -11)
#   echo "$text" >> data.txt
#   echo -e "----------------------------------------------------------------\n"
#   echo "$text"

#   echo -e "Enter 'q' to exit\n"
#   read -t 3 exit_cmd
#   if [[ $exit_cmd == "q" ]]; then
#       cleanup
#   fi

#   sleep 3
# done
