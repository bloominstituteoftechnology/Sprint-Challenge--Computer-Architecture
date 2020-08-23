import sys
## operations
LDI = 0b10000010
PRN = 0b01000111
HLT = 0b00000001
CMP = 0b10100111

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

	def hlt(self):
		print("htl")
		self.running = False

	def ldi(self, reg_index, num_to_save):
		num_to_save = self.ram[self.pc + 2]
		reg_index = self.ram[self.pc + 1]
		self.reg[reg_index] = num_to_save
		self.pc += 3
	
	def prn(self, reg_index, val_to_print):
		val_to_print = self.ram[self.pc + 1]
		self.pc += 2

	def alu(self, cmd, reg_a, reg_b):
		if cmd == "CMP":
			# set to 0
			self.flag = 0b00000000
			if self.reg[reg_a] == self.reg[reg_b]:
				# set to 1
				self.flag = 0b00000001
			elif self.reg[reg_a] < self.reg[reg_b]:
				# set less than l flag to 1
				self.flag = 0b00000100
			elif self.reg[reg_a] > self.reg[reg_b]:
				# set greater than flag to 1
				self.flag = 0b00000010

	def load(self, filename):
		# load file and store into memory
		try:
			address = 0
			with open(filename) as file:
				for line in file:
					# read file and commands stripping "#" comments and the empty lines
					command = line.split("#")[0].strip(" ")
					if command == '':
						continue
					# add commands to ram
					cmd = int(command, 2)
					# store commands in memory
					self.ram[address] = cmd

					address += 1

		except FileNotFoundError:
			print(f"{sys.argv[0]}: {filename} not found")
			sys.exit()

# def running
	def run(self):
		while self.running:
			cmd = self.ram[self.pc]
			
			if cmd == HLT:
				self.hlt()

			elif cmd == LDI:
				reg_index = self.ram[self.pc + 1]
				num_to_save = self.ram[self.pc + 2]
				self.ldi(reg_index, num_to_save)

			elif cmd == PRN:
				reg_value = self.ram[self.pc + 1]
				self.prn(val_to_print)
			
			elif cmd == CMP:
				reg_a = self.ram[self.pc + 1]
				reg_b = self.ram[self.pc + 2]
				self.alu("CMP", reg_a, reg_b)


if __name__ == "__main__":
    cpu = CPU()
    cpu.load(filename="sctest.ls8")
    cpu.run()


