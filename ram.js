class RAM {
  constructor(size) {
    this.memory = new Array(size).fill(0);
  }

  write(MAR, MDR) {
    // MAR - Memory Address Register - holds the memory address for RAM
    // MDR - Memory Data Register - holds the value to store in RAM
    this.memory[MAR] = MDR;
  }

  read(MAR) {
    return this.memory[MAR];
  }
}

module.exports = RAM;