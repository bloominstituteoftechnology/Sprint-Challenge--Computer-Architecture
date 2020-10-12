#!/usr/bin/env python3

import sys
from cpu import *


   
if len(sys.argv) != 2:
    print(f"usage: {sys.argv[0]} filename", file = sys.stderr)
    
cpu = CPU()
cpu.load(sys.argv[1])
cpu.run()