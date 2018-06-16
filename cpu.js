/**
 * LS-8 v2.0 emulator skeleton code
 */

// Add the CMP instruction and equal flag to your LS-8.

// Add the JMP instruction.

// Add the JEQ and JNE instructions.

const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;

const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

const FL_E = 0b00000001;
const FL_G = 0b00000010;
const FL_L = 0b00000100;
const FL_X = 0b00000111;

const SP = 7;

// Class for simulating a simple Computer (CPU & memory)
 
class CPU {     // Initialize the CPU
  constructor(ram) {
    this.ram = ram;
    
    this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
    
    this.reg[SP] = 0xf4; // Start of Stack
    this.PC = 0; // Program Counter
    
    this.reg.FL = 0;
    this.flag = 0;
  }

 // Store value in memory address, useful for program loading
  
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

  /**
   * ALU functionality
   *
   * The ALU is responsible for math and comparisons.
   *
   * If you have an instruction that does math, i.e. MUL, the CPU would hand
   * it off to it's internal ALU component to do the actual work.
   *
   * op can be: ADD SUB MUL DIV INC DEC CMP
   */
  alu(op, regA, regB) {
    switch (op) {
      case "MUL":
        this.reg[regA] = this.reg[regA] * this.reg[regB];
        break;
      case "ADD":
        this.reg[regA] += this.reg[regB];
        break;
      case "CMP":
        if (this.reg[regA] === this.reg[regB]) this.reg.FL = FL_E;
        if (this.reg[regA] < this.reg[regB]) this.reg.FL = FL_L;
        if (this.reg[regA] > this.reg[regB]) this.reg.FL = FL_G;
        break;
      default:
        console.log("Error!");
        break;
    }
  }

  tick() {
    let IR = this.ram.read(this.PC);

    //console.log(`${this.PC}: ${IR.toString(2)}`);

    let operandA = this.ram.read(this.PC + 1);
    let operandB = this.ram.read(this.PC + 2);

    switch (IR) {
      case LDI:
        this.reg[operandA] = operandB;
        break;

      case PRN:
        console.log(this.reg[operandA]);
        break;

      case MUL:
        this.alu("MUL", operandA, operandB);
        break;

      case HLT:
        this.stopClock();
        break;

      case PUSH:
        this.reg[SP]--;
        this.ram.write(this.reg[SP], this.reg[operandA]);
        break;

      case POP:
        this.reg[operandA] = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        break;

      case CMP:
        this.alu("CMP", operandA, operandB);
        break;

      case JMP:
        this.PC = this.reg[operandA];
        this.flag = 1;
        break;

      case JEQ:
        if (this.reg.FL === FL_E) {
          this.PC = this.reg[operandA];
          this.flag = 1;
        }
        break;
        
        case JNE:
        if (this.reg.FL !== FL_E) {
          this.PC = this.reg[operandA];
          this.flag = 1;
        }
        break;

      default:
        console.log("invalid instruction: " + IR.toString(2));
        this.stopClock();
    }

    if (this.flag === 0) {
      let instructionLen = (IR >> 6) + 1;
      this.PC += instructionLen;
    }
    this.flag = 0;
  }
}

module.exports = CPU;