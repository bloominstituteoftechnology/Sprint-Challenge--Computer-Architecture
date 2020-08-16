#!/usr/bin/env python3

"""Main."""

import sys
import argparse
from cpu import *
import os


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument('program_path')     

    args = parser.parse_args()

    cpu = CPU()

    path = os.path.join(os.getcwd(), args.program_path)

    cpu.load(program=path)
    cpu.run()