


class Ram {

  constructor() {
    this.memo = new Array().fill(0)

  }

  write(MAR, MDR) {
    this.memo[MAR] = MDR;

  }
  read(MAR) {
    return this.memo[MAR];

  }
}
module.exports = Ram;