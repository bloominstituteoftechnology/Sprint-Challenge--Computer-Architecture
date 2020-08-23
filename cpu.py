import sys

## operations
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111

class CPU:
	def __init__(self):+
		## set to run
        self.running = true
        # set a counter
        self.pc = 0
        # set memory
        self.ram = [0] * 256
        # set registers
        self.reg = [0] * 8
		# set flag
		self.flag = None
# set up RAM read
	def ram_read(self, address):
		ram_value = self.ram[address]
# set up RAM write
	def ram_write(self, value, address):
		self.ram[address] = value

# def Load
	def load(self):
		""" Load a program into memory. """
		pass

# def running
	while running:
		pass

