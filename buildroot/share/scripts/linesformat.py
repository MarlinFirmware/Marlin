#!/usr/bin/env python3
"""
Formatter script for *.h, *.c, *.cpp, *.md, *.txt, and other source files

usage: linesformat.py [infile] [outfile]

With no parameters convert STDIN to STDOUT
"""

import sys, re, argparse

do_log = False
def logmsg(msg, line):
    if do_log: print(msg, line)

def format_text(argv):
    parser = argparse.ArgumentParser(description="Formatter script for source files")
    parser.add_argument('infile', nargs='?', default=None, help="Input file to read from. If not provided, reads from stdin.")
    parser.add_argument('outfile', nargs='?', default=None, help="Output file to write to. If not provided, writes to stdout.")
    parser.add_argument('-v', action='store_true', help="Enable logging.")

    args = parser.parse_args(argv)

    src_file = args.infile or 'stdin'
    dst_file = args.outfile or None

    scnt = 0
    for arg in argv:
        if arg == '-v':
            global do_log
            do_log = args.v or True
        elif scnt == 0:
            # Get a source file if specified. Default destination is the same file
            src_file = dst_file = arg
            scnt += 1
        elif scnt == 1:
            # Get destination file if specified
            dst_file = arg
            scnt += 1

    # No text to process yet
    file_text = ''

    if src_file == 'stdin':
        # If no source file specified read from STDIN
        file_text = sys.stdin.read()
    else:
        # Directory will be processed recursively with editorconfig
        if os.path.isdir(src_file):
            apply_editorconfig_rules(src_file)
            return

        # Open and read the file src_file
        with open(src_file, 'r', encoding='utf-8') as rf:
            file_text = rf.read()

    if len(file_text) == 0:
        print('No text to process')
        return

    # Read from file or STDIN until it terminates
    filtered = re.sub(r'\s+$', '', file_text) + '\n'
    if dst_file:
        with open(dst_file, 'w', encoding='utf-8', newline='') as wf:
            wf.write(filtered)
    else:
        print(filtered)

# Python standard startup for command line with arguments
if __name__ == '__main__':
    format_text(sys.argv[1:])

# Usage
apply_editorconfig_rules('/path/to/your/folder')
