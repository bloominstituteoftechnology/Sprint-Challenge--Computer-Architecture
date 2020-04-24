#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

program_filename = "./files/" + sys.argv[1]

cpu = CPU()
# print("before loading")
cpu.load(program_filename)
# print(cpu.ram)
cpu.run()