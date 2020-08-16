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
        op = code >> 6
        alu = (code >> 5) & 1
        pc = (code >> 4) & 1

        return op, alu, pc


    def fetch_dispatch(self, code):
        op, alu, pc = self.parse_code(code)

        data = self._request_data(op)
        # print('Data read: ', data)

        if pc == 1:
            self.pc(code, *data, alu=self.alu)

        if alu == 1:
            result = self.alu(code, *data, pc=self.pc)

        if (pc == 0) and (alu == 0):
            signal = self.pc(code, *data, alu=self.alu)
            return signal


    def _request_data(self, op):
        data = []
        for _ in range(op):
            data.append(self.pc.ram_read())
        return data


    def __call__(self, code):
        return self.fetch_dispatch(code)





class Cursor():
    def __init__(self):
        self.value = 0


    def __add__(self, x):
        self.value += x


    def jump(self, x):
        self.value = x


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


    def __call__(self, code, *args, **kwargs):
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
        return self.reg[int(idx)]


    def reg_write(self, idx, val):
        self.reg[int(idx)] = val
    
