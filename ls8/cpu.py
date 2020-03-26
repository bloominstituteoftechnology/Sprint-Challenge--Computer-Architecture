"""CPU functionality."""

import msvcrt
import re
import sys
import time

class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        # general registers
        self.reg = bytearray([0]*7 + [0xf4]) # [00,00,00,00,00,00,00,f4]

        # memory
        self.ram = bytearray(256)

        # internal registers
        self.pc = 0 # Program Counter: address of the currently executing instruction
        self.mar = 0 # Memory Address Register: holds the memory address we're reading or writing
        self.mdr = 0 # Memory Data Register: holds the value to write or the value just read
        self.fl = 0 # Flags: 00000LGE; L: less than, G: Greater than, E: Equal

        # cu instructions
        CALL = 0b1010000
        HLT = 0b00000001
        INT = 0b01010010
        IRET = 0b00010011
        JEQ = 0b01010101
        JGE = 0b01011010
        JGT = 0b01010111
        JLE = 0b01011001
        JLT = 0b01011000
        JMP = 0b01010100
        JNE = 0b01010110
        LD = 0b10000011
        LDI = 0b10000010
        NOP = 0b00000000
        POP = 0b01000110
        PRA = 0b01001000
        PRN = 0b01000111
        PUSH = 0b01000101
        RET = 0b00010001
        ST = 0b10000100

        # alu instructions
        ADD = 0b10100000
        AND = 0b10101000
        CMP = 0b10100111
        DEC = 0b01100110
        DIV = 0b1010001
        INC = 0b01100101
        MOD = 0b10100100
        MUL = 0b10100010
        NOT = 0b01101001
        OR = 0b10101010
        SHL = 0b10101100
        SHR = 0b10101101
        SUB = 0b10100001
        XOR = 0b10101011

        # handlers
        self.handle = {
            ADD: self.handle_ADD,
            CALL: self.handle_CALL,
            CMP: self.handle_CMP,
            DEC: self.handle_DEC,
            HLT: self.handle_HLT,
            INC: self.handle_INC,
            IRET: self.handle_IRET,
            JEQ: self.handle_JEQ,
            JLT: self.handle_JLT,
            JMP: self.handle_JMP,
            LD: self.handle_LD,
            LDI: self.handle_LDI,
            MUL: self.handle_MUL,
            POP: self.handle_POP,
            PRA: self.handle_PRA,
            PRN: self.handle_PRN,
            PUSH: self.handle_PUSH,
            RET: self.handle_RET,
            SHL: self.handle_SHL,
            ST: self.handle_ST,
        }

    def ram_read(self, address):
        """Accepts an address to read and returns the value stored there."""
        self.mar = address
        self.mdr = self.ram[self.mar]
        return self.mdr

    def ram_write(self, value, address):
        """Accepts a value and address to write the value to."""
        self.mdr = value
        self.mar = address
        self.ram[self.mar] = self.mdr

    def load(self, path):
        """Load a program into memory."""
        address = 0

        with open(path) as f:
            program = f.read()
            program = re.findall('^[01]{8}', program, re.MULTILINE)
            program = map(lambda code: int(code, 2), program)

        for instruction in program:
            self.ram[address] = instruction
            address += 1

    def alu(self, op, reg_a, reg_b):
        """ALU operations."""
        try:
            self.handle[op](reg_a, reg_b)
        except KeyError:
            raise Exception("Unsupported ALU operation")

    def cu(self, op, operand_a, operand_b):
        """CU operations."""
        try:
            self.handle[op](operand_a, operand_b)
        except KeyError:
            raise Exception("Unsupported CU operation")

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
        # run program until HLT
        interupt_time = time.time()
        self.running = True
        while self.running:
            # get keypress
            if msvcrt.kbhit():
                key = msvcrt.getch()[0]
                self.ram_write(key, 0xf4)
                self.reg[6] = self.reg[6] | 0b00000010
            
            # set bit #0 of R6 to 1 every second
            now = time.time()
            if now - interupt_time >= 1:
                self.reg[6] = self.reg[6] | 0b00000001
                interupt_time = now

            # handle interupts: compare R5 interrupt mask (IM) and R6 interrupt status (IS)
            masked_interrupts = self.reg[5] & self.reg[6]
            for i in range(8):
                if (masked_interrupts >> i) & 1:
                    self.reg[6] = self.reg[6] & int('1'*(7-i) + '0' + '1'*(i), 2) # clear bit in IS (i.e. 01000001 → 01000000)
                    # add to stack
                    self._push(self.pc)
                    self._push(self.fl)
                    self.fl = 0
                    for n in range(7):
                        self._push(self.reg[n])
                        self.reg[n] = 0 # disables interupts by clearing R5 (IM) & R6 (IS)
                    # jmp to vector
                    iv_ptr = 0xf8+i # interupt vector pointer
                    self.pc = self.ram_read(iv_ptr)
                    break

            ir = self.ram_read(self.pc) # Instruction Register: contains a copy of the currently executing instruction
            operand_a = self.ram_read(self.pc + 1)
            operand_b = self.ram_read(self.pc + 2)

            op_count = (ir & 0b11000000) >> 6
            alu_oper = (ir & 0b00100000) >> 5

            self.pc += 1 + op_count

            if alu_oper:
                self.alu(ir, operand_a, operand_b)
            else:
                self.cu(ir, operand_a, operand_b)

    def _push(self, val):
        """Push immediate value to stack"""
        self.reg[7] -= 1
        sp = self.reg[7]
        self.ram_write(val, sp)

    def _pop(self):
        """Pop value from stack"""
        sp = self.reg[7]
        val = self.ram_read(sp)
        self.reg[7] += 1
        return val
    
    def handle_ADD(self, a, b):
        """Add values of 2 registers and store sum in the first."""
        self.reg[a] += self.reg[b]

    def handle_CALL(self, a, b):
        """Add next PC ptr to stack, set PC to address from register"""
        self.reg[7] -= 1
        sp = self.reg[7]
        val = self.pc
        self.ram_write(val, sp)

        self.pc = self.reg[a]

    def handle_CMP(self, a, b):
        """Compare the values in two registers."""
        self.fl = 0b00000000
        if self.reg[a] < self.reg[b]:
            self.fl = self.fl | 0b00000100
        if self.reg[a] > self.reg[b]:
            self.fl = self.fl | 0b00000010
        if self.reg[a] == self.reg[b]:
            self.fl = self.fl | 0b00000001

    def handle_DEC(self, a, b):
        """Decrement (subtract 1 from) the value in the given register."""
        self.reg[a] -= 1

    def handle_HLT(self, a, b):
        """Stop the cpu run loop."""
        self.running = False

    def handle_INC(self, a, b):
        """Increment (add 1 to) the value in the given register."""
        self.reg[a] += 1

    def handle_IRET(self, a, b):
        """Return from an interrupt handler."""
        for i in range(7):
            self.reg[6 - i] = self._pop() # enables interupts by restoring R5 (IM) & R6 (IS)
        self.fl = self._pop()
        self.pc = self._pop()

    def handle_JEQ(self, a, b):
        """If equal flag is set, jump to the address stored in the given register."""
        if self.fl & 0b00000001:
            self.pc = self.reg[a]

    def handle_JLT(self, a, b):
        """If less-than flag is set (true), jump to the address stored in the given register."""
        if self.fl & 0b00000100:
            self.pc = self.reg[a]

    def handle_JMP(self, a, b):
        """Jump to the address stored in the given register."""
        self.pc = self.reg[a]

    def handle_LD(self, a, b):
        """Loads registerA with the value at the memory address stored in registerB."""
        self.reg[a] = self.ram_read(self.reg[b])

    def handle_LDI(self, a, b):
        """Store a value in a register."""
        self.reg[a] = b

    def handle_MUL(self, a, b):
        """Multiply values of 2 registers and store product in the first."""
        self.reg[a] *= self.reg[b]

    def handle_POP(self, a, b):
        """Pop value from top of the stack to a register and update stack pointer (R7)."""
        sp = self.reg[7]
        self.reg[a] = self.ram_read(sp)
        self.reg[7] += 1

    def handle_PRA(self, a, b):
        """Print to the console the ASCII character corresponding to the value in the register."""
        print(chr(self.reg[a]), end="")

    def handle_PRN(self, a, b):
        """Print value from a register."""
        print(self.reg[a])

    def handle_PUSH(self, a, b):
        """Update stack pointer (R7) and push a value from a register to top of the stack."""
        self.reg[7] -= 1
        sp = self.reg[7] 
        val = self.reg[a]
        self.ram_write(val, sp)

    def handle_RET(self, a, b):
        """Pop the value from the top of the stack and store it in the PC."""
        sp = self.reg[7]
        self.pc = self.ram_read(sp)
        self.reg[7] += 1

    def handle_SHL(self, a, b):
        """Shift the value in registerA left by the number of bits specified in registerB, filling the low bits with 0."""
        self.reg[a] = self.reg[a] << self.reg[b]

    def handle_ST(self, a, b):
        """Store value in registerB in the address stored in registerA."""
        ptr = self.reg[a]
        val = self.reg[b]
        self.ram_write(val, ptr)
