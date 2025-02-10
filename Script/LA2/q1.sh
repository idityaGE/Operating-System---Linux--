#!/bin/bash

echo "Top 10 process with high memory usages"
#printf "%-10s %-10s %-15s %-s\n" "PID" "USER" "MEM" "COMMAND"
echo "-------------------------------------------"

ps -eo pid,user,%mem,cmd --sort=-%mem | head -11
