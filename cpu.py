# to run in terminal, type: py ls8.py sctest.ls8

import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256  # 256 bytes of RAM/memory
        self.reg = [0] * 8  # registers 0:7 are saved for quick access
        self.pc = 0  # program counter/pointer
        self.sp = 7
        self.running = False
        self.ops = {
            "LDI": 0b10000010,
            "PRN": 0b01000111,
            "MUL": 0b10100010,
            "HLT": 0b00000001,
            "PSH": 0b01000101,
            "POP": 0b01000110,
            "CAL": 0b01010000,
            "RET": 0b00010001,
            "CMP": 0b10100111,
            "JMP": 0b01010100,
            "JEQ": 0b01010101,
            "JNE": 0b01010110
        }
        self.reset_fl()

    def reset_fl(self):
        self.fl = {
            "E": 0,  # equal
            "L": 0,  # less than
            "G": 0  # greater than
        }

    def load(self):
        """Load a program into memory."""

        address = 0
        program = []

        if len(sys.argv) != 2:
            print('Please add a filename as a second argument.')
            sys.exit(1)  # error

        # get the raw instructions
        unformatted_program = []
        with open(sys.argv[1], 'r') as file:
            unformatted_program = file.readlines()

        # format the instructions (remove comments and \n flags)
        for instruction in unformatted_program:
            formatted = instruction.split()

            # if the line is empty or the line is a comment
            if len(formatted) < 1 or formatted[0] == '#':
                continue  # just continue to the next loop

            program.append(int(formatted[0], 2))
        i = 1
        # load instructions into the program
        for instruction in program:
            # print(i, bin(instruction))
            self.ram[address] = instruction
            address += 1
            i += 1

    def ram_read(self, address):  # read from memory at the location
        return self.ram[address]

    def ram_write(self, address, value):  # write to memory at the location
        self.ram[address] = value
        return self.ram[address]

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
            self.reg[reg_a] = self.reg[reg_a] & 0b11111111
        elif op == "SUB":
            self.reg[reg_a] -= self.reg[reg_b]
            self.reg[reg_a] = self.reg[reg_a] & 0b11111111
        elif op == "MUL":
            self.reg[reg_a] *= self.reg[reg_b]
            self.reg[reg_a] = self.reg[reg_a] & 0b11111111
        elif op == "DIV":
            self.reg[reg_a] /= self.reg[reg_b]
            self.reg[reg_a] = self.reg[reg_a] & 0b11111111
        elif op == "CMP":
            self.reset_fl()
            if self.reg[reg_a] == self.reg[reg_b]:
                # set Equal flag to 1
                self.fl["E"] = 1
            elif self.reg[reg_a] < self.reg[reg_b]:
                # set Less than flag to 1
                self.fl["L"] = 1
            elif self.reg[reg_a] > self.reg[reg_b]:
                # set Greater than flag to 1
                self.fl["G"] = 1
        else:
            raise Exception("Unsupported ALU operation")

    # def trace(self):
    #     print(f"TRACE: %02X | %02X %02X %02X |" % (
    #         self.pc,
    #         self.fl,
    #         # self.ie,
    #         self.ram_read(self.pc),
    #         self.ram_read(self.pc + 1),
    #         self.ram_read(self.pc + 2)
    #     ), end='')
    #     for i in range(8):
    #         print(" %02X" % self.reg[i], end='')
    #     print() print in console current CPU state

    def run(self):
        """Run the CPU."""
        self.running = True

        self.reg[self.sp] = 0xf3  # stack is pointing at top of the stack

        while self.running:

            instruction = self.ram_read(self.pc)  # read current instruction

            if instruction == self.ops["LDI"]:  # LDI
                register_number = self.ram_read(
                    self.pc + 1)  # get the number register
                # get the value to be stored
                value = self.ram_read(self.pc + 2)
                # set the register to that value
                self.reg[register_number] = value
                self.pc += 3  # increment the program counter

            elif instruction == self.ops["PRN"]:  # PRN
                register_number = self.ram_read(
                    self.pc + 1)  # get the register
                # print the value in the register
                print(self.reg[register_number])
                self.pc += 2  # increment the program counter

            elif instruction == self.ops["MUL"]:  # MUL
                reg_a = self.ram_read(self.pc + 1)  # get register a
                reg_b = self.ram_read(self.pc + 2)  # get register b
                # multiply value of reg_a times reg_b
                self.alu("MUL", reg_a, reg_b)
                self.pc += 3  # increment the program counter

            elif instruction == self.ops["PSH"]:
                # if reg_0 = 20, it means that we push the value of this register to the stack
                given_register = self.ram_read(self.pc + 1)
                value_in_register = self.reg[given_register]
                # decrement the Stack Pointer
                self.reg[self.sp] -= 1
                # write the value of the given register to memory AT the SP location
                self.ram_write(self.reg[self.sp], value_in_register)
                self.pc += 2

            elif instruction == self.ops["POP"]:
                given_register = self.ram_read(self.pc + 1)
                # read the value from memory at the top of stack to the given register
                self.reg[given_register] = self.ram_read(self.reg[self.sp])
                # increment the stack pointer
                self.reg[self.sp] += 1
                self.pc += 2

            elif instruction == self.ops["CAL"]:
                print('register at pc', self.reg[self.pc])
                # Get the given register in the operand
                given_register = self.ram_read(self.pc + 1)
                # Store the return address (PC + 2) onto the stack
                # decrement the Stack Pointer
                self.reg[self.sp] -= 1
                # write return address (going to the next instruction)
                self.ram_write(self.reg[self.sp], self.pc + 2)
                # set PC To the value inside given_register
                self.pc = self.reg[given_register]

            elif instruction == self.ops["RET"]:
                # set PC to the value at the top of the stack
                self.pc = self.ram_read(self.reg[self.sp])
                # pop from stack
                self.reg[self.sp] += 1

            elif instruction == self.ops["CMP"]:  # compare
                reg_a = self.ram_read(self.pc + 1)
                reg_b = self.ram_read(self.pc + 2)

                self.alu("CMP", reg_a, reg_b)

                self.pc += 3

                # print(self.fl)

            elif instruction == self.ops["JMP"]:  # jump
                given_register = self.ram_read(self.pc + 1)
                self.pc = self.reg[given_register]

            elif instruction == self.ops["JEQ"]:  # jump if equal
                # if flag is set, then jump to the adress in the register
                if self.fl["E"]:
                    given_register = self.ram_read(self.pc + 1)
                    self.pc = self.reg[given_register]
                # else just go to the next instruction
                else:
                    self.pc += 2

            elif instruction == self.ops["JNE"]:  # jump if not equal
                if not self.fl["E"]:
                    given_register = self.ram_read(self.pc + 1)
                    self.pc = self.reg[given_register]
                else:
                    self.pc += 2

            elif instruction == self.ops["HLT"]:  # HLT
                self.running = False
                self.pc += 1

            else:
                print(f'Unknown instruction: {(instruction)}')
                self.pc += 1
