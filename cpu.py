"""CPU functionality."""

# Imports
import sys
from helpers import *

# Define the CPU class
class CPU:
    """Main CPU class."""
    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0] * 256
        self.registers = {
            0: 0,
            1: 0,
            2: 0,
            3: 0,
            4: 0,
            5: 0,
            6: 0,
            7: 0      # Reserved for 'SP' - the Stack Pointer
        }
        self.registers_internal = {
            "PC": 0,
            "IR": 0,
            "MAR": 0,
            "MDR": 0,
            "FL": 0
        }
        self.instructions   = []
    
    def hex2dec(self, hx_str):
        """return the decimal value associated with various hex numbers in string form"""
        map_hx2dec = {
            "F4": 244
        }
        return map_hx2dec[hx_str]

    def bitwise(self, val1, val2, op):
        """
        execute a bitwise operation (op) on two passed decimal values (val1, val2)
        """
        ret_val = 0
        if   op == "and":
            ret_val = int(bin(val1&val2), 2)
        elif op == "or":
            ret_val = int(bin(val1|val2), 2)
        else:
            print(f"ERR: invalid bitwise operation: {op}")
            quit()

        return ret_val

    def read_file(self, fname):
        """
        read_file reads instructions from the passed filename and loads them
        into the 'instructions' object variable
        """

        #* Open the file
        try:
            # print("Open file: {fname}".format(fname=fname))
            fle = open(fname, 'r')
        except:
            print("ERR: error opening input file - terminating")
            quit()

        #* Read the file contents into a list
        try:
            lines = fle.readlines()
            # print("INF: read in {num_lines}".format(num_lines=len(lines)))
        except:
            print("ERR: error reading the input file - terminating")
            quit()

        #* Iterate through the list of file lines
        self.instructions = []
        for lne in lines:
            # Skip non instruction lines
            if lne[0] == "#":
                # skip a standard comment line
                continue

            if len(lne) < 8:
                # skip a short line (e.g. whitespace)
                continue
            # generate a binary instruction from the line
            #   being iterated on
            instr = genInt(lne[:8])
            self.instructions.append(instr)
    
    def load(self):
        """Load a program into memory."""
        address = 0

        # grab the passed program instructions
        for instruction in self.instructions:
            self.ram[address] = instruction
            address += 1

        # Initialize the 'SP' Stack Pointer register
        self.registers[7] = self.hex2dec("F4")

    def ram_read(self, loc):
        """Return the value that passed memory location"""
        self.registers_internal["MAR"] = loc            # the memory address being read
        self.registers_internal["MDR"] = self.ram[loc]  # the value just read
        return self.ram[loc]

    def ram_write(self, val, loc):
        """Write the value to the passed memory location"""
        self.registers_internal["MAR"] = loc    # the memory address being written to
        self.registers_internal["MDR"] = val    # the value to be written
        self.ram[loc] = val
        return 

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        if   op == "ADD":
            self.registers[reg_a] += self.registers[reg_b]
        elif op == "MUL":
            self.registers[reg_a] = self.registers[reg_a] * self.registers[reg_b]
        elif op == "CMP":
            # Reset the comparison flags
            self.registers_internal['FL'] = self.bitwise(self.registers_internal['FL'], 248, "and")

            # Compare register 'a' to register 'b'
            if self.registers[reg_a] < self.registers[reg_b]:
                # register a < register b: set "less than" flag to 1
                self.registers_internal['FL'] = self.bitwise(self.registers_internal['FL'], 4, "or")
            elif self.registers[reg_a] > self.registers[reg_b]:
                # register a > register b: set "greater than" flag to 1
                self.registers_internal['FL'] = self.bitwise(self.registers_internal['FL'], 2, "or")
            else:
                # register a == register b: set the "equal" flag to 1
                self.registers_internal['FL'] = self.bitwise(self.registers_internal['FL'], 1, "or")
        else:
            raise Exception("Unsupported ALU operation")

        if self.registers[reg_a] > 255:
            self.registers[reg_a] = 255

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

    def stack_push(self, val):
        """stack_push pushes the value onto the 'stack'"""
        # Increment the stack counter
        self.registers[7] = self.registers[7] - 1
        # Push the value onto stack (set the value at the now current location)
        self.ram_write(val, self.registers[7])

    def stack_pop(self):
        """stack_pop pops and returns the current value from the 'stack'"""
        # Read the value a the current stack location
        ret_val = self.ram_read(self.registers[7])
        # Adjust the stack pointer to the next logical stack value
        self.registers[7] = self.registers[7] + 1

        return ret_val

    def run(self):
        """Run the CPU."""
        ctr_ovrflw = 1

        # Execute while the CPU is deemed running
        while True:
            if ctr_ovrflw > 1000000:
                print("ERR: runaway process encountered. Terminate process")
                break

            #* Read the data at the program counter ('') memory location
            #   PC: program counter
            #   IR: instruction register
            self.registers_internal["IR"] = self.ram[self.registers_internal["PC"]]

            # Read the next two memory locations (in case they happen to be operands)
            loc_operand_a = self.registers_internal["PC"] + 1
            loc_operand_b = self.registers_internal["PC"] + 2
            operand_a     = self.ram_read(loc_operand_a)
            operand_b     = self.ram_read(loc_operand_b)
            instr         = self.registers_internal["IR"]

            #* Execute the current instruction
            # PUSH Instruction: push register value onto the stack - Binary = 1000101
            if instr == 69:
                self.stack_push(self.registers[operand_a])

                # Advance the program counter 3 memory locations
                self.registers_internal["PC"] = self.registers_internal["PC"] + 2 

            # POP Instruction: pop the current stack value from the stack - Binary = 1000110
            #   and load that value into the register reference by operand A
            elif instr == 70:
                # Pop the value
                tmp_val = self.stack_pop()
                # Load the value into the register reference in operand A
                self.registers[operand_a] = tmp_val

                # Advance the program counter 3 memory locations
                self.registers_internal["PC"] = self.registers_internal["PC"] + 2 

            # PRN Instruction: print register - Binary = 1000111
            elif instr == 71:
                # Operand A: register containing the data to print
                print(f'{self.registers[operand_a]}')

                # Advance the program counter 2 memory locations
                self.registers_internal["PC"] = self.registers_internal["PC"] + 2

            # JMP Instruction: jump to the instruction stored in ram pointed to by the - Binary = 1010100
            #    memory address stored in the passed register
            elif instr == 84:
                # Operand A: register containing the memory address to execute
                self.registers_internal["PC"] = self.registers[operand_a]               

            # JEQ Instruction: if the "equal" flag is set, jump execution to the - Binary = 1010101
            #    memory address stored in the passed register
            elif instr == 85:
                # Operand A: register containing the memory address to execute
                # Is the "equal" flag set? 
                if self.bitwise(self.registers_internal["FL"], 1, "and") == 1:
                    # "equal" flag is set --> set the 'PC' register ("program counter") 
                    #     to the register a's value
                    self.registers_internal["PC"] = self.registers[operand_a]
                else:
                    # "equal" flag not set --> just advance to the next instruction
                    self.registers_internal["PC"] = self.registers_internal["PC"] + 2 

            # JNE Instruction: if the "equal" flag is off, jump execution to the - Binary = 1010110
            #    memory address stored in the passed register
            elif instr == 86:
                # Operand A: register containing the memory address to execute
                # Is the "equal" flag set? 
                if self.bitwise(self.registers_internal["FL"], 1, "and") == 0:
                    # "equal" flag is off (not set) --> set the 'PC' register ("program counter") 
                    #     to the register a's value
                    self.registers_internal["PC"] = self.registers[operand_a]
                else:
                    # "equal" flag is set (condition fails) --> just advance to the next instruction
                    self.registers_internal["PC"] = self.registers_internal["PC"] + 2

            # LDI Instruction: load into register - Binary = 10000010
            elif instr == 130:
                # Attempting to access a reserved register?
                if operand_a in [5, 6, 7]:
                    # attempting to access a reserved register: 5, 6, 7
                    print("ERR: attempting to access a reserved register: 5, 6, 7")
                    break

                # Operand A: register in which to load data
                # Operand B: data to be loaded
                self.registers[operand_a] = operand_b

                # Advance the program counter 3 memory locations
                self.registers_internal["PC"] = self.registers_internal["PC"] + 3 
            
            # MUL Instruction: Multiply registers - Binary = 10100010
            elif instr == 162:
                # Invoke the ALU to execute register multiplication
                self.alu("MUL", operand_a, operand_b)

                # Advance the program counter 3 memory locations
                self.registers_internal["PC"] = self.registers_internal["PC"] + 3 

            # CMP Instruction: Compare values in two registers - Binary = 10100111
            elif instr == 167:
                # Invoke the ALU to execute register comparison
                self.alu("CMP", operand_a, operand_b)

                # Advance the program counter 3 memory locations
                self.registers_internal["PC"] = self.registers_internal["PC"] + 3 

            # DBG Instruction: trip a debug breakpoint - Binary = 11111110
            elif instr == 254:
                print("DEBUG STATEMENT")
                self.registers_internal["PC"] = self.registers_internal["PC"] + 1

            # HLT Instruction: halt execution - Binary = 00000001
            elif instr == 1:
                quit()

            # Invalid Instruction
            else:
                print(f"ERR: read in invalid instruction: {instr} at ram location {self.registers_internal['PC']}")
                print("ERR: invalid instruction encountered. Terminating")
                quit()

            ctr_ovrflw = ctr_ovrflw + 1
             
    def foo(self):
        """foo is a debugging convenience method"""
        return True