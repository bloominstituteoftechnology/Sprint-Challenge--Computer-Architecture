const HLT = 0b00000001;
const LDI = 0b10011001;
const MUL = 0b10101010;
const ADD = 0b10101000;
const SUB = 0b10101001;
const DIV = 0b10101011;
const MOD = 0b10101100;
const INC = 0b01111000;
const DEC = 0b01111001;
const AND = 0b10110011;
const NOT = 0b01110000;
const OR = 0b10110001;
const XOR = 0b10110010;
const PRN = 0b01000011;
const LD = 0b10011000;
const NOP = 0b00000000;
const PRA = 0b01000010;
const ST = 0b10011010;
const POP = 0b01001100;
const PUSH = 0b01001101;
const CALL = 0b01001000;

const SP = 7;

class CPU {
  constructor(ram) {
    this.ram = ram;
    this.reg = new Array(8).fill(0);

    this.reg.PC = 0;
    this.reg[SP] = 0xf4;
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
      case 'ADD':
        this.reg[regA] = this.reg[regA] + this.reg[regB];
        break;
      case 'SUB':
        this.reg[regA] = this.reg[regA] - this.reg[regB];
        break;
      case 'DIV':
        if (this.reg[regB] === 0) {
          console.error('Divider cannot be zero', this.reg[regB]);
          this.stopClock();
        }
        this.reg[regA] = this.reg[regA] / this.reg[regB];
        break;
      case 'MOD':
        if (this.reg[regB] === 0) {
          console.error('Divider cannot be zero', this.reg[regB]);
          this.stopClock();
        }
        this.reg[regA] = this.reg[regA] % this.reg[regB];
        break;
      case 'INC':
        this.reg[regA] = (this.reg[regA] + 1) & 0xff;
        break;
      case 'DEC':
        this.reg[regA] = (this.reg[regA] - 1) & 0xff;
        break;
      case 'AND':
        this.reg[regA] = this.reg[regA] & this.reg[regB];
        break;
      case 'NOT':
        this.reg[regA] = ~this.reg[regA];
        break;
      case 'OR':
        this.reg[regA] = this.reg[regA] | this.reg[regB];
        break;
      case 'XOR':
        this.reg[regA] = this.reg[regA] ^ this.reg[regB];
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
      case LD:
        this.reg[operandA] = this.ram.read(this.reg[operandB]);
        break;
      case MUL:
        this.alu('MUL', operandA, operandB);
        break;
      case DIV:
        this.alu('DIV', operandA, operandB)
        break;
      case MOD:
        this.alu('MOD', operandA, operandB);
        break;
      case ADD:
        this.alu('ADD', operandA, operandB);
        break;
      case SUB:
        this.alu('SUB', operandA, operandB);
        break;
      case INC:
        this.alu('INC', operandA);
        break;
      case DEC:
        this.alu('DEC', operandA);
        break;
      case AND:
        this.alu('AND', operandA, operandB);
        break;
      case NOT:
        this.alu('NOT', operandA);
        break;
      case OR:
        this.alu('OR', operandA, operandB);
        break;
      case XOR:
        this.alu('XOR', operandA, operandB);
        break;
      case PRN:
        console.log(this.reg[operandA]);
        break;
      case NOP:
        return undefined;
        break;
      case PRA:
        console.log(String.fromCharCode(this.reg[operandA]));
        break;
      case ST:
        this.ram.write(this.reg[operandA], this.reg[operandB]);
        break;
      case PUSH:
        this.reg[SP]--;
        this.ram.write(this.reg[SP], this.reg[operandA]);
        break;
      case POP:
        this.reg[operandA] = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        break;
      case CALL:
        this.alu('DEC', SP);
        this.reg[operandA] = this.ram.write(this.reg[SP], operandB);
        break;
      case HLT:
        this.stopClock();
        break;
      default:
        console.log(`Invalid instruction: ${IR.toString(2)}`);
        this.stopClock();
    }

    let operandCount = (IR >>> 6) & 0b11;
    let instructionLength = operandCount + 1;
    this.reg.PC += instructionLength;
  }
}

module.exports = CPU;
