#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

if len(sys.argv) != 2:
    print(argv_err_msg)
    sys.exit(1)
else:
    file_name = sys.argv[1]

cpu = CPU()
cpu.load(file_name)
cpu.run()