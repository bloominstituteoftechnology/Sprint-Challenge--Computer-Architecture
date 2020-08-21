#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

command = sys.argv[1]

cpu = CPU()

cpu.load(command)
cpu.run()