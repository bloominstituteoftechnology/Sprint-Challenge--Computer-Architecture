#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

if not sys.argv[1]:
    print("Please run with format 'python ls8.py [file]")

program = sys.argv[1]

cpu = CPU()

cpu.load(program)

cpu.run()