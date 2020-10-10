#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

# Set a default input file
fname = "./examples/stack.ls8"
# Grab the command line arguments
args  = sys.argv
# Validate the line arguments
fname = "sctest.ls8"
if len(args) > 1:
    # grab the filename from the arguments
    fname = args[1]

cpu = CPU()

cpu.read_file(fname)
cpu.load()
cpu.run()
