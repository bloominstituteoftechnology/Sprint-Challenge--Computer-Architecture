#!/usr/bin/env python3

"""Main."""

import sys
from mattLS8 import *

cpu = CPU()
cpu.load("sctest.ls8") #fileNameFromCommandLine
cpu.run()
