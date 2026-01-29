#!/usr/bin/env python3
"""
Makefile Generator - Automatically generates Makefiles from Makefile.in template
Detects source files, configures paths, and creates ready-to-use Makefiles
"""

import os
import sys
import argparse
from pathlib import Path
from typing import List, Dict, Optional

# ANSI colors
RESET = '\033[0m'
BOLD = '\033[1m'
DIM = '\033[2m'
GREEN = '\033[92m'
CYAN = '\033[96m'
YELLOW = '\033[93m'
RED = '\033[91m'

def find_source_files(directory: Path, recursive: bool = False) -> List[str]:
    """Find all C/C++ source files in directory"""
    extensions = ('.cpp', '.cc', '.cxx', '.c')
    sources = []

    if recursive:
        for ext in extensions:
            sources.extend([str(p.relative_to(directory)) for p in directory.rglob(f'*{ext}')])
    else:
        for ext in extensions:
            sources.extend([str(p.relative_to(directory)) for p in directory.glob(f'*{ext}')])

    # Filter out test files
    sources = [s for s in sources if not s.startswith('tests/')]

    return sorted(sources)

def find_relative_path(from_path: Path, to_path: Path) -> str:
    """Find relative path from one directory to another"""
    try:
        return os.path.relpath(to_path, from_path)
    except ValueError:
        # If on different drives (Windows), return absolute path
        return str(to_path)

def detect_gtest_dir(start_dir: Path) -> Optional[Path]:
    """Try to find Google Test directory by searching upwards"""
    current = start_dir.resolve()

    # Common gtest locations relative to project
    candidates = [
        'vendor/gtest',
        '../vendor/gtest',
        '../../vendor/gtest',
        '../../../vendor/gtest',
        'googletest',
        '../googletest',
    ]

    for candidate in candidates:
        test_path = (current / candidate).resolve()
        if test_path.exists() and test_path.is_dir():
            # Check if it looks like gtest
            if (test_path / 'include' / 'gtest').exists() or \
               (test_path / 'googletest' / 'include' / 'gtest').exists():
                return test_path

    return None

def detect_checker_script(start_dir: Path) -> Optional[Path]:
    """Try to find checker script by searching upwards"""
    current = start_dir.resolve()

    candidates = [
        'vendor/scripts/checker.py',
        '../vendor/scripts/checker.py',
        '../../vendor/scripts/checker.py',
        '../../../vendor/scripts/checker.py',
        'scripts/checker.py',
        '../scripts/checker.py',
    ]

    for candidate in candidates:
        test_path = (current / candidate).resolve()
        if test_path.exists() and test_path.is_file():
            return test_path

    return None

def detect_libcpp_dir(start_dir: Path) -> Optional[Path]:
    """Try to find libcpp directory"""
    current = start_dir.resolve()

    candidates = [
        'libcpp',
        '../libcpp',
        '../../libcpp',
        '../../../libcpp',
    ]

    for candidate in candidates:
        test_path = (current / candidate).resolve()
        if test_path.exists() and test_path.is_dir():
            return test_path

    return None

def find_test_sources(directory: Path) -> List[str]:
    """Find all test source files in tests/ subdirectory"""
    test_dir = directory / 'tests'
    if not test_dir.exists():
        return []
    extensions = ('.cpp', '.cc', '.cxx', '.c')
    sources = []
    for ext in extensions:
        sources.extend([str(p.relative_to(directory)) for p in test_dir.glob(f'*{ext}')])
    return sorted(sources)

def generate_makefile(
    target_dir: Path,
    template_path: Path,
    output_name: str = "Makefile",
    config: Optional[Dict] = None
) -> bool:
    """Generate Makefile from template with auto-detected configuration"""

    if not template_path.exists():
        print(f"{RED}Error: Template not found: {template_path}{RESET}")
        return False

    # Read template
    with open(template_path, 'r') as f:
        template = f.read()

    # Default configuration
    default_config = {
        'CXX': 'c++',
        'CXXFLAGS': '-Wall -Wextra -Werror',
        'INC': '',
        'LDFLAGS': '',
        'LIB_DIR': '',
        'LIBS': '',
        'SRC': '',
        'TARGET': 'program',
        'GTEST_DIR': '../../vendor/gtest',
        'CHECKER': '/path/to/checker.py',
    }

    # Merge with provided config
    if config:
        default_config.update(config)

    # Auto-detect source files if not provided
    if not default_config['SRC']:
        sources = find_source_files(target_dir)
        if sources:
            default_config['SRC'] = ' '.join(sources)
            print(f"  {DIM}detected$(RESET)     {CYAN}{len(sources)} source file(s){RESET}")
        else:
            print(f"  {YELLOW}warning$(RESET)      No source files found in {target_dir}")

    # Auto-detect test sources
    test_sources = find_test_sources(target_dir)
    default_config['TEST_SRCS'] = ' '.join(test_sources)
    default_config['TEST_BINS'] = ' '.join([
        f'$(BIN_DIR)/{Path(s).stem}' for s in test_sources
    ])
    default_config['TEST_OBJS'] = ' '.join([
        f'$(OBJ_DIR)/tests/{Path(s).stem}.o' for s in test_sources
    ])

    # Auto-detect target name from directory if not provided
    if default_config['TARGET'] == 'program':
        dir_name = target_dir.name
        if dir_name and dir_name != '.':
            default_config['TARGET'] = dir_name
            print(f"  {DIM}target$(RESET)       {CYAN}{dir_name}{RESET}")

    # Auto-detect Google Test
    if default_config['GTEST_DIR'] == '../../vendor/gtest':
        gtest_dir = detect_gtest_dir(target_dir)
        if gtest_dir:
            rel_path = find_relative_path(target_dir, gtest_dir)
            default_config['GTEST_DIR'] = rel_path
            print(f"  {DIM}gtest$(RESET)        {CYAN}{rel_path}{RESET}")

    # Auto-detect checker script
    if default_config['CHECKER'] == '/path/to/checker.py':
        checker = detect_checker_script(target_dir)
        if checker:
            rel_path = find_relative_path(target_dir, checker)
            default_config['CHECKER'] = rel_path
            print(f"  {DIM}checker$(RESET)      {CYAN}{rel_path}{RESET}")

    # Auto-detect libcpp
    if not default_config['LIB_DIR']:
        libcpp = detect_libcpp_dir(target_dir)
        if libcpp:
            rel_path = find_relative_path(target_dir, libcpp)
            default_config['LIB_DIR'] = rel_path
            print(f"  {DIM}libcpp{RESET}       {CYAN}{rel_path}{RESET} {DIM}(use LCPP=1 to enable){RESET}")

    # Replace placeholders in template
    result = template
    for key, value in default_config.items():
        # support multiple placeholder styles in Makefile.in templates:
        # $(KEY), ${KEY}, and $KEY
        result = result.replace(f'$({key})', str(value))
        result = result.replace('${' + key + '}', str(value))
        result = result.replace(f'${key}', str(value))

    # Write output
    output_path = target_dir / output_name
    with open(output_path, 'w') as f:
        f.write(result)

    print(f"\n  {GREEN}●${RESET} {BOLD}Makefile generated{RESET} {DIM}→ {output_path}{RESET}\n")
    return True

