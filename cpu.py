import sys

## operations
HLT = 0b00000001
LDI = 0b10000010
PRN = 0b01000111

class CPU:
	def __init__(self):
		## set to run
		self.running = True
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

# set up instructions ()

	def HTL(self):
		self.running = False

	def LDI(self, reg_index, num_to_save):
		num_to_save = self.ram[self.pc + 2]
		reg_index = self.ram[self.pc + 1]
		self.reg[reg_index] = num_to_save
		self.pc += 3
	
	def PRN(self, reg_index, val_to_print):
		val_to_print = self.ram[self.pc + 1]
		self.pc += 2



# def Load
	def load(self):
		""" Load a program into memory. """
		pass

# def running
	def run(self):
		self.load()
		while self.running:
			pass

