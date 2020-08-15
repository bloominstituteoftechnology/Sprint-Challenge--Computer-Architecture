# parser.py

import os
import re

class Parser():
    
    def __init__(self, source, bin_or_int='int'):
        self.bin_or_int = bin_or_int
        self.source = self.load(source)
        

    def load(self, source:str):

        def _remove_comments(line):
            if line[0] == '#':
                return None 
            else:
                return line.split('#')[0]

        def _snip_instruction(line):
            pat = '^\d*\d'
            try:
                return re.findall(pat, line)[0]
            except:
                return None

        def _convert_to_binary(line):
            return bin(int(f'0b{line}', 2))

        raw = []
        with open(source, 'r') as f:
            for line in f:
                raw.append(line)

        self.raw = raw

        processed = [_remove_comments(line) for line in raw]
        processed = [_snip_instruction(line) for line in processed]
        processed = [line for line in processed if line is not None]
        if self.bin_or_int == 'int':
            processed = [int(line, 2) for line in processed]
        else:
            processed = [_convert_to_binary(line) for line in processed]

        self.processed = processed