#!/bin/bash

# Check if the user passed in a file name
if [ $# -eq 0 ]; then
  echo "Usage: $0 <filename>"
  exit 1
fi

if [ ! -f "$1" ]; then
  echo "file does not exist: $1"
  exit 1
fi

# Compile the file and count the errors and warnings
compile=$(gcc -o /dev/null -Wall -Wextra -pedantic-errors "$1" 2>&1)
no_errors=$(echo "$compile" | grep -c "error:")
no_warnings=$(echo "$compile" | grep -c "warning:")

# Print the results
echo "$no_errors $no_warnings"

# Exit successfully
exit 0