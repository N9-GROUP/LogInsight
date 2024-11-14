#!/bin/bash

find . -type f -name "*.c" | while read -r file; do
    clang-format -i "$file"
    echo "Formatted $file"
done
