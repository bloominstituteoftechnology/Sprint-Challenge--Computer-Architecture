#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

cpu = CPU()

if len(sys.argv)==2:
    filename = sys.argv[1]

    cpu.load(filename)
    cpu.run()

else:
    print("ERROR: please include the name of the program you want to run as an argument.")