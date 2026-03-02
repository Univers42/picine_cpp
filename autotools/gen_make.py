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

def find_source_files(directory: Path, recursive: bool = True) -> List[str]:
    """Find all C/C++ source files in directory, excluding tests/ and build/ directories"""
    extensions = ('.cpp', '.cc', '.cxx', '.c')
    sources = []

    if recursive:
        for ext in extensions:
            sources.extend([str(p.relative_to(directory)) for p in directory.rglob(f'*{ext}')])
    else:
        for ext in extensions:
            sources.extend([str(p.relative_to(directory)) for p in directory.glob(f'*{ext}')])

    # Exclude any file that lives under a 'tests' or 'build' directory at any depth
    excluded_dirs = {'tests', 'build'}
    sources = [s for s in sources if not any(part in excluded_dirs for part in Path(s).parts)]

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
        'CXXFLAGS': '-std=c++98 -Wall -Wextra -Werror',
        'INC': '-I.', # Default include path
        'LDFLAGS': '',
        'LIB_DIR': '',
        'LIBS': '',
        'SRC': '',
        'TARGET': 'program',
        'GTEST_DIR': '../../vendor/gtest',
        'LINTER_SCRIPT': 'norminette.sh', # Fallback default
    }

    # Merge with provided config
    if config:
        default_config.update(config)

    # Auto-detect target name from directory if not provided
    if default_config['TARGET'] == 'program':
        dir_name = target_dir.name
        if dir_name and dir_name != '.':
            default_config['TARGET'] = dir_name
            print(f"  {DIM}target{RESET}       {CYAN}{dir_name}{RESET}")

    # ─── CRITICAL PATH MATH FOR LINTER ────────────────────────────────────────
    # 1. Get the absolute path of the directory containing this gen_make.py script
    autotools_dir = Path(__file__).resolve().parent

    # 2. Look for norminette.sh right next to it
    linter_path = autotools_dir / 'norminette.sh'

    if linter_path.exists():
        # 3. Mathematically calculate the relative path from the target_dir to the linter_path
        rel_linter = os.path.relpath(linter_path, target_dir)
        default_config['LINTER_SCRIPT'] = rel_linter
        print(f"  {DIM}linter{RESET}       {CYAN}{rel_linter}{RESET}")
    # ──────────────────────────────────────────────────────────────────────────

    # Replace @@KEY@@ placeholders in template (not $(KEY) which are Make variables)
    result = template
    for key, value in default_config.items():
        result = result.replace(f'@@{key}@@', str(value))

    # Write output
    output_path = target_dir / output_name
    with open(output_path, 'w') as f:
        f.write(result)

    print(f"\n  {GREEN}●{RESET} {BOLD}Makefile generated{RESET} {DIM}→ {output_path}{RESET}\n")
    return True

def main():
    parser = argparse.ArgumentParser(
        description='Generate Makefiles from template with auto-detection'
    )

    parser.add_argument('directory', nargs='?', default='.', help='Target directory')
    parser.add_argument('--template', default=None, help='Path to Makefile.in template')
    parser.add_argument('--output', default='Makefile', help='Output filename')
    parser.add_argument('--target', help='Target executable name')
    parser.add_argument('--src', help='Source files')
    parser.add_argument('--cxx', default='c++', help='C++ compiler')
    parser.add_argument('--cxxflags', default='-std=c++98 -Wall -Wextra -Werror', help='Compiler flags')
    parser.add_argument('--gtest-dir', help='Google Test directory')
    parser.add_argument('--libcpp-dir', help='libcpp directory')
    parser.add_argument('--recursive', action='store_true', default=True,
                        help='Recursively find source files (default: True)')
    parser.add_argument('--no-recursive', action='store_false', dest='recursive',
                        help='Only find source files in top-level directory')

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
            return 1

    # Build configuration
    config = {
        'CXX': args.cxx,
        'CXXFLAGS': args.cxxflags,
    }

    if args.target:
        config['TARGET'] = args.target

    # ─── THE FIX: AUTO-DETECT SOURCES EVERY TIME ──────────────────────────────
    if args.src:
        config['SRC'] = args.src
    else:
        sources = find_source_files(target_dir, recursive=args.recursive)
        if sources:
            config['SRC'] = ' '.join(sources)
            print(f"  {DIM}sources{RESET}      {CYAN}{len(sources)} file(s) auto-detected{RESET}")
        else:
            print(f"  {YELLOW}⚠ No source files (*.cpp) found in {target_dir}{RESET}")
            config['SRC'] = ''
    # ──────────────────────────────────────────────────────────────────────────

    if args.gtest_dir:
        config['GTEST_DIR'] = args.gtest_dir

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
