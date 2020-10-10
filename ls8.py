#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

if len(sys.argv) != 2:
    print("Improper Useage: ==> python ls8.py programName.py")
else:
    cpu = CPU()
    cpu.load(sys.argv[1])
    cpu.run()