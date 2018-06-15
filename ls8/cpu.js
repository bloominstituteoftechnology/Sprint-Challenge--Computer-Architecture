/**
 * LS-8 v2.0 emulator skeleton code
 */

// Instruction set vars
// 0b tells js this is a binary number
const ADD = 0b10101000;
const CALL = 0b01001000;
const CMP = 0b10100000;
const HLT = 0b00000001;
const JEQ = 0b01010001;
const JMP = 0b01010000;
const JNE = 0b01010010;
const LDI = 0b10011001;
const MUL = 0b10101010;
const POP = 0b01001100;
const PRN = 0b01000011;
const PUSH = 0b01001101;
const RET = 0b00001001;
const SP = 7;

/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {
  /**
   * Initialize the CPU
   */
  constructor(ram) {
    this.ram = ram;

    this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
    this.reg[SP] = 0xf4;
    // this.reg[FL] = 0;

    // Special-purpose registers
    this.PC = 0; // Program Counter
    this.FL = 0b00000000;
  }

  /**
   * Store value in memory address, useful for program loading
   */
  poke(address, value) {
    this.ram.write(address, value);
  }

  /**
   * Starts the clock ticking on the CPU
   */
  startClock() {
    this.clock = setInterval(() => {
      this.tick();
    }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
  }

  /**
   * Stops the clock
   */
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
        // !!! IMPLEMENT ME
        this.reg[regA] *= this.reg[regB]; // Multiply both registers and store in regA
        // this.PC += 3; // next instruction
        break;

      case "ADD":
        this.reg[regA] += this.reg[regB]; // Add both registers and store in regA
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the instruction that's about to be executed
    // right now.)

    // !!! IMPLEMENT ME
    let IR = this.ram.read(this.PC);

    // Debugging output
    //console.log(`${this.PC}: ${IR.toString(2)}`);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.

    // !!! IMPLEMENT ME
    let operandA = this.ram.read(this.PC + 1); // Register Number! (Index for register array)
    let operandB = this.ram.read(this.PC + 2); // Immediate Value!

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    // !!! IMPLEMENT ME
    switch (IR) {
      case ADD:
        // Add two registers and store in register A
        // ADD - R0 - R1
        // 10101000 00000aaa 00000bbb
        this.alu("ADD", operandA, operandB);
        break;

      case CALL:
        // Call subroutine at address stored in register
        // CALL - Register Number
        // 01001000 00000rrr
        this.reg[SP]--; // decrement SP (next inst)
        this.poke(this.reg[SP], this.PC + 2); // address of next int pushed to stack
        this.PC = this.reg[operandA]; // PC set to address in registerA
        break;

      case CMP:
        // Compare value in two registers
        // Equal -> flag E1, R0 < R1 -> flag L1, R0 > R1 -> flag G1
        // CMP - R0 - R1
        // 10100000 00000aaa 00000bbb
        if (this.reg[operandA] === this.reg[operandB]) {
          this.FL = 0b00000001;
        }
        if (this.reg[operandA] < this.reg[operandB]) {
          this.FL = 0b00000100;
        }
        if (this.reg[operandA] > this.reg[operandB]) {
          this.FL = 0b00000010;
        }
        //console.log(this.FL);
        //console.log(this.PC);
        break;

      case HLT:
        // Halt and quit
        // HLT
        // 00000001
        this.stopClock(); // stop function
        // this.PC += 1;
        break;

      case JEQ:
        // If equal flag = 1, jump to address stored in given register
        // JEQ - Register Number
        // 01010001 00000rrr
        if ((this.FL = 0b00000001)) {
          this.PC = this.reg[operandA];
        } 
        //console.log(this.PC);
        break;

      case JMP:
        // Jump to address stored in given register
        // JMP - Register Number
        // 01010000 00000rrr
        this.PC = this.reg[operandA]; // set PC to address in given register
        break;

      case JNE:
        // If equal flag = 0, jump to address stored in given register
        // JNE - Register Number
        // 01010010 00000rrr
        if ((this.FL = 0b00000000)) {
          this.PC = this.reg[operandA];
        }
        break;

      case LDI:
        // set the value in a register (R0)
        // LDI - Register Number - Immediate Value
        // 10011001 00000rrr iiiiiiii
        this.reg[operandA] = operandB; // register array[register number] = value
        // this.PC += 3; // next instruction
        // console.log(this.PC);
        break;

      case MUL:
        // Multiply R0*R1, storing result in R0
        // MUL - R0 - R1
        // 10101010 00000aaa 00000bbb
        this.alu("MUL", operandA, operandB); // Pass to alu()
        break;

      case POP:
        // Pop the value at the top of the stack into the given register.
        // POP - Register Number
        // 01001100 00000rrr
        this.reg[operandA] = this.ram.read(this.reg[SP]); // Copy value from SP address to register
        this.reg[SP]++; // increment SP
        break;

      case PRN:
        // Print value to register (R0)
        // PRN - Register Number
        // 01000011 0000rrr
        console.log(this.reg[operandA]); // should print 8
        // this.PC += 2; // next instruction
        break;

      case PUSH:
        // Push the given register on the stack.
        // PUSH - Register Number
        // 01001101 00000rrr
        this.reg[SP]--; // decrement SP
        this.poke(this.reg[SP], this.reg[operandA]); // copy value from register to SP address
        break;

      case RET:
        // Return from subroutine
        // RET
        // 00001001
        this.PC = this.ram.read(this.reg[SP]); //
        this.reg[SP]++; // increment SP
        break;

      default:
        console.log("Unknown instruction: " + IR.toString(2));
        this.stopClock(); // stop function
        return;
    }

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.

    // !!! IMPLEMENT ME
    if (IR !== CALL && IR !== JEQ) {
      const instLen = (IR >> 6) + 1; // operands + opcode
      this.PC += instLen;
    }
  }
}

module.exports = CPU;
