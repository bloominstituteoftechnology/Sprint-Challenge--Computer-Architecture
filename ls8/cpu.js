class CPU {
  constructor(ram) {
    this.mem = ram;
    this.reg = Array(8).fill(0);

    this.reg.PC = 0;
  }
  poke(address, value) {
    this.ram.write(address, value);
  }
  startClock() {
    this.clock = setInterval(() => {
      this.tick();
    }, 1);
  }
  stopClock() {
    clearInterval(this.clock);
  }
  alu(op, regA, regB) {
    switch (op) {
      case 'MUL':
        this.reg[regA] = this.reg[regA] * this.reg[regB];
        break;
    }
  }
  tick() {
    let IR = this.ram.read(this.reg.PC);

    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);
  }
}

module.exports = CPU;
