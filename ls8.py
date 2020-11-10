"""Main."""

import sys
from cpu import *

cpu = CPU()

sys.argv.append(
    r'/Users/chiragthesia/Desktop/CS/Sprint-Challenge--Computer-Architecture/sctest.ls8')

if len(sys.argv) > 1:
    cpu.load(sys.argv[1])
    cpu.run()
else:
    print("please add a program to run")
