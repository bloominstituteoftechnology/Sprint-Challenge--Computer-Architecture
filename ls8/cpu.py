"""CPU functionality."""

import sys
from instructions import PCInstructionSet, ALUInstructionSet
from components import PC, ALU, Dispatcher
from parser import Parser

class CPU:
    """Main CPU class."""

    def __init__(self, msize=256, rsize=8, rec_max=100):
        """Construct a new CPU."""
        
        self.rec_max = rec_max

        self.pc = PC(
            instructionset=PCInstructionSet(), 
            ram=self._seed_list(msize), 
            reg=self._seed_list(rsize),
        )

        self.alu = ALU(
            instructionset=ALUInstructionSet(),
        )

        self.dispatch = Dispatcher(
            alu = self.alu,
            pc = self.pc
        )
    

    def _sbin_to_int(self, line):
        return int(line, 2)


    def _convert_to_binary(self, line):
        return bin(int(f'0b{line}', 2))


    def _seed_list(self, rsize):
        def _ret_zero():
            return 0*0
    
        return [_ret_zero() for _ in range(rsize)]


    def load(self, program=None):
        """Load a program into memory."""
        def _parse_file(path):
            p = Parser(path)
            return p.processed

        address = 0

        if program is None:
            program = [
                0b00000001,
            ]
        elif type(program) == str:
            program = _parse_file(program)
        else:
            raise NotImplementedError('Cannot initialize program')

        for instruction in program:
            self.pc.ram[address] = instruction
            address += 1


    def run(self):
        """Run the CPU."""

        rec = 0
        signal = None
        while signal is None:
            code = self.pc.ram_read()
            signal = self.dispatch(code)
            # print('Received signal', signal)
            if signal:
                break
            rec += 1

        print('Program Halted Successfully.', rec)