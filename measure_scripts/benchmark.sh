#!/bin/bash

# This shell script was entirely writen by Gemini Generative-AI Chat-Bot

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

TARGET="./huffman"
TEST_DIR="test_files"
TMP_DIR="benchmark_tmp"

if [ ! -f "$TARGET" ]; then
    echo "Binary $TARGET not found. Compiling the project..."
    make clean && make
fi

mkdir -p "$TMP_DIR"

echo "========================================================================================="
printf "%-30s | %-12s | %-12s | %-10s | %-8s\n" "File" "Original" "Compressed" "% of Compressiom" "Integrity"
echo "========================================================================================="

for input in "$TEST_DIR"/*; do
    [ -f "$input" ] || continue

    filename=$(basename -- "$input")
    compressed="$TMP_DIR/${filename}.huff"
    restored="$TMP_DIR/${filename}.restored"

    orig_size=$(wc -c < "$input" | tr -d ' ')

    $TARGET -c "$input" "$compressed"
    comp_size=$(wc -c < "$compressed" | tr -d ' ')

    $TARGET -d "$compressed" "$restored"

    if cmp -s "$input" "$restored"; then
        status="${GREEN}OK${NC}"
    else
        status="${RED}FAIL${NC}"
    fi

    if [ "$orig_size" -gt 0 ]; then
        ratio=$(awk "BEGIN {printf \"%.2f\", (1 - ($comp_size / $orig_size)) * 100}")
    else
        ratio="0.00"
    fi

    printf "%-30s | %-12s | %-12s | %-9s%% | " "$filename" "$orig_size B" "$comp_size B" "$ratio"
    echo -e "$status"
done

echo "========================================================================================="
rm -rf "$TMP_DIR"