"""CPU functionality."""

import sys
from instructions import InstructionSet
from parser import Parser

class Cursor():
    value = 0

    def __add__(self, x):
        self.value += x


class CPU:
    """Main CPU class."""

    def __init__(self, msize=256, rsize=8, instructionset=InstructionSet(), rec_max=100):
        """Construct a new CPU."""
        self.ram = self._seed_list(msize)
        self.reg = self._seed_list(rsize)
        self.instructionset = instructionset
        self.rec_max = rec_max
        self.c1 = Cursor()

    @property 
    def pc(self):
        return self.c1.value
    

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
            # print(p.raw)
            # print('\n Processed \n', p.processed)
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
            self.ram[address] = instruction
            address += 1


    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            #self.fl,
            #self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()


    def ram_read(self, idx):
        return self.ram[int(idx)]


    def ram_write(self, idx, val):
        self.ram[int(idx)] = val


    def run(self):
        """Run the CPU."""
        def _run_instruction(instruction, interrupt):
            if not interrupt:
                method = self.instructionset(instruction)
                signal = method(self.ram, self.reg, self.c1)
                return signal
            return True
        
        def _check_interrupt(signal):
            return signal is not None

        def _check_rec(rec):
            return rec > self.rec_max

        rec = 0

        interrupt = False
        while not interrupt:
            # print(self.ram, self.reg, self.c1, rec)
            interrupt = _check_rec(rec)
            interrupt = _check_interrupt(
                _run_instruction(self.ram[self.pc], interrupt)
                )
            rec += 1

        print('Program Halted Successfully.', rec)