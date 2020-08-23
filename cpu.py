import sys
## operations
LDI = 0b10000010
PRN = 0b01000111
HLT = 0b00000001
CMP = 0b10100111
JMP = 0b01010100
JEQ = 0b01010101
JNE = 0b01010110
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
		self.flag = [0] * 8 #flags register
# set up RAM read
	def ram_read(self, address):
		ram_value = self.ram[address]
# set up RAM write
	def ram_write(self, value, address):
		self.ram[address] = value

# set up instructions ()

	def hlt(self):
		self.running = False

	def ldi(self, reg_index, value):
		self.reg[reg_index] = value
		self.pc += 3
	
	def prn(self, reg_index):
		print(self.reg[reg_index])
		self.pc += 2

	def alu(self, cmd, reg_a, reg_b):
		if cmd == "CMP":
			self.flag = 0b0000000
			# set to 0
			if self.reg[reg_a] == self.reg[reg_b]:
				# set to 1
				self.flag = 0b00000001
			elif self.reg[reg_a] < self.reg[reg_b]:
				# set less than l flag to 1
				self.flag = 0b00000100
			elif self.reg[reg_a] > self.reg[reg_b]:
				# set greater than flag to 1
				self.flag = 0b00000010
			self.pc += 3

	def jmp(self, reg_index):
		self.pc = self.reg[reg_index]
	
	def jeq(self, reg_index):
		# if equal flag is set to true jump to address in given reg
		if self.flag & 1 == 1:
			self.pc = self.reg[reg_index]
		else:
			self.pc +=2

	def jne(self, reg_index):
		# if E flag is CLEar (false, 0) jump to address stored in given reg
		if self.flag & 1 == 0:
			self.pc = self.reg[reg_index]
		else:
			self.pc += 2

	def load(self):
		if len(sys.argv) < 2:
			print("error")
		filename = sys.argv[1]
		# load file and store into memory
		try:
			address = 0
			with open(filename) as file:
				for line in file:
					# read file and commands stripping "#" comments and the empty lines
					command = line.split('#')[0].strip("")
					# add commands to ram
					if command == '':
						continue
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
				value = self.ram[self.pc + 2]
				self.ldi(reg_index, value)

			elif cmd == PRN:
				reg_index = self.ram[self.pc + 1]
				self.prn(reg_index)
			
			elif cmd == CMP:
				reg_a = self.ram[self.pc + 1]
				reg_b = self.ram[self.pc + 2]
				self.alu("CMP", reg_a, reg_b)
			
			elif cmd == JMP:
				reg_index = self.ram[self.pc + 1]
				self.jmp(reg_index)

			elif cmd == JEQ:
				reg_index = self.ram[self.pc + 1]
				self.jeq(reg_index)

			elif cmd == JNE:
				reg_index = self.ram[self.pc + 1]
				self.jne(reg_index)

if __name__ == "__main__":
    cpu = CPU()
    cpu.load()
    cpu.run()