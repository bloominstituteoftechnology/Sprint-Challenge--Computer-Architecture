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
    // write the value in the MDR to the address MAR
    this.mem[MAR] = MDR;
    return MDR;
  }

  /**
   * Read (load) MDR value from address MAR
   *
   * @returns MDR
   */
  read(MAR) {
    // Read the value in address MAR and return it
    let MDR = this.mem[MAR];
    return MDR;
  }
}

module.exports = RAM;
