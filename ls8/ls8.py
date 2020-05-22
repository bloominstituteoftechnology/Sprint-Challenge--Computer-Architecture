#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

cpu = CPU()
if len(sys.argv) <= 1:
    print('You must provide a filename')
else:

    cpu.load(sys.argv[1])
    cpu.run()