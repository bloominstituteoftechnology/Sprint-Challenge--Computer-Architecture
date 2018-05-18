/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Constants
 */
// implemented

const ADD = 0b10101000; // 2 operands, ALU OP
const AND = 0b10110011; // 2 operands, ALU OP
const CALL = 0b01001000; // 1 operand
const CMP = 0b10100000; // 2 operands, ALU OP
const DEC = 0b01111001; // 1 operand, ALU OP
const DIV = 0b10101011; // 2 operands, ALU OP
const HLT = 0b00000001; // 0 operand
const INC = 0b01111000; // 1 operands, ALU OP
const INT = 0b01001010; // 1 operand,
const IRET = 0b00001011; // 0 operand
const IM = 0b00000101; // Interrupt Mask
const IS = 0b00000110; // Interrupt Status
const IV_Table = [
  0b11111000,
  0b11111001,
  0b11111010,
  0b11111011,
  0b11111100,
  0b11111101,
  0b11111110,
  0b11111111,
];
const JEQ = 0b01010001; // 1 operand
const JGT = 0b01010100; // 1 operand
const JLT = 0b01010011; // 1 operand
const JMP = 0b01010000; // 1 operand
const JNE = 0b01010010; // 1 operand
const LD = 0b10011000; // 2 operands
const MOD = 0b10101100; // 2 operands, ALU OP
const LDI = 0b10011001; // 2 operands
const MUL = 0b10101010; // 2 operands, ALU OP
const NOP = 0b00000000; // 0 operands
const POP = 0b01001100; // 1 operand
const PRA = 0b01000010; // 1 operand
const PRN = 0b01000011; // 1 operand
const PUSH = 0b01001101; // 1 operand
const RET = 0b00001001; // 0 operand
const SP = 0b00000111; // Stack Pointer
const ST = 0b10011010; // 2 operands
const SUB = 0b10101001; // 2 operands, ALU OP

