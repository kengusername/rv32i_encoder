#!/bin/bash
set -e

for t in tests/*; do
    n=${t##*/}
    [ -f "$t/$n.cc" ] || continue

    g++ -std=c++26 -Iinclude "$t/$n.cc" -o a.out
    ./a.out > actual.txt

    riscv32-unknown-linux-gnu-as "$t/$n.s" -o a.out
    riscv32-unknown-linux-gnu-objcopy -O binary -j .text a.out /dev/stdout | od -An -tx4 -w4 -v | sed 's/^ *0*//' > expected.txt

    if diff -u expected.txt actual.txt; then
        printf '[%s] \033[32mPASS\033[0m\n' "$n"
    else
        printf '[%s] \033[31mFAIL\033[0m\n' "$n"
        rm -f a.out expected.txt actual.txt
        exit 1
    fi

    rm -f a.out expected.txt actual.txt
done

