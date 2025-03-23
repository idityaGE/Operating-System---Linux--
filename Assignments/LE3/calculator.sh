#!/bin/bash

# Scientific Calculator Script
# Supports: Trigonometric operations, Logarithms, Exponents and basic operations

# Define math library with extended precision
BC_LIB=$(
    cat <<'EOL'
scale=10;
define pi() {
  return 4*a(1);
}
define to_rad(x) {
  return x*pi()/180;
}
EOL
)

show_menu() {
    echo "===== Calculator ====="
    echo "1. Add       2. Subtract"
    echo "3. Multiply  4. Divide"
    echo "5. Modulo    6. Power"
    echo "7. sin(x)    8. cos(x)"
    echo "9. tan(x)    10. ln(x)"
    echo "11. log10(x) 12. sqrt(x)"
    echo "13. e^x      14. |x|"
    echo "15. Factorial 16. Expression"
    echo "0. Exit"
    echo "======================"
}

# Function to get numeric input with validation
get_number() {
    local prompt=$1
    local num

    while true; do
        read -p "$prompt" num
        if [[ $num =~ ^[+-]?[0-9]*\.?[0-9]+$ ]]; then
            echo "$num"
            return 0
        else
            echo "Error: Please enter a valid number."
        fi
    done
}

# Function to calculate factorial
factorial() {
    local n=$1
    local result=1

    if ((n < 0)); then
        echo "Error: Factorial is not defined for negative numbers."
        return 1
    elif ((n == 0)); then
        echo 1
        return 0
    fi

    for ((i = 1; i <= n; i++)); do
        result=$((result * i))
    done

    echo "$result"
}

# Main calculator logic
calculate() {
    local choice=$1
    local result

    case $choice in
    1) # Addition
        n1=$(get_number "Enter first number: ")
        n2=$(get_number "Enter second number: ")
        result=$(echo "$BC_LIB; $n1 + $n2" | bc -l)
        ;;
    2) # Subtraction
        n1=$(get_number "Enter first number: ")
        n2=$(get_number "Enter second number: ")
        result=$(echo "$BC_LIB; $n1 - $n2" | bc -l)
        ;;
    3) # Multiplication
        n1=$(get_number "Enter first number: ")
        n2=$(get_number "Enter second number: ")
        result=$(echo "$BC_LIB; $n1 * $n2" | bc -l)
        ;;
    4) # Division
        n1=$(get_number "Enter numerator: ")
        n2=$(get_number "Enter denominator: ")
        if [[ $(echo "$n2 == 0" | bc -l) -eq 1 ]]; then
            echo "Error: Division by zero!"
            return 1
        fi
        result=$(echo "$BC_LIB; $n1 / $n2" | bc -l)
        ;;
    5) # Modulo
        n1=$(get_number "Enter dividend: ")
        n2=$(get_number "Enter divisor: ")
        result=$(echo "$BC_LIB; $n1 % $n2" | bc -l)
        ;;
    6) # Power
        n1=$(get_number "Enter base: ")
        n2=$(get_number "Enter exponent: ")
        result=$(echo "$BC_LIB; $n1 ^ $n2" | bc -l)
        ;;
    7) # Sine
        n1=$(get_number "Enter angle in degrees: ")
        result=$(echo "$BC_LIB; s(to_rad($n1))" | bc -l)
        ;;
    8) # Cosine
        n1=$(get_number "Enter angle in degrees: ")
        result=$(echo "$BC_LIB; c(to_rad($n1))" | bc -l)
        ;;
    9) # Tangent
        n1=$(get_number "Enter angle in degrees: ")
        if [[ $(echo "($n1 - 90) % 180 == 0" | bc -l) -eq 1 ]]; then
            echo "Error: Tangent is undefined at $n1 degrees."
            return 1
        fi
        result=$(echo "$BC_LIB; s(to_rad($n1))/c(to_rad($n1))" | bc -l)
        ;;
    10) # Natural logarithm
        n1=$(get_number "Enter a positive number: ")
        if [[ $(echo "$n1 <= 0" | bc -l) -eq 1 ]]; then
            echo "Error: Logarithm is defined only for positive numbers."
            return 1
        fi
        result=$(echo "$BC_LIB; l($n1)" | bc -l)
        ;;
    11) # Base-10 logarithm
        n1=$(get_number "Enter a positive number: ")
        if [[ $(echo "$n1 <= 0" | bc -l) -eq 1 ]]; then
            echo "Error: Logarithm is defined only for positive numbers."
            return 1
        fi
        result=$(echo "$BC_LIB; l($n1)/l(10)" | bc -l)
        ;;
    12) # Square root
        n1=$(get_number "Enter a non-negative number: ")
        if [[ $(echo "$n1 < 0" | bc -l) -eq 1 ]]; then
            echo "Error: Square root is not defined for negative numbers."
            return 1
        fi
        result=$(echo "$BC_LIB; sqrt($n1)" | bc -l)
        ;;
    13) # Exponential e^x
        n1=$(get_number "Enter exponent: ")
        result=$(echo "$BC_LIB; e($n1)" | bc -l)
        ;;
    14) # Absolute value
        n1=$(get_number "Enter a number: ")
        result=$(echo "$BC_LIB; if($n1<0) -$n1 else $n1" | bc -l)
        ;;
    15) # Factorial
        n1=$(get_number "Enter a non-negative integer: ")
        if [[ ! $n1 =~ ^[0-9]+$ ]]; then
            echo "Error: Factorial requires a non-negative integer."
            return 1
        fi
        result=$(factorial "$n1")
        ;;
    16) # Evaluate an expression
        read -p "Enter a mathematical expression: " expression
        result=$(echo "$BC_LIB; $expression" | bc -l 2>/dev/null)
        if [[ $? -ne 0 ]]; then
            echo "Error: Invalid expression."
            return 1
        fi
        ;;
    *)
        echo "Invalid option!"
        return 1
        ;;
    esac

    # Format result to remove trailing zeros if it's a whole number
    if [[ $result =~ \.0+$ ]]; then # regex used here
        result=$(echo "$result" | sed 's/\.0*$//')
    fi

    echo "Result: $result"
}

# Main program loop
while true; do
    show_menu
    read -p "Enter your choice (0-16): " choice

    if [[ $choice -eq 0 ]]; then
        echo "Thank you for using the Scientific Calculator!"
        exit 0
    elif [[ $choice -ge 1 && $choice -le 16 ]]; then
        calculate "$choice"
    else
        echo "Invalid option! Please select a number between 0 and 16."
    fi

    echo
    read -p "Press Enter to continue..."
    clear
done
