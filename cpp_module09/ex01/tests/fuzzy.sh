#!/bin/bash

# MAGIC LINE: This forces the script to run inside the tests/ directory,
# regardless of where you call it from in your terminal.
cd "$(dirname "$0")"

# Colors for pretty output
GREEN='\033[0;32m'
RED='\033[0;31m'
CYAN='\033[0;36m'
NC='\033[0m'

# Since we are in the tests/ folder, the RPN binary is one level up
RPN_EXEC="../build/bin/ex01"
TOTAL_TESTS=1000
PASSED=0
FAILED=0

echo -e "${CYAN}======================================${NC}"
echo -e "${CYAN}      RPN HARDCORE FUZZ TESTER        ${NC}"
echo -e "${CYAN}======================================${NC}"

# 1. Safety Check: Did you run make?
if [ ! -f "$RPN_EXEC" ]; then
    echo -e "\n${RED}[ERROR] RPN executable not found!${NC}"
    echo -e "It looks like '$RPN_EXEC' is missing."
    echo -e "Please go to your ex01 folder and run 'make' first!"
    exit 1
fi

# 2. Compile the generator
echo -e "${CYAN}Compiling C++98 RPN Generator...${NC}"
c++ -Wall -Wextra -Werror -std=c++98 rpn_generator.cpp -o rpn_generator
if [ $? -ne 0 ]; then
    echo -e "${RED}[ERROR] Failed to compile rpn_generator.cpp! Make sure it is in the tests/ folder.${NC}"
    exit 1
fi

echo -e "${CYAN}Starting $TOTAL_TESTS Valid Expression Tests...${NC}"

# Read the generated outputs (Process substitution prevents subshell variable loss)
while IFS='|' read -r expr expected; do
    # Run your RPN program
    output=$($RPN_EXEC "$expr" 2>&1)

    if [ "$output" == "$expected" ]; then
        ((PASSED++))
    else
        echo -e "\n${RED}[FAILED]${NC} Expression: $expr"
        echo "Expected: $expected"
        echo "Got:      $output"
        ((FAILED++))
    fi
done < <(./rpn_generator $TOTAL_TESTS)

echo -e "${CYAN}Starting Error Handling Tests...${NC}"

# Define intentional error cases (Expecting "Error" output)
ERROR_CASES=(
    "1 +"                # Not enough operands
    "1 2 3 +"            # Too many leftovers
    "5 0 /"              # Division by zero
    "(1 + 1)"            # Invalid tokens
    "a b +"              # Letters
    "1.5 2 +"            # Decimals
)

for err_expr in "${ERROR_CASES[@]}"; do
    output=$($RPN_EXEC "$err_expr" 2>&1)
    if [[ "$output" == *"Error"* ]]; then
        ((PASSED++))
    else
        echo -e "\n${RED}[FAILED ERROR TEST]${NC} Expression: $err_expr"
        echo "Expected standard error containing 'Error'"
        echo "Got: $output"
        ((FAILED++))
    fi
done

# Final Results
TOTAL=$((TOTAL_TESTS + ${#ERROR_CASES[@]}))
echo -e "\n======================================"
if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}SUCCESS! Passed $PASSED / $TOTAL tests.${NC}"
else
    echo -e "${RED}FAILURE! Passed $PASSED / $TOTAL tests. Failed $FAILED.${NC}"
fi
echo -e "======================================"

# Clean up the compiled generator
rm -f rpn_generator
