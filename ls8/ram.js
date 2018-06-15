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
    // !!! IMPLEMENT ME
    // write the value in the MDR to the address MAR
    // console.log("Write was called. " + "MAR is " + MAR + " and MDR is " + MDR);
    this.mem[MAR] = MDR;
  }

  /**
   * Read (load) MDR value from address MAR
   *
   * @returns MDR
   */
  read(MAR) {
    // !!! IMPLEMENT ME
    // Read the value in address MAR and return it
    // console.log("Read was called. MAR is " + MAR);
    return this.mem[MAR];
  }
}

module.exports = RAM;
