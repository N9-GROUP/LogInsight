#!/bin/bash

echo -e "\n\n\033[0;34m 🚀 Testing...\n"

find . -type f -name "*.cpp" | while read -r file; do
    echo -e "\033[0;32m 🔎 Testing: $file"
    clang++ --analyze "$file"
done
