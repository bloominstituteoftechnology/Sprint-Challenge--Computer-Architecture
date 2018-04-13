const SP = 0b00000111;
let subroutine = false;

class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0);
    this.reg[SP] = 0xf4;
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

  error() {
    console.log("ERROR! Improper argument length");
    this.stopClock();
  }

  alu(op, regA, regB) {
    switch (op) {
      case "MUL":
        this.reg[regA] *= this.reg[regB];
        break;
      case "ADD":
        this.reg[regA] += this.reg[regB];
        break;
      default:
        console.log("There was an error in the ALU...command not found");
        break;
    }
  }

  tick() {
    const ADD = 0b10101000;
    const CALL = 0b01001000;
    const HLT = 0b00000001;
    const LDI = 0b10011001;
    const MUL = 0b10101010;
    const POP = 0b01001100;
    const PRN = 0b01000011;
    const PUSH = 0b01001101;
    const RET = 0b00001001;

    const IR = this.reg.PC;
    let operandA = this.ram.read(IR + 1);
    let operandB = this.ram.read(IR + 2);

    switch (this.ram.read(IR)) {
      case ADD:
        this.alu("ADD", operandA, operandB);
        break;
      case CALL:
        this.reg[SP] -= 1;
        this.ram.write(this.reg[SP], this.reg.PC + 1);
        this.reg.PC = this.reg[1];
        subroutine = true;
        break;
      case HLT:
        this.stopClock();
        break;
      case LDI:
        this.reg[operandA] = operandB;
        break;
      case MUL:
        this.alu("MUL", operandA, operandB);
        break;
      case POP:
        this.reg[operandA] = this.ram.read(this.reg[SP]);
        this.reg[SP] += 1;
        break;
      case PRN:
        console.log(this.reg[operandA]);
        break;
      case PUSH:
        this.reg[SP] -= 1;
        this.ram.write(this.reg[SP], this.reg[operandA]);
        break;
      case RET:
        this.reg.PC = this.ram.read(this.reg[SP]);
        this.reg[SP] += 1;
        break;
      default:
        console.log(`Error at position: ${IR}, code: ${this.ram.read(IR)}`);
        this.stopClock();
    }
    if (!subroutine) {
      this.reg.PC += 1;
      const num = this.ram.read(IR) >>> 6;
      this.reg.PC += num;
    } else {
      subroutine = false;
    }
  }
}

module.exports = CPU;
