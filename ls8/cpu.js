const HLT = 0b00000001;
const LDI = 0b10011001;
const MUL = 0b10101010;
const PRN = 0b01000011;

class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0);

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

    switch (IR) {
      case LDI:
        this.reg[operandA] = operandB;
        break;
      case MUL:
        this.alu('MUL', operandA, operandB);
        break;
      case PRN:
        console.log(this.reg[operandA]);
        break;
      case HLT:
        this.stopClock();
        break;
      default:
        console.log(`Invalid instruction: ${IR.toString(2)}`);
    }

    let operandCount = (IR >>> 6) & 0b11;
    let instructionLength = operandCount + 1;
    this.reg.PC += instructionLength;
  }
}

module.exports = CPU;
