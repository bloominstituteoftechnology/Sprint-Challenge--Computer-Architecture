const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const ADD = 0b10101000;
const RET = 0b000001001;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;
const FL_EQ = 0b00000001;
const FL_GT = 0b00000010;
const FL_LT = 0b00000100;
const SP = 7;

class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0);
    this.reg.PC = 0;
    this.flag = false;
    this.reg.FL = 0;
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
      case "MUL":
        this.reg[regA] *= this.reg[regB];
        break;
      case "ADD":
        this.reg[regA] += this.reg[regB];
        break;
      case "CMP":
        if (this.reg[regA] === this.reg[regB]) this.reg.FL = FL_EQ;
        if (this.reg[regA] > this.reg[regB]) this.reg.FL = FL_GT;
        if (this.reg[regA] < this.reg[regB]) this.reg.FL = FL_LT;
        break;
      default:
        break;
    }
  }

  tick() {
    const IR = this.ram.read(this.reg.PC);

    const operandA = this.ram.read(this.reg.PC + 1);
    const operandB = this.ram.read(this.reg.PC + 2);

    const _push = value => {
      this.reg[SP]--;
      this.ram.write(this.reg[SP], value);
    };

    switch (IR) {
      case LDI:
        this.reg[operandA] = operandB;
        break;
      case PRN:
        console.log(this.reg[operandA]);
        break;
      case HLT:
        this.stopClock();
        break;
      case MUL:
        this.alu("MUL", operandA, operandB);
        break;
      case ADD:
        this.alu("ADD", operandA, operandB);
        break;
      case PUSH:
        if (this.reg[SP] === 0) this.reg[SP] = 0xf4;
        _push(this.reg[operandA]);
        break;
      case POP:
        this.reg[operandA] = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        break;
      case CALL:
        this.flag = true;
        _push(this.reg.PC + 2);
        this.reg.PC = this.reg[operandA];
        break;
      case RET:
        this.flag = true;
        this.reg.PC = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        break;
      case CMP:
        this.alu("CMP", operandA, operandB);
        break;
      case JMP:
        this.flag = true;
        this.reg.PC = this.reg[operandA];
        break;
      case JEQ:
        if (this.reg.FL === FL_EQ) {
          this.flag = true;
          this.reg.PC = this.reg[operandA];
        }
        break;
      case JNE:
        if (this.reg.FL !== FL_EQ) {
          this.flag = true;
          this.reg.PC = this.reg[operandA];
        }
        break;
      default:
        console.log("Unknown Instruction");
        break;
    }

    if (!this.flag) {
      let operandCount = (IR >>> 6) & 0b11;
      let totalInstructionLen = operandCount + 1;
      this.reg.PC += totalInstructionLen;
    }
    this.flag = false;
  }
}

module.exports = CPU;
