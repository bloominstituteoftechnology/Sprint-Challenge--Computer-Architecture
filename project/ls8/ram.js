/**
 * Ram Access
 */
class RAM {
  constructor(size) {
    this.mem = new Array(size);
    this.mem.fill(0);
  }

  write(MAR, MDR) {
    // write the value in MDR to the address MAR

    this.mem[MAR] = MDR;
  }

  read(MAR) {
    //return the MAR location

    return this.mem[MAR];
  }
}

module.exports = RAM;
