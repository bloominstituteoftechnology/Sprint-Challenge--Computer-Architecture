"""CPU functionality."""

import sys
import re

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.reg = [0] * 8 # 8 general-purpose registers.
        self.ram = [0] * 256 # to hold 256 bytes of memory

        self.reg[7] = 0xF4
        self.pc = 0
        self.fl = 0b00000000

    def load(self, file):
        """Load a program into memory."""

        address = 0

        # # For now, we've just hardcoded a program:
        with open(file, 'r') as commands:
            for command in commands:
                is_command = re.match("^(0|1)\w+", command)

                if is_command == None:
                    continue
                self.ram[address] = int(is_command.group(0), 2)
                address += 1



    def ram_read(self, mar):
        return self.ram[mar]

    def ram_write(self, mdr, mar):
        self.ram[mar] = mdr

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]
        #elif op == "SUB": etc
        else:
            raise Exception("Unsupported ALU operation")

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

    def run(self):
        """Run the CPU."""
        ir = None # copy of currently execution instruction
        mar = None # holds the address we're reading from
        mdr = None # holds the value to write or just read

        # It needs to read the memory address that's stored in register PC
        # and store that result in IR

        is_running = True

        # Meanings of the bits in the first byte of each instruction: AABCDDDD
        # 01
        # 0
        # 1
        # 0100

        # AA Number of operands for this opcode, 0-2
        # B 1 if this is an ALU operation
        # C 1 if this instruction sets the PC
        # DDDD Instruction identifier
        # 01
        # 0
        # 1
        # 0110
        while is_running:

            is_alu_operation = (self.ram[self.pc] >> 5) & 0b001
            is_setting_pc = (self.ram[self.pc] >> 4) & 0b0001
            # 01000111
            ir = self.ram[self.pc] & 0b0001111

            if is_setting_pc == True:
                # Implement RET
                if (ir == 0b0001): #RET
                    mdr = self.ram[self.reg[7]]
                    self.pc = mdr
                    self.reg[7] += 1
                elif (ir == 0b0000): #CALL
                    # next intruction's address
                    # pushed onto the stack
                    self.reg[7] -= 1
                    self.ram[self.reg[7]] = self.pc + 2

                    # set pc to the address stored in the given address
                    self.pc = self.reg[self.ram[self.pc + 1]]
                elif (ir == 0b0101): # JEQ If equal flag is set (true)
                    is_equal = self.fl & 0b0001
                    # raise Exception(f"{self.fl}")
                    if is_equal == 0b0001:
                        self.pc = self.reg[self.ram[self.pc + 1]]
                        print(f"JEQ \n\nMoving to {self.pc}")
                    else:
                        print(f"JEQ \n\nKeep going and ignoring parameter")
                        self.pc += 2
                elif (ir == 0b0110): # JNE If E flag is clear (false, 0)
                    is_equal = self.fl & 0b0001
                    if is_equal == 0b0000:
                        # print(f"I say flag E is 0 but, is it? {self.fl}. Jumping to {'{0:b}'.format(self.ram[self.reg[self.ram[self.pc + 1]]])}")
                        # raise Exception(f"")
                        self.pc = self.reg[self.ram[self.pc + 1]]
                        print(f"JNE \n\n E Flag is clear, jumping to {self.pc}")
                        # print(f"{'{0:b}'.format(self.ram[self.pc])}")
                        # raise Exception("")
                    else:
                        print(f"JNE \n\n E Flag is not clear, ignoring parameter")
                        self.pc += 2
                elif (ir == 0b0100): #JMP
                    self.pc = self.reg[self.ram[self.pc + 1]]
                    print(f"JMP \n\n  Jumping to {self.pc}")
                continue
            elif is_alu_operation == True:
                # This is an instruction handled by the ALU.
                # What really is a ALU instruction?
                if (ir == 0b0010): # MUL
                    mdr = self.ram[self.pc + 1] # Index of register A
                    mar = self.ram[self.pc + 2] # Index of register B
                    result = self.reg[mdr] * self.reg[mar]
                    self.reg[mdr] = result
                    self.pc += 2
                elif (ir == 0b0000): # ADD
                    mar = self.reg[self.ram[self.pc + 1]]
                    mdr = self.reg[self.ram[self.pc + 1]]
                    self.reg[self.ram[self.pc + 1]] = mar + mdr
                elif (ir == 0b0111): # CMP
                    mar = self.reg[self.ram[self.pc + 1]]
                    mdr = self.reg[self.ram[self.pc + 2]]
                    # print(f"is {mar} and {mdr}")
                    if mar == mdr:
                        self.fl = 0b0001
                    elif mar < mdr:
                        self.fl = 0b0100
                    elif mar > mdr:
                        self.fl = 0b0010
                    print(f"CMP \n\n      RESULT: {self.fl}")
                    self.pc += 2
            else:
                # PRN
                # PRN register pseudo-instruction

                # Print numeric value stored in the given register.

                # Print to the console the decimal integer value that is stored in the given register.
                if (ir == 0b0111): #PRN
                    mdr = self.reg
                    mar = self.ram[self.pc + 1]
                    self.pc += 1
                    print(f"==> RESULT: {mdr[mar]}")
                elif (ir == 0b0010): # LDI
                    # Set the value of a register to an integer.
                    # is the first argument the register?
                    # is the second argument the register?
                    # if (self.fl == 0b0100):
                    #     raise Exception("HER")
                    mdr = self.ram[self.pc + 2]
                    self.reg[self.ram[self.pc + 1]] = mdr
                    print(f"LDI \n\n{mdr}")
                    self.pc += 2
                elif (ir == 0b0101): # PUSH
                    # print("PUSH")
                    # decrement the pointer
                    self.reg[7] -= 1 # decrement pointer
                    # print(f"SP decremented to {self.reg[7]}")
                    mar = self.reg[self.ram[self.pc + 1]]
                    # print(f"Pushing {mar} to reg 7")
                    self.ram[self.reg[7]] = mar
                    # print(f"{mar} pushed to register 7")
                    self.pc += 1
                elif (ir == 0b0110): # POP
                    mdr = self.reg[7]
                    mar = self.ram[mdr]
                    register_index = self.ram[self.pc + 1]
                    self.reg[register_index] = mar
                    self.reg[7] += 1
                    self.pc += 1
                elif (ir == 0b01): # HLT
                    # print("HLT instruction")
                    is_running = False
                    break

            self.pc += 1