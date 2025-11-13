#!/bin/sh

# Usage: ./env.sh [directory]
# Default directory is current directory
DIR="${1:-.}"

export CXX=c++
export CXXFLAGS="-Wall -Wextra -Werror"

# Load variables from .env if it exists
if [ -f ./autotools/.env ]; then
    set -a
    . ./autotools/.env
    set +a
    echo "Loaded variables from .env"
fi

# Find all .c and .cpp files in the specified directory, print only the filename (no path)
export SRC="$(find "$DIR" -type f \( -name '*.c' -o -name '*.cpp' \) -exec basename {} \; | tr '\n' ' ')"
# ...

echo "Detected source files: $SRC"

# Generate Makefile from Makefile.in
envsubst < Makefile.in > "$DIR"/Makefile
echo "Generated autotools/Makefile"