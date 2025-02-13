#!/bin/bash

# Trap Ctrl+C to cleanup before exit
trap cleanup SIGINT SIGTERM

get_sys_details() {
    echo "System Information:"
    echo "System Uptime: $(uptime)"
    echo "Load Average: $(cat /proc/loadavg)"
    echo "Total Processes: $(ps aux | wc -l)"
    echo ""
}

get_mem_info() {
    echo "Memory Info:"
    echo "$(free -h | tail -n +2)"
    echo ""
}

get_processes_info() {
    echo "Process Details:"
    ps -eo pid,pcpu,pmem,stat,sess,user,cmd
    echo ""
}

print_options() {
    echo "Options:"
    echo " 1) Refresh"
    echo " 2) Get process details by PID"
    echo " 3) Exit"
}

get_process_info_by_id() {
    echo -n "Enter the PID: "
    read PID

    if ! ps -p "$PID" > /dev/null; then
        echo "Process with PID $PID not found."
        return 1
    fi

    echo "Detailed information for PID $PID:"
    ps -p "$PID" -o pid,ppid,user,%cpu,%mem,nice,stat,start,time,command --headers

    # Pause to let user read the output before refreshing
    read -p "Press enter to continue..."
}

cleanup() {
    echo -e "\nExiting..."
    tput cnorm  # Restore cursor visibility
    exit 0
}

main() {
    while true; do
        clear
        get_sys_details
        get_mem_info
        get_processes_info
        print_options

        echo -n "Enter option: "
        read -t 5 option  # Read input with a timeout

        case $option in
            1) continue ;;
            2) get_process_info_by_id ;;  # Calls function without refreshing
            3) cleanup ;;
            *) sleep 2 ;;  # Avoids immediate refresh if invalid input
        esac
    done
}

# Hide cursor
tput civis

main
