class RAM {
  constructor(size) {
    this.mem = Array(size).fill(0);
  }
  write(MAR, MDR) {
    // Write the value in the MDR to the address MAR
    this.mem[MAR] = MDR;
  }
  read(MAR) {
    // Read the address in MAR and return it
    return this.mem.[MAR];
  }
}

module.exports = RAM;
