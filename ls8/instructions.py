# instructions.py


# Common instructions

def halt(*args, alu):
    return True


def ldi(*args, alu):
    pc, i, val = args
    pc.reg_write(i, val)


def prn(*args, alu):
    pc, i = args
    val = pc.reg_read(i)
    print(val)


def mul(*args, pc):
    alu, i1, i2 = args
    v1 = pc.reg_read(i1)
    v2 = pc.reg_read(i2)
    pc.reg_write(i1, v1 * v2)


def cmp(*args, pc):
    alu, i1, i2 = args
    v1 = pc.reg_read(i1)
    v2 = pc.reg_read(i2)
    alu.E = int(v1 == v2)
    alu.L = int(v1 < v2)
    alu.G = int(v1 > v2)


def jmp(*args, alu):
    pc, i = args
    pc.c.jump(pc.reg_read(i))


def jeq(*args, alu):
    if alu.E:
        jmp(*args, alu=alu)


def jne(*args, alu):
    if not alu.E:
        jmp(*args, alu=alu)


# Instruction Set Classes

class InstructionSet():
    def __init__(self, bin_or_int='int'):
        super().__init__()
        
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
            raise NotImplementedError(f'Code {code} not defined')
        except:
            raise
            


class ALUInstructionSet(InstructionSet):
    def __init__(self, bin_or_int='int'):
        super().__init__(bin_or_int=bin_or_int)
        instructions_set = (
                ('10100010', mul), # MUL
                ('10100111', cmp), # CMP
        )

        self.bin_or_int = bin_or_int
        self.instructions = self._build_instructions(instructions_set)


class PCInstructionSet(InstructionSet):
    def __init__(self, bin_or_int='int'):
        super().__init__(bin_or_int=bin_or_int)
        instructions_set = (
                ('10000010', ldi), # LDI
                ('01000111', prn), # PRN
                ('01010100', jmp), # JMP
                ('01010101', jeq), # JEQ
                ('01010110', jne), # JNE
                ('00000001', halt), # HLT
        )

        self.bin_or_int = bin_or_int
        self.instructions = self._build_instructions(instructions_set)