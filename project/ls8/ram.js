class RAM {
  constructor(size) {
    this.mem = new Array(size);
    this.mem.fill(0);
  }

  write(MAR, MDR) {
    this.mem[MAR] = MDR;
  }

  read(MAR) {
    return this.mem[MAR];
  }
}

module.exports = RAM;
