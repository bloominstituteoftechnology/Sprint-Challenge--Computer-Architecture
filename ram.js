/**
 * RAM access
 */
class RAM {
  constructor(size) {
    this.mem = new Array(size);
    this.mem.fill(0);
  }

  /**
   * Write (store) MDR value at address MAR
   */
  write(MAR, MDR) {
    this.mem[MAR] = MDR;
  }

  /**
   * Read (load) MDR value from address MAR
   * 
   * returns MDR
   */
  read(MAR) {
    return this.mem[MAR];
  }
}

module.exports = RAM;
