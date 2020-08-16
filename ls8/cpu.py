"""CPU functionality."""

import sys
from instructions import InstructionSet
from components import PC, ALU, Dispatcher
from parser import Parser

class CPU:
    """Main CPU class."""

    def __init__(self, msize=256, rsize=8, instructionset=InstructionSet(), rec_max=100):
        """Construct a new CPU."""
        
        self.rec_max = rec_max

        self.pc = PC(
            instructionset=instructionset, 
            ram=self._seed_list(msize), 
            reg=self._seed_list(rsize),
        )

        self.alu = ALU(
            instructionset=instructionset,
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

        # Get next item at cursor position and advance cursor
        for _ in range(10):
            code = self.pc.ram_read()
            self.dispatch(code)



        # def _run_instruction(instruction, interrupt):  # TODO: Move runto component unit
        #     if not interrupt:
        #         method = self.instructionset(instruction)
        #         signal = method(self.ram, self.reg, self.c1)
        #         return signal
        #     return True
        
        # def _check_interrupt(signal):
        #     return signal is not None

        # def _check_rec(rec):
        #     return rec > self.rec_max

        # rec = 0

        # interrupt = False
        # while not interrupt:
        #     # print(self.ram, self.reg, self.c1, rec)
        #     interrupt = _check_rec(rec)
        #     interrupt = _check_interrupt(
        #         _run_instruction(self.ram[self.pc], interrupt)
        #         )
        #     rec += 1

        print('Program Halted Successfully.')#, rec)