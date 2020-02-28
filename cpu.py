"""CPU fn"""

import sys

class CPU:
    """main CPU class"""

    def __init__(self):
        self.ram = [0]*256
        self.pc = 0
        self.fl = None
        self.reg = [0]*8
        self.stack_pointer = 0xF4
        self.reg[7] = self.ram[self.stack_pointer]
        
        # op dictionary
        self.op_table = {}
        self.op_table[0b10000010] = self.op_ldi
        self.op_table[0b01000111] = self.op_prn
        self.op_table[0b00000001] = self.op_hlt
        self.op_table[0b01000101] = self.op_push
        self.op_table[0b01000110] = self.op_pop
        self.op_table[0b00010001] = self.op_ret
        self.op_table[0b01010000] = self.op_call
        self.op_table[0b01010100] = self.op_jmp
        self.op_table[0b01010101] = self.op_jeq
        self.op_table[0b01010110] = self.op_jne

    def ram_read(self, address):
        return self.ram[address]

    def ram_write(self, address, value):
        self.ram[address] = value

    def op_ldi(self, operand_a, operand_b):
        self.reg[operand_a] = operand_b
        self.pc += 3 
    
    def op_prn(self, operand_a):
        print(self.reg[operand_a])
        self.pc +=2

    def op_hlt(self):
        sys.exit()
    
    def op_push(self, operand_a):
        self.stack_pointer -= 1
        value = self.ram[operand_a]
        self.ram[self.stack_pointer] = value
        self.pc += 2
    
    def op_pop(self, operand_a):
        val = self.ram[self.stack_pointer]
        self.reg[operand_a] = val
        self.stack_pointer += 1 
        self.pc += 2
    
    def op_ret(self):
        address = self.ram[self.stack_pointer]
        self.pc = address
        self.stack_pointer += 1
    
    def op_call(self, operand_a):
        self.stack_pointer -= 1
        address = self.pc + 2
        self.ram[self.stack_pointer] = address
        sub_address = self.ram[operand_a]
        self.stack_pointer = sub_address
    
    def op_jmp(self, operand_a):
        self.pc = self.reg[operand_a]
    
    def op_jeq(self, operand_a):
        if self.fl == 0b00000010:
            self.pc = self.reg[operand_a]
        else:
            self.pc += 2
    
    def op_jne(self, operand_a):
        if self.fl != 0b00000010:
            self.pc = self.reg[operand_a]
        else:
            self.pc += 2


    def load(self):
        address = 0
        if len(sys.argv) != 2:
            print('Usage: using file <filename>', file = sys.stderr)
            sys.exit(1)
        
        try:
            with open(sys.argv[1])as f:
                for line in f:
                    num = line.split('#')[0]
                    num = num.replace('\n', '')
                    num = num.strip()
                    if num is not '':
                        num = int(num, 2)
                        self.ram[address] = num
                        address +=1
        
        except FileNotFoundError:
            print(f'{sys.argv[0]}: {sys.argv[1]} not found')
            sys.exit()
        
    def alu(self, op, reg_a, reg_b):
        """All ops"""
        MUL = 0b10100010
        ADD = 0b10100000
        SUB = 0b10100001
        DIV = 0b10100011
        XOR = 0b10101011
        SHR = 0b10101101
        SHL = 0b10101100
        CMP = 0b10100111

        if op == ADD:
            self.reg[reg_a] += self.reg[reg_b]

        elif op == MUL:
            self.reg[reg_a] *= self.reg[reg_b]
        
        elif op == SUB:
            self.reg[reg_a] -= self.reg[reg_b]

        elif op == DIV:
            self.reg[reg_a] /= self.reg[reg_b]
        
        elif op == XOR:
            xor = self.reg[reg_a]^self.reg[reg_b]
            self.reg[reg_b] = xor

        elif op == SHR:
            shr = self.reg[reg_a]
            right = shr >> self.reg[reg_b]
            self.reg[reg_a] = right
        
        elif op ==SHL:
            shl = self.reg[reg_a]
            left = shl << self.reg[reg_b]
            self.reg[reg_a] = left
        
        elif op == CMP:
            a = self.reg[reg_a]
            b = self.reg[reg_b]
            if a == b:
                self.fl = 0b00000010
            elif a < b:
                self.fl = 0b00000100
            elif a > b: 
                self.fl = 0b00000001

        else:
            raise Exception('Unsupported ALU operation')
        self.pc += 3
    def trace(self):
      
        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            # self.fl,
            # self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        print()

    def run(self):
        """Run the CPU"""
        
        ir = self.ram[self.pc]

        while True:
            ir = self.ram[self.pc]

            operand_a = self.ram_read(self.pc+1)
            operand_b = self.ram_read(self.pc+2)

            cpu_op = (ir & 0b11000000) >> 6
            alu_op = (ir & 0b00100000) >> 5

            if ir == 0b01010000:
                self.op_table[ir](operand_a)
                continue

            elif ir == 0b00010001:
                self.op_table[ir]()
                continue

            if alu_op:
                self.alu(ir, operand_a, operand_b)
            elif cpu_op == 2:
                self.op_table[ir](operand_a, operand_b)
            elif cpu_op == 1:
                self.op_table[ir](operand_a)
            elif cpu_op == 0:
                self.op_table[ir]()
            else:
                self.op_hlt()