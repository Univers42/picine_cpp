#!/bin/bash

# --- Colors ---
RESET="\033[0m"
BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
CYAN="\033[36m"

# --- Executable ---
EXEC="./build/bin/ex04"

# Make sure the program is compiled
make > /dev/null 2>&1

if [ ! -f "$EXEC" ]; then
    echo -e "${RED}${BOLD}Error: $EXEC not found! Run 'make' first.${RESET}"
    exit 1
fi

echo -e "${BOLD}${CYAN}==============================================${RESET}"
echo -e "${BOLD}${CYAN}       KMP REPLACER - AUTOMATED TESTER        ${RESET}"
echo -e "${BOLD}${CYAN}==============================================${RESET}\n"

# Helper variables
PASSED=0
FAILED=0

# Helper function to run a test
# Usage: run_test "Test Name" "File Content" "s1" "s2" "Expected Content" "Expected Exit Code"
run_test() {
    TEST_NAME="$1"
    CONTENT="$2"
    S1="$3"
    S2="$4"
    EXPECTED="$5"
    EXPECTED_CODE="$6"

    FILE="test_input.txt"
    OUT_FILE="${FILE}.replace"

    # Create the input file
    echo -n "$CONTENT" > "$FILE"

    # Run the program
    $EXEC "$FILE" "$S1" "$S2" > /dev/null 2>&1
    ACTUAL_CODE=$?

    # Check exit code
    if [ "$ACTUAL_CODE" -ne "$EXPECTED_CODE" ]; then
        echo -e "${RED}[FAIL]${RESET} $TEST_NAME"
        echo -e "       ${YELLOW}Expected exit code: $EXPECTED_CODE, Got: $ACTUAL_CODE${RESET}"
        ((FAILED++))
        rm -f "$FILE" "$OUT_FILE"
        return
    fi

    # If we expect success, check the file content
    if [ "$EXPECTED_CODE" -eq 0 ]; then
        if [ ! -f "$OUT_FILE" ]; then
            echo -e "${RED}[FAIL]${RESET} $TEST_NAME"
            echo -e "       ${YELLOW}Output file '$OUT_FILE' was not created!${RESET}"
            ((FAILED++))
            rm -f "$FILE" "$OUT_FILE"
            return
        fi

        ACTUAL=$(cat "$OUT_FILE")
        if [ "$ACTUAL" == "$EXPECTED" ]; then
            echo -e "${GREEN}[PASS]${RESET} $TEST_NAME"
            ((PASSED++))
        else
            echo -e "${RED}[FAIL]${RESET} $TEST_NAME"
            echo -e "       ${YELLOW}Expected: '$EXPECTED'${RESET}"
            echo -e "       ${YELLOW}Got     : '$ACTUAL'${RESET}"
            ((FAILED++))
        fi
    else
        # If we expected a failure (like bad arguments), getting here means it passed
        echo -e "${GREEN}[PASS]${RESET} $TEST_NAME"
        ((PASSED++))
    fi

    # Cleanup
    rm -f "$FILE" "$OUT_FILE"
}

# ------------------------------------------------------------------------------
# 1. NORMAL CASES
# ------------------------------------------------------------------------------
run_test "Basic Replacement" "hello world" "world" "42" "hello 42" 0
run_test "Multiple Occurrences" "foo bar foo baz foo" "foo" "qux" "qux bar qux baz qux" 0
run_test "Multiline Replacement" "line1\nline2\nline3" "line" "row" "row1\nrow2\nrow3" 0

# ------------------------------------------------------------------------------
# 2. EDGE CASES (THE ONES EVALUATORS WILL TRY)
# ------------------------------------------------------------------------------
run_test "s2 is Empty (Deletion)" "remove the bad word" "bad " "" "remove the word" 0
run_test "s1 is Empty (Should Error)" "hello" "" "world" "" 1
run_test "Empty File" "" "findme" "replace" "" 0
run_test "No Occurrences Found" "untouched text" "missing" "here" "untouched text" 0

# ------------------------------------------------------------------------------
# 3. KMP ALGORITHM STRESS TESTS
# ------------------------------------------------------------------------------
run_test "Continuous Pattern Overlap" "aaaaa" "aa" "b" "bba" 0
run_test "S1 is the entire file" "exact match" "exact match" "perfect" "perfect" 0
run_test "S1 is longer than file" "short" "very long string" "fail" "short" 0
run_test "S2 is longer than S1" "a" "a" "abcde" "abcde" 0

# ------------------------------------------------------------------------------
# 4. SYSTEM ERRORS (Files that don't exist or permissions)
# ------------------------------------------------------------------------------
echo -n "unreadable" > no_perms.txt
chmod 000 no_perms.txt
$EXEC "no_perms.txt" "a" "b" > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}[PASS]${RESET} Missing Read Permissions"
    ((PASSED++))
else
    echo -e "${RED}[FAIL]${RESET} Missing Read Permissions (Program did not exit 1)"
    ((FAILED++))
fi
rm -f no_perms.txt

$EXEC "does_not_exist.txt" "a" "b" > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}[PASS]${RESET} File Does Not Exist"
    ((PASSED++))
else
    echo -e "${RED}[FAIL]${RESET} File Does Not Exist (Program did not exit 1)"
    ((FAILED++))
fi

# ------------------------------------------------------------------------------
# SUMMARY
# ------------------------------------------------------------------------------
echo -e "\n${BOLD}${CYAN}==============================================${RESET}"
if [ $FAILED -eq 0 ]; then
    echo -e "${BOLD}${GREEN}  ALL $PASSED TESTS PASSED! AMAZING KMP!${RESET}"
else
    echo -e "${BOLD}${RED}  RESULTS: $PASSED PASSED, $FAILED FAILED.${RESET}"
fi
echo -e "${BOLD}${CYAN}==============================================${RESET}\n"
