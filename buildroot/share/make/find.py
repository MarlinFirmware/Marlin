#!/usr/bin/env python3
# This is a cut-down version of the Linux 'find' command, implemented in Python for Marlin build scripts.
import sys
import os
import fnmatch

def find_files(root, mindepth, pattern):
    results = []
    root = os.path.normpath(root)
    for dirpath, dirnames, filenames in os.walk(root):
        for filename in filenames:
            if fnmatch.fnmatch(filename, pattern):
                full_path = os.path.join(dirpath, filename)
                rel_path = os.path.relpath(full_path, root)
                # mindepth: number of path components in rel_path >= mindepth
                if rel_path.count(os.sep) + 1 >= mindepth:
                    results.append(full_path)
    return results

if __name__ == "__main__":
    if len(sys.argv) != 6 or sys.argv[2] != '-mindepth' or sys.argv[4] != '-name':
        print("Usage: find.py <dir> -mindepth <num> -name <pattern>", file=sys.stderr)
        sys.exit(1)
    root = sys.argv[1]
    mindepth = int(sys.argv[3])
    pattern = sys.argv[5]
    files = find_files(root, mindepth, pattern)
    for f in files:
        print(f.replace('\\', '/'))
