# components.py

from instructions import InstructionSet


class Dispatcher():
    """
    Dispatcher

    Call components to fetch data and dispatch reads, commands, to components.
    """
    def __init__(self, alu, pc):
        super().__init__()
        self.alu = alu 
        self.pc = pc

    def parse_code(self, code):
        print('got code: ', code)
        op = code >> 6
        alu = (code >> 5) & 1
        pc = (code >> 4) & 1

        data = (None)
        if pc == 1:
            data = self.pc(code)

        if alu == 1:
            result = self.alu(code, *data)
            


        print('ops: ', op, ' alu: ', alu, ' pc: ', pc)





    def __call__(self, code):
        self.parse_code(code)




class Cursor():
    def __init__(self):
        self.value = 0

    def __add__(self, x):
        self.value += x

    def __call__(self):
        pos = self.value
        self.value += 1
        return pos



class ALU():
    """
    ALU

    Contains setters for ALU and execution control for ALU instructions.
    """
    def __init__(self, instructionset):
        self.E = 0
        self.L = 0
        self.G = 0
        self.instructionset = instructionset

    def __call__(self, code, *args, **kwargs):
        fn = self.instructionset(code)
        return fn(self, *args, **kwargs)





class PC():
    """
    PC

    Cursor control.  Handles data fetch and set
    """
    def __init__(self, instructionset, ram=None, reg=None):
        self.instructionset = instructionset
        self.c = Cursor()
        self.ram = ram
        self.reg = reg


    def __call__(self, code):
        fn = self.instructionset(code)
        return fn(self, *args, **kwargs)

    def ram_read(self):
        if self.c.value < len(self.ram):
            data = self.ram[self.c()]
            return data
        return None


    def ram_write(self, idx, val):
        self.ram[int(idx)] = val

    def reg_read(self, idx):
        return self.register[int(idx)]
    
    def reg_write(self, idx, val):
        self.register[int(idx)] = val
    
