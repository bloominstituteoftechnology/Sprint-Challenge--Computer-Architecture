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
        
            
        # print('RAM-ROD', self.ram)



    def LDI(self):
        r = input("register number  ")
        item = input("item that is saved     ")
        if type(int(item)) == int:
            print('it is a number!')
            self.reg[int(r)] = int(item)
        
        else:
            print('oh no!', type(int(item)))

            self.reg[int(r)] = item

        self.PC+=3
        print('counter is at', self.PC)

        



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
                # print(self.ram, self.reg)
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

                # print('PC PUSH', self.PC)


            elif IR == "0b1000110":
                # print(self.ram, self.reg)
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

                print('counter is at', self.PC)
                # print('PC POP', self.PC)



            elif IR == "0b10100111":
                
                registerA = input("First register to compare  ")
                registerB = input("Second register to compare   ")

                if self.reg[int(registerA)] > self.reg[int(registerB)]:
                    self.FLG = '00000'
                    for i in range(0,3):
                        if i == 0:
                            self.FLG = self.FLG + '1'
                        else:
                            self.FLG = self.FLG + '0'

                if self.reg[int(registerA)] < self.reg[int(registerB)]:
                    self.FLG = '00000'
                    for i in range(0,3):
                        if i == 1:
                            self.FLG = self.FLG + '1'
                        else:
                            self.FLG = self.FLG + '0'

                if self.reg[int(registerA)] == self.reg[int(registerB)]:
                    self.FLG = '00000'
                    for i in range(0,3):
                        if i == 2:
                            self.FLG = self.FLG + '1'
                        else:
                            self.FLG = self.FLG + '0'
                          
                self.PC+=3
                print('counter is at', self.PC)


            elif IR == "0b1010100":
                address == input("Register to jump to   ")

                self.ram[int(self.reg[int(address)])]

                self.PC = int(self.reg[int(address)])
                
                print('counter is at', self.PC)


            elif IR == "0b1010110":

                if self.FLG != "00000001":

                    address = input("Register to jump to   ")


                    self.ram[int(self.reg[int(address)])]
                    self.PC = int(self.reg[int(address)])
                    
                    print('counter is at', self.PC)

                else:
                    self.PC += 2
                    print('counter is at', self.PC)


            elif IR == "0b1010101":
                print('FLAG', self.FLG, self.PC)

                if self.FLG == "00000001":

                    address = input("Register to jump to   ")

                    self.ram[int(self.reg[int(address)])]
                    self.PC = int(self.reg[int(address)])
                    print('counter is at', self.PC)

                
                else:
                    self.PC += 2
                    print('counter is at', self.PC)





            elif IR == self.HLT:
                running = False


# # - [ ] Add the `CMP` instruction and `equal` flag to your LS-8.

# # - [ ] Add the `JMP` instruction.

# # - [ ] Add the `JEQ` and `JNE` instructions.

# t = '1'
# f = '0'
# cpu = CPU()
# group = re.search(r'(00000)(0)(0)(0)', cpu.FLG)
# groupAll = group.group()
# groupAll = re.sub(r'(00000)(0)(1)(1)',q + r'\3\4','00000011')
# groupAll = '00000' + groupAll

# groupAll = '00000111'
# b = '00000'
# f = groupAll[5:6]
# g = groupAll[6:7]
# h = groupAll[7:8]
# g = '0'
# s = b+f+g+h
# print(groupAll, group.group())



