#!/bin/bash

echo -e "\n\n\033[0;34m 🚀 Formatting...\n"

find . -type f -name "*.cpp" | while read -r file; do
    clang-format -i "$file"
    echo -e "\033[0;32m 🔥 Formatted $file"
done
