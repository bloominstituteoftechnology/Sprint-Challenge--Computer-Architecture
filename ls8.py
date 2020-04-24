"""Main."""

import sys
from cpu import *

def main():
    if len(sys.argv) != 2:
        print('Usage: ls8.py [filename.ls8]')
        sys.exit(1)
    program = sys.argv[1]
    
    cpu = CPU()
    cpu.load(program)
    cpu.run()

if __name__ == "__main__":
    main()