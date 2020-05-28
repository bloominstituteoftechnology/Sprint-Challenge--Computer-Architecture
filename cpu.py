"""CPU functionality."""
import sys


class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.register = {k: 0 for k in range(0, 8)}
        self.register[7] = 0xF4
        self.pc = 0
        self.fl = 0b00000000  # 00000LGE
        self.ram = [0] * 256
        self.alu = {
            0b0111: self.CMP,
        }
        self.pc_mutators = {
            0b0000: self.CALL, # Call
            0b0001: self.RET,  # Return
            0b0100: self.JMP,  # Jump Register
            0b0101: self.JEQ,  # Jump IF EQUAL
            0b0110: self.JNE,  # Jump IF NOT EQUAL
        }
        self.general = {
            0b0001: self.HLT,  # Halt
            0b0010: self.LDI,  # Load Register Immediate
            0b0011: self.LD,   # Load registerA with the value at mem address registerB
            0b0100: self.ST,   # Store value in regB in address stored in regA
            0b0101: self.PUSH, # Push value in given reg on to the stack
            0b0110: self.POP,  # Pop value at top of stack INTO given register
            0b0111: self.PRN,  # Print numeric in given register
        }

    def load(self, filename=None):

        """Load a program into memory."""
        address = 0
        if len(sys.argv) > 1:
            filename = sys.argv[1]
        program = open(filename)
        for line in program:
            string_val = line.split("#")[0].strip()
            if string_val == '':
                continue
            v = int(string_val, 2)
            self.ram[address] = v
            address += 1

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """
        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.pc,
            self.ram_read(self.pc),
            self.ram_read(self.pc + 1),
            self.ram_read(self.pc + 2)
        ), end='')
        for i in range(8):
            print(" %02X" % self.register[i], end='')
        print()

    def ram_read(self, position):
        self.pc += 1
        return self.ram[position]

    def ram_write(self, position, value):
        self.ram[position] = value

    def run(self):
        """Run the CPU."""
        self.halted = False
        self.interrupted = False
        while not self.halted:
            current_instruct = self.ram_read(self.pc)
            decoded = self.decode(current_instruct)
            # If instruction uses the ALU
            if decoded['is_alu']:
                func = self.alu[decoded['id']]
            # If instruction sets the PC
            elif decoded['sets_pc']:
                func = self.pc_mutators[decoded['id']]
            # If neither of the above
            else:
                func = self.general[decoded['id']]
            if func is not None:
                # Finally, call the func if given
                if decoded['num_operands'] == 0:
                    func()
                elif decoded['num_operands'] == 1:
                    reg_a = self.ram_read(self.pc)
                    func(reg_a)
                elif decoded['num_operands'] == 2:
                    reg_a = self.ram_read(self.pc)
                    reg_b = self.ram_read(self.pc)
                    func(reg_a, reg_b)
        # sys.exit(0)

    # ALU

    def CMP(self, reg_a, reg_b): # Compare
        val_a = self.register[reg_a]
        val_b = self.register[reg_b]
        if val_a == val_b:
            self.fl = 0b00000001
        elif val_a > val_b:
            self.fl = 0b00000010
        elif val_a < val_b:
            self.fl = 0b00000100
        else:
            self.fl = 0b0

    # PC Control
    def CALL(self, reg): # Call
        self.register[7] -= 1
        self.ram[self.register[7]] = self.pc
        self.pc = self.register[reg]

    def RET(self): # Return
        self.pc = self.ram[self.register[7]]
        self.register[7] += 1

    def JMP(self, reg_a): # Jump to address in reg
        self.pc = self.register[reg_a]

    def JEQ(self, reg_a): # Jump to address in reg IF EQUAL
        a = bool(self.fl & 0b00000001)
        if a:
            self.pc = self.register[reg_a]

    def JNE(self, reg_a): # Jump to address in reg IF NOT EQUAL
        a = bool(self.fl & 0b00000001)
        if not a:
            self.pc = self.register[reg_a]

    # General commands

    def HLT(self): # Halt
        self.halted = True

    def LDI(self, reg_a, val): # Load Register Immediate
        self.register[reg_a] = val

    def LD(self, reg_a, reg_b): # Load registerA with the value at mem address registerB
        self.register[reg_a] = self.ram[self.register[reg_b]]

    def ST(self, reg_a, reg_b): # Store value in regB in address stored in regA
        self.ram[self.register[reg_a]] = self.register[reg_b]

    def PUSH(self, reg_a): # Push value in given reg on to the stack
        self.register[7] -= 1
        self.ram[self.register[7]] = self.register[reg_a]

    def POP(self, reg_a): # Pop value at top of stack INTO given register
        self.register[reg_a] = self.ram[self.register[7]]
        self.register[7] += 1

    def PRN(self, reg_a): # Print numeric in given register
        value = self.register[reg_a]
        print(value)

    def decode(self, instruction): # Convert INT to BINARY; Create dictionary of outputs split by category
        instruction = bin(instruction)[2:].zfill(8)
        output = {'num_operands': int(instruction[:2], 2), 'is_alu': bool(int(instruction[2], 2)),
                  'sets_pc': bool(int(instruction[3], 2)), 'id': int(instruction[4:], 2)}
        return output
