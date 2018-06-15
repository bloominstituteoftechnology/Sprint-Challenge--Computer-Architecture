class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0); // register R0-R7
    this.PC = 0; // program counter - holds the address of the currently executed instruction
  }

  /* Store a byte of data in the memory address */
  writeToRAM(address, value) {
    this.ram.write(address, value);
  }

  /* Start the clock on the CPU */
  startClock() {
    this.clock = setInterval(() => this.tick(), 1);
  }

  /* Stop the clock. Halt everything. */
  stopClock() {
    clearInterval(this.clock);
  }

  /* ARITHMETIC LOGIC UNIT - the part of the CPU that strictly handles basic arithmetic and comparisions */
  ALU(operation, regA, regB) {}

  /* Advances the CPU one cycle */
  tick() {}
}

module.exports = CPU;