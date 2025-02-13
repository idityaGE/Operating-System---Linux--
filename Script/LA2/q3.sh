#!/bin/bash

# Trap Ctrl+C to cleanup before exit
trap cleanup SIGINT SIGTERM

get_sys_details() {
    echo "System Information:"
    echo "System Uptime: $(uptime)"
    echo "Load Average: $(cat /proc/loadavg)"
    echo "Total Process : $(ps aux | wc -l)"

    echo ""
}

get_mem_info() {
    echo "Memory Info :"
    echo "$(free -h | tail +2)"

    echo ""
}

get_pocesses_info() {
    echo "Process Details: "
    ps -eo pid,pcpu,pmem,stat,sess,user,cmd

    echo ""
}

print_options() {
    echo "Options :"
    echo " 1 )-> Refresh"
    echo " 2 )-> Get Process details by PID "
    echo " 3 )-> Exit "
}

get_process_info_by_id() {
    echo "Enter the PID : "
    read PID
    if ! ps --p $PID > /dev/null; then
        echo "Process with PID $pid not found"
        return 1
    fi

    echo "Detailed information for PID $pid:"
    ps -p $pid -o pid,ppid,user,%cpu,%mem,nice,stat,start,time,command --headers
}

cleanup() {
    echo -e "\nExiting.."
    tput cnorm  # Restore cursor
    exit 0
}

main() {
    while true;do
        clear
        get_sys_details
        get_mem_info
        get_pocesses_info
        print_options

        echo "enter options : "
        read -t 2 options 
        
        case $options in
            1) continue ;;
            2) get_process_info_by_id ;;
            3) cleanup ;;
            *) sleep 1 ;;
        esac


    done
}

# Hide cursor
tput civis

main