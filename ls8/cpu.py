"""CPU functionality."""

import re



class CPU:
    """Main CPU class."""

    def __init__(self):
        """Construct a new CPU."""
        self.ram = [0]*256
        self.reg = [0]*8
        self.PC = 0
        self.HLT = "0b1"
        self.reg[7] = 243
        self.reg[4] = "00000000"
        self.FLG = self.reg[4]
    def ram_read(self, MAR):
        return self.ram[MAR]
    def ram_write(self, MAR, MDR):
        self.ram[MAR] = MDR

        

    def load(self, program):
        """Load a program into memory."""
    
        with open(program) as program:

            address = 0

            for instruction in program:
                try:
                    binary = re.search(r'\d{8}', instruction).group()
                    self.ram[address] = bin(int(binary,2))
                    address += 1
                except:
                    address += 1
            



    def LDI(self):
        r = input("register number  ")
        n = input("number that is saved     ")
        self.reg[int(r)] = int(n)
        self.PC+=3
        



    def alu(self, op, reg_a, reg_b):
        """ALU operations."""

        if op == "ADD":
            self.reg[reg_a] += self.reg[reg_b]

        elif op == "MULTIPLY":

            self.reg[int(reg_a)] = self.reg[int(reg_a)] * self.reg[int(reg_b)]

            self.PC+=3


        #elif op == "SUB": etc
        else:
            raise Exception("Unsupported ALU operation")

    def trace(self):
        """
        Handy function to print out the CPU state. You might want to call this
        from run() if you need help debugging.
        """

        print(f"TRACE: %02X | %02X %02X %02X |" % (
            self.PC,
            #self.fl,
            #self.ie,
            self.ram_read(self.PC),
            self.ram_read(self.PC + 1),
            self.ram_read(self.PC + 2)
        ), end='')

        for i in range(8):
            print(" %02X" % self.reg[i], end='')


    def run(self, file):
        self.load(file)
        """Run the CPU."""
        running = True
        while running:
            IR = self.ram_read(self.PC)

            if IR == "0b10000010":
                self.LDI()

            elif IR == "0b1000111":
                rn = input("Register Number to Print    ")
                print(self.reg[int(rn)])
                self.PC+=2

            elif IR == "0b10100010":
                r1 = input("register number 1  ")
                r2 = input("register number 2  ")

                self.alu("MULTIPLY", r1, r2)

            elif IR == "0b1000101":
                print(self.ram, self.reg)
                # print('SP PUSH', SP)
                self.reg[7] -= 1
                SP = self.reg[7]

                # print('REG[7] PUSH', self.reg[7])

                address = int(str(self.ram[self.PC + 1]),2)
                # print('ADDRESS PUSH', address)

                value = self.reg[address]
                # print('VALUE PUSH', value)


                self.ram[SP] = value

                # print('ram[SP] PUSH', self.ram[SP])

                self.PC += 2

                print('PC PUSH', self.PC)


            elif IR == "0b1000110":
                print(self.ram, self.reg)
                SP = self.reg[7]
                # print('SP POP', SP)


                value = self.ram[SP]
                # print('VALUE POP', value)


                address = int(str(self.ram[self.PC + 1]),2)
                # print('ADDRESS POP', address)


                self.reg[address] = value
                self.reg[7] += 1
                # print('REG[7] POP', self.reg[7])


                # registers[7] = ( SP + 1 ) % 255

                self.PC += 2
                # print('PC POP', self.PC)





            
            elif IR == self.HLT:
                running = False


