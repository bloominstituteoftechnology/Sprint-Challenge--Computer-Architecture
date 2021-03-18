#!/usr/bin/env python3

"""Main."""

import sys
from cpu import CPU

if len(sys.argv) !=2:
    print(f"{sys.argv[0]} <file name>")
    sys.exit(1)

cpu = CPU()

cpu.load(sys.argv[1])
cpu.run()

if __name__ == "__main__":
    pass