// TBD implemented
const NOT = 0b01110000; // 1 operand, ALU OP
const OR = 0b10110001; // 2 operands, ALU OP
const XOR = 0b10110010; // 2 operands, ALU OP

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

    // Special-purpose registers
    this.PC = 0b00000000; // Program Counter
    this.reg[SP] = 0b11110100; // stack pointer
    this.FL = 0b00000000; // Flags
    this.interrupt = false; // skip the interrupt implementation for now

    // init variables
    this.ALU = 0; // not an ALU OP
    this.handler = undefined; // opcode is not defined yet

    // load the function handlers
    this.branchTable = [];
    this.branchTable[ADD] = this.handle_ADD;
    this.branchTable[AND] = this.handle_AND;
    this.branchTable[CALL] = this.handle_CALL;
    this.branchTable[CMP] = this.handle_CMP;
    this.branchTable[DEC] = this.handle_DEC;
    this.branchTable[DIV] = this.handle_DIV;
    this.branchTable[HLT] = this.handle_HLT;
    this.branchTable[INC] = this.handle_INC;
    this.branchTable[INT] = this.handle_INT;
    this.branchTable[IRET] = this.handle_IRET;
    this.branchTable[JEQ] = this.handle_JEQ;
    this.branchTable[JGT] = this.handle_JGT;
    this.branchTable[JLT] = this.handle_JLT;
    this.branchTable[JMP] = this.handle_JMP;
    this.branchTable[JNE] = this.handle_JNE;
    this.branchTable[LD] = this.handle_LD;
    this.branchTable[LDI] = this.handle_LDI;
    this.branchTable[MUL] = this.handle_MUL;
    this.branchTable[NOP] = this.handle_NOP;
    this.branchTable[POP] = this.handle_POP;
    this.branchTable[PRN] = this.handle_PRN;
    this.branchTable[PRA] = this.handle_PRA;
    this.branchTable[PUSH] = this.handle_PUSH;
    this.branchTable[RET] = this.handle_RET;
    this.branchTable[ST] = this.handle_ST;
    this.branchTable[SUB] = this.handle_SUB;
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
    // doesn't seem necessary but implemented anyways
    this.handler = this.branchTable[op];
    this.handler(regA, regB);
  }

  /**
   * Handles the interrupt sequence operations
   */
  check_INT(operandA, operandB) {
    // The IM register is bitwise AND-ed with the IS register
    const maskedInterrupts = this.reg[IM] & this.reg[IS];

    const bite = [0, 0, 0, 0, 0, 0, 0, 0];
    for (let i = 0; i < 8; i++) {
      let temp = bite;
      temp[7 - i] = 1;
      // check each Interrupt vector
      let result = maskedInterrupts & temp;
      if (result) {
        // disable further interrupts
        this.interrupt = false;
        // clear the IS status
        this.reg[IS] = 0b00000000;

        // PC is pushed onto the stack
        this.push(this.PC);

        // FL is pushed onto the stack
        this.push(this.FL);

        //RO-R6 are push onto the stack
        for (let r = 0; r <= 6; r++) {
          this.push(this.reg[r]);
        }

        // Interrupt vector is lloked up from the interrupt vector
        this.PC = IV_Table[i - 1];
      }
      temp[7 - i] = 0; // clear the position
    }
  }

  /**
   * Handles the ADD operations
   */
  handle_ADD(operandA, operandB) {
    this.reg[operandA] += this.reg[operandB];
  }

  /**
   * Handles the AND operations
   */
  handle_AND(operandA, operandB) {
    this.reg[operandA] &= this.reg[operandB];
  }

  /**
   * Handles the CALL operations
   */
  handle_CALL(operandA) {
    this.push(this.PC + 2);
    this.PC = this.reg[operandA];
  }

  /**
   * Handles the CMP operations
   */
  handle_CMP(operandA, operandB) {
    if (this.reg[operandA] < this.reg[operandB]) {
      //less than
      this.FL = 0b00000100;
    } else if (this.reg[operandA] > this.reg[operandB]) {
      // greater than
      this.FL = 0b00000010;
    } else if (this.reg[operandA] === this.reg[operandB]) {
      // equal
      this.FL = 0b00000001;
    } else {
      // nothing
      this.FL = 0b00000001;
    }
  }

  /**
   * Handles the DIV operations
   */
  handle_DEC(operandA) {
    this.reg[operandA] -= 1;
  }

  /**
   * Handles the DIV operations
   */
  handle_DIV(operandA, operandB) {
    this.reg[operandA] /= this.reg[operandB];
  }

  /**
   * Handles the HLT operations
   */
  handle_HLT() {
    console.log(`execution done!`);
    this.stopClock();
  }

  /**
   * Handles the INC operations
   */
  handle_INC(operandA) {
    this.reg[operandA] += 1;
  }

  /**
   * Handles the INT operations
   */
  handle_INT(operandA) {
    this.reg[IS] = operandA;
  }

  /**
   * Handles the IRET operations
   */
  handle_IRET(operandA, operandB) {
    //R6-R0 are popped off the stack
    for (let i = 6; i >= 0; i--) {
      this.reg[i] = this.pop();
    }

    // FL is popped of the stack
    this.FL = this.pop();

    // return address is popped of the stack
    this.PC = this.pop();

    this.interrupt = true;
  }

  /**
   * Handles the JEQ operations
   */
  handle_JEQ(operandA) {
    if (this.FL === 0b00000001) {
      this.PC = this.reg[operandA];
    }
  }

  /**
   * Handles the JGT operations
   */
  handle_JGT(operandA) {
    if (this.FL === 0b00000010) {
      this.PC = this.reg[operandA];
    }
  }

  /**
   * Handles the JLT operations
   */
  handle_JLT(operandA) {
    if (this.FL === 0b00000100) {
      this.PC = this.reg[operandA];
    }
  }

  /**
   * Handles the JMP operations
   */
  handle_JMP(operandA) {
    this.PC = this.reg[operandA];
  }

  /**
   * Handles the JNE operations
   */
  handle_JNE(operandA) {
    if (this.FL != 0b00000001) {
      this.PC = this.reg[operandA];
    }
  }

  /**
   * Handles the LD operations
   */
  handle_LD(operandA, operandB) {
    this.reg[operandA] = this.peek(this.reg[operandB]);
  }

  /**
   * Handles the LDI operations
   */
  handle_LDI(operandA, operandB) {
    this.reg[operandA] = operandB;
  }

  /**
   * Handles the MUL operations
   */
  handle_MUL(operandA, operandB) {
    this.reg[operandA] *= this.reg[operandB];
  }

  /**
   * Handles the NOP operations
   */
  handle_NOP() {
    // does nothing
  }

  /**
   * Handles the POP operations
   */
  handle_POP(operandA) {
    this.reg[operandA] = this.pop();
  }

  /**
   * Handles the PRA operations
   */
  handle_PRA(operandA) {
    console.log(String.fromCharCode(this.reg[this.operandA]));
  }

  /**
   * Handles the PRN operations
   */
  handle_PRN(operandA) {
    console.log(this.reg[operandA]);
  }

  /**
   * Handles the PUSH operations
   */
  handle_PUSH(operandA) {
    this.push(this.reg[operandA]);
  }

  /**
   * Handles the RET operations
   */
  handle_RET(operandA) {
    this.PC = this.pop();
  }

  /**
   * Handles the ST operations
   */
  handle_ST(operandA, operandB) {
    this.poke(this.reg[operandA], this.reg[operandB]);
  }

  /**
   * Handles the SUB operations
   */
  handle_SUB(operandA, operandB) {
    this.reg[operandA] -= this.reg[operandB];
  }

  /**
   * read value in memory address
   */
  peek(address) {
    return this.ram.read(address);
  }

  /**
   * Store value in memory address, useful for program loading
   */
  poke(address, value) {
    this.ram.write(address, value);
  }

  /**
   * pop method
   */
  pop() {
    // make sure we don't pop anything above 0xf3
    if (this.reg[SP] > 0xf3) {
      // don't do anything because the stack is empty

      return 0;
    } else {
      const result = this.peek(this.reg[SP]);
      this.reg[SP]++;
      return result;
    }
  }

  /**
   * push
   */
  push(item) {
    // decrement
    this.reg[SP]--;

    // write to ram
    this.poke(this.reg[SP], item);
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
   * Advances the CPU one cycle
   */
  tick() {
    // this checks the interrupts, only if the
    if (this.interrupt) {
      this.check_INT();
    }

    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the instruction that's about to be executed
    // right now.)

    const IR = this.peek(this.PC);

    // Get the two bytes in memory _after_ the PC in case the instruction
    // needs them.
    const operandA = this.ram.read(this.PC + 1);
    const operandB = this.ram.read(this.PC + 2);

    // Debugging output
    /*
    console.log(`${this.PC}: ${IR.toString(2)}`);
    console.log(`opA: ${operandA.toString(2)}`);
    console.log(`opB: ${operandB.toString(2)}`);
    console.log('at rams:', this.reg[SP], this.peek(this.reg[SP]));
    // // print out the stack
    for (let i = 0b11110011; i >= this.reg[SP]; i--) {
      console.log(`@ram ${i} is ${this.peek(i)}`);
    }
    console.log(`=====>: ${this.reg} <======`);
    console.log('=============================');
    */

    // Execute the instruction. Perform the actions for the instruction as
    // outlined in the LS-8 spec.

    this.handler = this.branchTable[IR];
    // do a check before moving on
    if (this.handler === undefined) {
      console.log(
        `Instructions Registration error at: ${this.PC}: ${IR.toString(2)}`
      );
      this.stopClock();
      return;
    }
    // move on if defined!

    // determine if its ALU OP or not by first right shifting by 5 and then masking
    if ((IR >> 5) & 0b00000001) {
      this.alu(IR, operandA, operandB);
    } else {
      this.handler(operandA, operandB);
    }

    // Increment the PC register to go to the next instruction. Instructions
    // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
    // instruction byte tells you how many bytes follow the instruction byte
    // for any particular instruction.
    if (IR !== CALL && IR !== RET && IR !== JMP && IR !== INT && IR !== IRET) {
      this.PC += (IR >> 6) + 1;
    }
  }
}

module.exports = CPU;
