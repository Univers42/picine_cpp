# Makefile Auto-Generation System

## Overview
This system allow us to automatically generate professional, modern Makefiles for
C++ projects with:
- Auto-detection of source files
- Google Test Integration
- Modern build structure (build/obj, build/bin/)
- clean, professional output
- code quality checking (norminette)
- Consistent structure across all exercices

## Files
1. `Makefile.in` -Template
The master template with placeholders that get replaced during generation:
- $CXX -Compiler
- $CXXFLAGS -Compiler flags
- $SRC -Source Files
- $Target -Executable name
- $GTEST_DIR -Path to Google test
- $CHECKER -Path to checker script
- $LIB_DIR -Path to libcpp(if using)
- $BUILD_LIBCPP - Whether to build libcpp

2. `generate_makefile.py` - Generator_script
Python script that:
- auto-detects source files (*.cpp, *.c)
- Auto-detects Google test location
- Auto-detects checker script
- Generates target name from directory
- Creates ready-to-use Makefile

# Quick start
## Basic Usage (Auto-detect everything)

```cpp
./gen_make.py

./gen_make.py /path/to/project
```


