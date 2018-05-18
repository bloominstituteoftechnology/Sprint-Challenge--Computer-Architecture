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

/**
 * RAM access
 */

/**
 * Write (store) MDR value at address MAR
 */

// !!! IMPLEMENT ME
// write the value in the MDR to the address MAR

/**
 * Read (load) MDR value from address MAR
 *
 * @returns MDR
 */

// !!! IMPLEMENT ME
// Read the value in address MAR and return it
