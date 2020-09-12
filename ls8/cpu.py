"""CPU functionality."""

import sys

# from opcodes import *
import opcodes

# MUL = 0b10100010
# ADD = 0b10100000

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.memory = [0] * 256
        self.gen_reg =  { 
                0 : 0b0000000,   # clear to 0b0 on boot
                1 : 0b0000000,
                2 : 0b0001000,
                3 : 0b0000000,
                4 : 0b0000000,
                5 : 0b0000000,    # R5  interrupt mask
                6 : 0b0000000,    # IS  interrupt status
                7 : 0b0000000     # SP  stack pointer   # init to 0b0 
            }

        self.reg = [0] * 8    # needed for trace
        self.reg[7] = 0xf4    # Stack pointer

        self.int_reg =  { 
                        'PC': 0b0000000,    # PC program counter
                        'IR': 0b0000000,    # IR instruction register
                        'MAR': 0b0000000,   # MAR memory address register  
                        'MDR': 0b0000000,   # memory data register
                        'FL': 0b0000000,    # flags
                    }
        
        self.running = False
        self.pc = self.int_reg['PC']  # needed for trace
        # self.pc = 0

        pass

    # def load(self):  
    #     address = 0  
    def load(self, filename):
        """Load a program into memory."""

        # #  print(f' >>>> sys.argv    {sys.argv} sys.argv')

        program = []
        address = 0

        try:    
            with open(filename) as f:
                for line in f:
                    if line.split("#")[0] != '\n' and line.split("#")[0] != '':
                        # program.append(line.split('#')[0].strip())

                        self.memory[address] = int(line.split('#')[0].strip(), 2)
                        address += 1
        except FileNotFoundError:
            print(f' filename {filename} NOT FOUND ')
            sys.exit(2)

        print(f' NOW program is {program} ')


        # For now, we've just hardcoded a program:

        # program = [
        #     # From print8.ls8
        #     0b10000010, # LDI R0,8
        #     0b00000000,
        #     0b00001000,
        #     0b01000111, # PRN R0
        #     0b00000000,
        #     0b00000001, # HLT
        # ]

        # program = [
        #     0b10000010, # LDI R1,MULT2PRINT
        #     0b00000001,
        #     0b00011000,
        #     0b10000010,# LDI R0,10
        #     0b00000000,
        #     0b00001010,
        #     0b01010000,# CALL R1
        #     0b00000001,
        #     0b10000010, # LDI R0,15
        #     0b00000000,
        #     0b00001111,
        #     0b01010000, # CALL R1
        #     0b00000001,
        #     0b10000010, # LDI R0,18
        #     0b00000000,
        #     0b00010010,
        #     0b01010000, # CALL R1
        #     0b00000001,
        #     0b10000010, # LDI R0,30
        #     0b00000000,
        #     0b00011110,
        #     0b01010000, # CALL R1
        #     0b00000001,
        #     0b00000001, # HLT
        #     # MULT2PRINT (address 24):
        #     0b10100000, # ADD R0,R0
        #     0b00000000,
        #     0b00000000,
        #     0b01000111, # PRN R0
        #     0b00000000,
        #     0b00010001, # RET
        # ]

        # for instruction in program:
        #     self.memory[address] = instruction
        #     address += 1


    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        # I removed quotes around variables
        print(f' op  {op}')
        if op == opcodes.ADD:
            self.reg[reg_a] += self.reg[reg_b]
        elif op == opcodes.MUL:
            self.reg[reg_a] *= self.reg[reg_b]  
            # print(f' after mul >> self.reg[reg_a] {self.reg[reg_a]}  self.reg[reg_b] {self.reg[reg_b]} ')

            # print(f' after MUL >>  reg_a {self.reg[reg_a]}' )  
        #elif op == "SUB": etc
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        # changed this is %02d from 02%x so I can see mem addresses in decimal and file lines easier
        print(f"TRACE: %02d | %02X %02X %02X || line: %02d ||" % (
            self.pc,
            #self.fl,
            #self.ie,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2), 
            self.pc + 1
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')

        # for i in range(8):
        #     print(" %02X" % self.gen_reg[i], end='')


        print()

    def run(self):
        """Run the CPU."""
        self.running = True
        # self.load()    # REMOVE to read file

        while self.running:
            # load up  
            # instruction_r = self.ram_read(self.int_reg['PC']) 
            # op_a = self.ram_read(self.int_reg['PC'] + 1)
            # op_b = self.ram_read(self.int_reg['PC'] + 2)

            instruction_r = self.ram_read(self.pc) 
            op_a = self.ram_read(self.pc + 1)
            op_b = self.ram_read(self.pc + 2)                


            self.trace()

            if instruction_r == opcodes.HLT:
                print(f' HLT called')
                # self.dump_mem()
                self.running = False
                break

            elif instruction_r == opcodes.LDI:
                print(f' LDI called ')
                self.reg[op_a] = op_b            
                # self.int_reg['PC'] += 3
                self.pc += 3

            elif instruction_r == opcodes.PRN:
                print(f'\t PRN called')    
                print(f'\t reg {op_a} val: {self.reg[op_a]}')
                # self.int_reg['PC'] += 2
                self.pc += 2

            elif instruction_r == opcodes.ADD:
                print(f' ADD called')
                self.alu(opcodes.ADD, op_a, op_b)
                self.pc += 3
                # self.pc =+3    # Not sure why linter did not see this


            elif instruction_r == opcodes.MUL:
                print(f' MUL called ')
                # print(f' op_a_reg {op_a}    op_b_reg  {op_b} ')
                print(f' reg {op_a} val: {self.reg[op_a]}  reg {op_b} val: {self.reg[op_b]}')
                self.alu(opcodes.MUL, op_a, op_b)           
                # self.int_reg['PC'] += 3
                self.pc += 3 


            elif instruction_r == opcodes.PUSH:
                print(f' PUSH called')
                self.reg[7] -= 1   # decrement SP >> PUSH
                # print(f'>> self.reg[self.memory[self.pc + 1]] {self.reg[self.memory[self.pc + 1]]}')
                # print(f' >>> self.memory[self.reg[7]]   {self.memory[self.reg[7]]} ]')
                print(f' >> self.reg[7]   self.memory[self.reg[7]]  = self.reg[self.memory[self.pc + 1]]')
                print(f' >> \t {hex(self.reg[7])} \t    {self.memory[self.reg[7]]} =  {self.reg[self.memory[self.pc + 1]]}')
                self.memory[self.reg[7]] = self.reg[self.memory[self.pc + 1]]

                # print(f' >>>> self.memory[self.reg[7]]   {self.memory[self.reg[7]]}')
                # print(f' self.reg[op_a]  {self.reg[op_a]}')
                print(f' >> write (self.reg[op_a], self.memory[self.reg[7]])   ')
                print(f' >>  write ({self.reg[op_a]}, {self.memory[self.reg[7]]})   ')
                self.ram_write(self.reg[op_a], self.reg[7])
                self.pc += 2

            elif instruction_r == opcodes.POP:
                print(f' POP called')
                val = self.ram_read(self.reg[7])
                print(f' >>>>  val {val}')

                self.reg[7] += 1   # increment SP >> POP

                print(f' >>>>  self.reg[op_a] = val ')
                print(f' >>>>  reg    {op_a}')
                print(f' >>>> previously: {self.reg[op_a]} = now: {val}')
                self.reg[op_a] = val
                self.pc += 2

            elif instruction_r == opcodes.CALL:
                print(f' CAll has been called')
                self.reg[7] -= 1
                

                print(f'   self.memory[self.reg[7]]  =  self.pc + 2 ')
                print(f' \t\t\t {self.memory[self.reg[7]]}  =  {self.pc + 2} ')
                self.memory[self.reg[7]] = self.pc + 2

                self.pc = self.reg[op_a]

            elif instruction_r == opcodes.RET:
                print(f' RET called')
                print(f' \t   self.pc  =   self.memory[self.reg[7]]  ')
                print(f' \t\t{self.pc}  =   {self.memory[self.reg[7]]}  ')
                self.pc = self.memory[self.reg[7]]

                print(f' {hex(self.reg[7])}')
                self.reg[7] += 1

        pass

    # def dump_mem(self):
    #     print(f' mem size: {len(self.memory)}\n ****** DUMP  *******')
        
    #     for i in range(0, len(self.memory), 8):
    #         for j in range(i, len(self.memory), 8):
    #             end = i + 7
    #             # print(f' i {i }  :  j {i + 8}')
    #             print(f' {self.memory[i:end]}  mem {i } : {end} ')
    #             break        

    def dump_mem(self, base = None):
        print(f' mem size: {len(self.memory)}\n ****** DUMP  *******')
        
        for i in range(len(self.memory) - 1, 0, -8):
            for j in range(len(self.memory) - 1, i - 8, -8):
                end = i - 7
                # print(f' i {i }  :  j {end}')
                str_mem = ''
                for item in self.memory[end: i + 1]:
                    if base == 'b':
                        str_mem +=  str(bin(item)) + ",\t"
                    elif base == 'h':
                        str_mem +=  str(hex(item)) + ",\t "
                    else:
                        str_mem += str(item) + ",\t" 
                    # print(f' {str_mem}')
                # print(f' {(self.memory[end: i + 1])}  \t\t  mem {end } : {i} ')
                print(f' {str_mem}  \t\t  mem {end } : {i} ')
                break    






    def read_gen_reg_b(self, reg):
        return bin(self.gen_reg[reg])   

    def read_gen_reg_h(self, reg):
        return hex(self.gen_reg[reg])

    def read_gen_reg_d(self, reg):
        bin_val = int(reg, 2)
        return self.gen_reg[bin_val]    

    def return_bin(self, reg_val):
        return bin(reg_val)

    def read_reg(self, reg_type = 'gen_reg', reg = '0', base = 'None'):
        # reg_v = f'self.reg_type'
        # print(f'reg_v {reg_v} ')
        # if base == 'hex':
        #     return hex(reg_v[reg])
        # elif base == 'bin':
        #     return bin(reg_v[reg])    
        # else:
        #      return reg_v    

        if reg_type == 'gen_reg':
            if base == 'hex':
                return self.read_gen_reg_h(reg)
            elif base == 'bin':
                return bin(self.gen_reg[reg])
            else:
                return self.gen_reg[reg]

        elif reg_type == 'int_reg':
            if base == 'hex':
                return hex(self.int_reg[reg])
            elif base == 'bin':
                return self.return_bin(self.int_reg[reg])
            else:
                return self.int_reg[reg]               

   



    def check_valid_h(self, val):
        test_val = str(int(val, 16))
        if test_val >= '0' and test_val <= '255':
            # val = int(val, 16)
            # print(type(val))
            return val

        else:
            return None            

    def check_valid_b(self, val):
        test_val = str(val)
    
        if test_val >= '0' and test_val <= '255':
        # val = int(val, 16)
            # print(type(val))
            return bin(val)
        else:
            return None


    def write_reg_h(self, reg_type, reg, val):
        if self.check_valid_h(val): 
            if reg_type == 'gen_reg':    
                self.gen_reg[reg] = val
            elif reg_type == 'int_reg':
                self.int_reg[reg] = val
            else:
                return None    

    def write_mem_h(self, address, val):
        if int(val, 16):
            print(f' HEX write given')
            # return
        
        
        if self.check_valid_h(val):
            self.memory[address] = hex(int(val, 16))
            return val
        return None
            
    def read_mem_h(self, address):
        if self.check_valid_h(str(address)):
            return self.memory[address]
        else:
            raise ValueError(' Address out of range 0-255')
            print(f' out of range')    

    def ram_write(self, address, val):
        self.memory[address] = val
        return val
        
        # self.memory[address] = hex(val)
        # return hex(val)
        
    def ram_read(self, address):
        return self.memory[address]
        # return hex(self.memory[address])



# cpu = CPU()
# print(f' memory is {cpu.memory} ')  
# cpu.dump_mem()
# print(f'*********   register  **********')
# print(cpu.read_gen_reg_b(0))   # 0b1101010
# print(f' used a binary register val')
# print(cpu.read_gen_reg_d('010'))

# print(cpu.read_gen_reg_h(0))   # 0x6a
# print(cpu.read_reg('gen_reg', 0, 'hex'))  # 0x6a
# print(cpu.read_reg('gen_reg', 0, 'bin'))  # 0b1101010
# print(cpu.read_reg('gen_reg', 0))     # 106
# print(cpu.read_reg('int_reg', 'FL', 'hex'))  # 0x7
# print(cpu.read_reg('int_reg', 'FL', 'bin'))  # 0b111
# print(cpu.read_reg('int_reg', 'FL'))         # 7

# print(cpu.check_valid_h('FF'))
# print(cpu.check_valid_h('0c0'))
# print(cpu.check_valid_h('0b010'))  # bin returns None
# print(cpu.check_valid_h('122'))   # dec returns none

# cpu.write_reg_h('int_reg', 'FL', 'EF')
# print(cpu.read_reg('int_reg', 'FL'))

# print(f'  ***********   memory    ************')
# print(cpu.write_mem_h(7, 'EF'))
# print(cpu.write_mem_h(10, 'BB'))

# print(cpu.write_mem(11, 0b1101))

# cpu.dump_mem()

# print(cpu.read_mem(10))
# print(cpu.read_mem(500))   # should make error


# print(cpu.check_valid_b(0b10000010))
# print(cpu.write_mem_b(0, 0b10000010))
# print(cpu.read_mem_b(0))
# cpu.dump_mem()

# cpu.load('ls8/examples/print8.ls8')
# cpu.dump_mem('h')
# cpu.run()
# cpu.dump_mem('h')
