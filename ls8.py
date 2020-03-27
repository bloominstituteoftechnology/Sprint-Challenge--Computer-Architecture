#!/usr/bin/env python3

"""Main."""

import sys
from cpu import *

cpu = CPU()

cpu.load(sys.argv[1])
cpu.run()

# def main(argv):
#     """Main."""

#     if len(argv) != 2:
#         print(f"usage: {argv[0]} filename", file=sys.stderr)
#         return 1

#     cpu = CPU()

#     cpu.load(argv[1])
#     cpu.run()

#     return 0

# if __name__ == "__main__":
#     sys.exit(main(sys.argv))