class RAM {
  constructor(size) {
    this.memory = new Array(size).fill(0);
  }

  write(MAR, MDR) {
    this.memory[MAR] = MDR;
  }

  read(MAR) {
    return this.memory[MAR];
  }
}

module.exports = RAM;