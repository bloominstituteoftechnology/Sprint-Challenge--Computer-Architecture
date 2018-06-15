const Ram = require('./ram.js');

const SP = 7;
const FL = 4;
const EQUAL = 1;
const GREATER = 2;
const LESS = 4;
const instructions = {
  LDI: 153,
  CMP: 160,
  JEQ: 81,
  PRN: 67,
  JNE: 82,
  JPM: 80,
  HLT: 1,
}

class CPU {
  constructor(Ram) {
    this.ram = Ram
    this.reg = new Array(8).fill(0)
    this.PC = 0
    this.reg[SP] = 0xf4;
    this.reg[FL] = 0b0000000;
  }

  startClock() {
    this.clock = setInterval(() => {
      this.tick()
    }, 1)
  }
  stopClock() {
    clearInterval(this.clock)
  }
  pock(address, value) {
    this.ram.write(address, value)
  }


  ldi(operandA, operandB) {

    this.reg[operandA] = operandB

  }

  cmp(operandA, operandB) {

    if (this.reg[operandA] === this.reg[operandB]) {
      this.reg[FL] = EQUAL
    }
    else if (this.reg[operandA] > this.reg[operandB]) {
      this.reg[FL] = GREATER
    }
    else {
      if (this.reg[operandA] < this.reg[operandB]) {
        this.reg[FL] = LESS
      }
    }

  }
  jeq(operandA) {

    this.PC = this.reg[operandA]
    this.pcAdvance = false

  }
  prn(operandA) {

    console.log(this.reg[operandA])
  }
  jne(operandA) {
    this.PC = this.reg[operandA]
    this.pcAdvance = false;

  }
  jpm(operandA) {
    this.PC = this.reg[operandA]
    this.pcAdvance = false

  }

  tick() {
    const IR = this.ram.read(this.PC);
    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);
    this.pcAdvance = true;


    switch (IR) {
      case instructions.LDI:
        this.ldi(operandA, operandB)
        break;

      case instructions.CMP:
        this.cmp(operandA, operandB)
        break;
      case instructions.JEQ:
        if (this.reg[FL] === EQUAL) {
          this.jeq(operandA)
        }
        break;
      case instructions.PRN:
        this.prn(operandA)
        break;
      case instructions.JNE:
        if (this.reg[FL] !== EQUAL) {
          this.jne(operandA)
        }
        break;
      case instructions.JPM:
        this.jpm(operandA)
        break;
      case instructions.HLT:
        this.stopClock()
        break;
      default:
        console.log('unknown instruction:' + IR);
        this.stopClock()
        return;
    }
    if (this.pcAdvance) {
      const instructionsLength = (IR >> 6) + 1;
      this.PC += instructionsLength

    }

  }
}
module.exports = CPU
