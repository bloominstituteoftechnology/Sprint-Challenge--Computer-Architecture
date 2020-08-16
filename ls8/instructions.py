# instructions.py


# Common instructions

def halt(*args):
    return True


def ldi(*args):
    m, r, c = args
    cp = c.value
    i = m[cp+1]
    r[i] = m[cp+2]
    c += 3


def prn(*args):
    m, r, c = args
    cp = c.value
    i = m[cp+1]
    print(r[i])
    c += 2


def mul(*args):
    m, r, c = args
    cp = c.value
    i1 = m[cp+1]
    i2 = m[cp+2]
    r[i1] = r[i1] * r[i2]
    c += 3


def cmp(*args):
    pass 


def equal(*args):
    pass


def jmp(*args):
    pass 


def jeq(*args):
    pass


def jne(*args):
    pass


# Instruction Set Class

class InstructionSet():
    def __init__(self, bin_or_int='int'):
        super().__init__()

        instructions_set = (
                ('10000010', ldi), # LDI
                ('01000111', prn), # PRN
                ('10100010', mul), # MUL
                ('00000001', halt), # HLT
        )

        self.bin_or_int = bin_or_int
        self.instructions = self._build_instructions(instructions_set)

        
    def _sbin_to_int(self, line):
        return int(line, 2)


    def _convert_to_binary(self, line):
        return bin(int(f'0b{line}', 2))


    def _build_instructions(self, iset):
        instructions = {}
        if self.bin_or_int == 'int':
            fn = self._sbin_to_int
        else:
            fn = self._convert_to_binary
        
        for ins in iset:
            n = fn(ins[0])
            instructions[n] = ins[1]
        return instructions


    def _parse_op(self, code):
        return self._sbin_to_int(code) >> 6



    def __call__(self, code):
        try:
            return self.instructions[code]
        except KeyError:
            raise NotImplementedError(f'Code {c} not defined')
        except:
            raise
            