def main():
    parser = argparse.ArgumentParser(
        description='Generate Makefiles from template with auto-detection',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  # Generate Makefile in current directory
  %(prog)s

  # Generate in specific directory
  %(prog)s /path/to/project

  # Generate with custom target name
  %(prog)s --target my_program

  # Generate with custom source files
  %(prog)s --src "main.cpp utils.cpp"

  # Generate with custom compiler flags
  %(prog)s --cxxflags "-Wall -Wextra -O2"
        '''
    )

    parser.add_argument(
        'directory',
        nargs='?',
        default='.',
        help='Target directory (default: current directory)'
    )

    parser.add_argument(
        '--template',
        default=None,
        help='Path to Makefile.in template (default: auto-detect)'
    )

    parser.add_argument(
        '--output',
        default='Makefile',
        help='Output filename (default: Makefile)'
    )

    parser.add_argument(
        '--target',
        help='Target executable name (default: directory name)'
    )

    parser.add_argument(
        '--src',
        help='Source files (default: auto-detect *.cpp, *.c)'
    )

    parser.add_argument(
        '--cxx',
        default='c++',
        help='C++ compiler (default: c++)'
    )

    parser.add_argument(
        '--cxxflags',
        default='-Wall -Wextra -Werror',
        help='Compiler flags (default: -Wall -Wextra -Werror)'
    )

    parser.add_argument(
        '--gtest-dir',
        help='Google Test directory (default: auto-detect)'
    )

    parser.add_argument(
        '--checker',
        help='Checker script path (default: auto-detect)'
    )

    parser.add_argument(
        '--libcpp-dir',
        help='libcpp directory (default: auto-detect)'
    )

    parser.add_argument(
        '--recursive',
        action='store_true',
        help='Recursively find source files in subdirectories'
    )

    args = parser.parse_args()

    # Resolve paths
    target_dir = Path(args.directory).resolve()
    if not target_dir.exists():
        print(f"{RED}Error: Directory not found: {target_dir}{RESET}")
        return 1

    # Find template
    if args.template:
        template_path = Path(args.template).resolve()
    else:
        # Look for Makefile.in in common locations
        candidates = [
            Path(__file__).parent / 'Makefile.in',
            Path.cwd() / 'Makefile.in',
            target_dir / 'Makefile.in',
            target_dir.parent / 'Makefile.in',
        ]
        template_path = None
        for candidate in candidates:
            if candidate.exists():
                template_path = candidate
                break

        if not template_path:
            print(f"{RED}Error: Could not find Makefile.in template{RESET}")
            print(f"{DIM}Searched in:{RESET}")
            for c in candidates:
                print(f"  - {c}")
            return 1

    # Build configuration
    config = {
        'CXX': args.cxx,
        'CXXFLAGS': args.cxxflags,
    }

    if args.target:
        config['TARGET'] = args.target

    if args.src:
        config['SRC'] = args.src
    elif args.recursive:
        sources = find_source_files(target_dir, recursive=True)
        if sources:
            config['SRC'] = ' '.join(sources)

    if args.gtest_dir:
        config['GTEST_DIR'] = args.gtest_dir

    if args.checker:
        config['CHECKER'] = args.checker

    if args.libcpp_dir:
        config['LIB_DIR'] = args.libcpp_dir

    # Print header
    print(f"\n{BOLD}{CYAN}Makefile Generator{RESET}")
    print(f"{DIM}{'─' * 40}{RESET}\n")
    print(f"  {DIM}template${RESET}     {CYAN}{template_path}{RESET}")
    print(f"  {DIM}directory${RESET}    {CYAN}{target_dir}{RESET}")

    # Generate Makefile
    success = generate_makefile(target_dir, template_path, args.output, config)

    return 0 if success else 1

if __name__ == '__main__':
    sys.exit(main())
