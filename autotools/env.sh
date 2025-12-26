#!/bin/sh
# Determine script directory (absolute) so templates are found reliably
script_dir="$(cd "$(dirname "$0")" 2>/dev/null && pwd -P || printf '%s' "$(dirname "$0")")"
TEMPLATE_DIR="${1:-$script_dir}"
TARGET_DIR="${2:-.}"

# If TARGET_DIR looks like a Windows path (contains backslashes or drive:), try converting with wslpath
case "$TARGET_DIR" in
  *\\*|[A-Za-z]:/*)
    if command -v wslpath >/dev/null 2>&1; then
      conv="$(wslpath -u "$TARGET_DIR" 2>/dev/null || true)"
      if [ -n "$conv" ]; then
        TARGET_DIR="$conv"
      fi
    fi
    ;;
esac

# Resolve TARGET_DIR to an absolute path and ensure it exists
if [ -d "$TARGET_DIR" ]; then
    TARGET_DIR="$(cd "$TARGET_DIR" 2>/dev/null && pwd -P || printf '%s' "$TARGET_DIR")"
else
    printf 'Error: target dir "%s" does not exist\n' "$TARGET_DIR" >&2
    exit 1
fi

# Ensure template file exists
if [ ! -f "$TEMPLATE_DIR/Makefile.in" ]; then
    printf 'Error: template "%s/Makefile.in" not found\n' "$TEMPLATE_DIR" >&2
    exit 1
fi

# Base compiler settings
export CXX="${CXX:-c++}"
export CXXFLAGS="${CXXFLAGS:--Wall -Wextra -Werror}"
export LDFLAGS="${LDFLAGS:-}"

# Locate libcpp dir (prefer project relative path)
CAND1="$(pwd)/libcpp"
CAND2="$(pwd)/$(dirname "$TARGET_DIR")/libcpp"
if [ -d "$CAND1" ]; then
    LIBCPP_DIR="$CAND1"
elif [ -d "$CAND2" ]; then
    LIBCPP_DIR="$CAND2"
else
    LIBCPP_DIR="${LIBCPP_DIR:-$(pwd)/libcpp}"
fi
export LIB_DIR="$LIBCPP_DIR"

# Detect include directory inside libcpp (include -> inc -> root)
if [ -d "${LIBCPP_DIR}/include" ]; then
    INC_DIR="${LIBCPP_DIR}/include"
elif [ -d "${LIBCPP_DIR}/inc" ]; then
    INC_DIR="${LIBCPP_DIR}/inc"
else
    # if there are header files at lib root, use root; otherwise still use root as fallback
    if find "${LIBCPP_DIR}" -maxdepth 1 -type f \( -name '*.h' -o -name '*.hpp' \) | grep -q .; then
        INC_DIR="${LIBCPP_DIR}"
    else
        INC_DIR="${LIBCPP_DIR}"
    fi
fi
export INC="-I${INC_DIR}"

# Link against libcpp (shared or static depending on what's available)
export LIBS="${LIBS:--lcpp}"

# Find sources in target dir (only base filenames, as generator expects)
export SRC="$(find "$TARGET_DIR" -maxdepth 1 -type f \( -name '*.c' -o -name '*.cpp' \) -exec basename {} \; | tr '\n' ' ')"

# Optionally set a default TARGET based on directory name if not provided
if [ -z "$TARGET" ]; then
    BASE="$(basename "$TARGET_DIR")"
    export TARGET="${TARGET:-${BASE:-a.out}}"
fi

echo "Template: $TEMPLATE_DIR/Makefile.in -> $TARGET_DIR/Makefile"
echo "Detected libcpp dir: $LIB_DIR"
echo "Detected source files: $SRC"

# Generate Makefile from template
# Protect double-dollar sequences so envsubst doesn't mangle Makefile recipes
tmpl="$(mktemp)"
sed 's/\$\$/@@DOLLAR@@/g' "$TEMPLATE_DIR/Makefile.in" > "$tmpl"
envsubst < "$tmpl" | sed 's/@@DOLLAR@@/$$/g' > "$TARGET_DIR/Makefile"
rm -f "$tmpl"
echo "Generated $TARGET_DIR/Makefile"