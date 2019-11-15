#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

cpu = CPU()
# cpu.load()


if len(sys.argv) != 2:
    print("usage: 02-fileio02.py <filename>")
    sys.exit(1)

cpu.load(sys.argv[1])
print(cpu.ram)

cpu.run()