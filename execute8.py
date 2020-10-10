#!/usr/bin/env python3

"""Main execution logic"""

# Imports
import sys
from cpu import *

# Set a default input file
fname = "sctest.ls8"
# Grab the command line arguments
args  = sys.argv
# Validate the line arguments
if len(args) > 1:
    # grab the filename from the arguments
    fname = args[1]

# Instantiate a CPU processing class
cpu = CPU()

cpu.read_file(fname)    # read instruction data from a file into the class
cpu.load()              # load the read intructions for processing
cpu.run()               # execute the loaded program instructions
