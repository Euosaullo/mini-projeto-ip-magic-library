#!/usr/bin/env bash
set -euo pipefail

output="biblioteca"

case "$(uname -s)" in
    MINGW*|MSYS*|CYGWIN*)
        output="biblioteca.exe"
        ;;
esac

gcc -Wall -Wextra -Wpedantic -std=c11 \
    main.c \
    library.c \
    files.c \
    encryption.c \
    utils.c \
    -o "$output"

echo "Build finished: ./$output"
