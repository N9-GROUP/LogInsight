#!/bin/bash

find . -type f -name "*.c" | while read -r file; do
    clang-format -i "$file"
    🔥 echo "Formatted $file"
done

echo "🚀 Testing..."

find . -type f -name "*.c" | while read -r file; do
    echo "🔎 Testing: $file"
    clang --analyze "$file"
done
