/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

const HLT = 0b00000001;
const LDI = 0b10011001;
const PRN = 0b01000011;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const CALL = 0b01001000;
const RET = 0b00001001;
const ADD = 0b10101000;
const ST = 0b10011010;
const PRA = 0b01000010;
const IRET = 0b00001011;
const JMP = 0b01010000;
const CMP = 0b10100000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

class CPU {
  /**
   * Initialize the CPU
   */
  constructor(ram) {
    this.ram = ram;

    this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

    // Special-purpose registers
    this.reg.PC = 0; // Program Counter
    this.reg[7] = this.ram.read(Number((0xf4).toString()));
    this.reg.FL = 0b00000000; //flag
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
    const _this = this;

    this.clock = setInterval(() => {
      this.intTimer();
      _this.tick();
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
        this.reg[regA] = this.reg[regA] * this.reg[regB];
        break;
      case "ADD":
        this.reg[regA] = this.reg[regA] + this.reg[regB];
        break;
    }
  }

  intTimer() {
    if (this.reg[5]) {
      console.log("Timer interrupted");
      this.reg[6] = 0b00000001;
      let maskedInterrupts = this.reg[5] & this.reg[6];
      for (let i = 0; i < 8; i++) {
        let interruptHappened = ((maskedInterrupts >> i) & 1) == 1;
        if (interruptHappened) {
          // console.log('Interrupt happened at index: ', i);
          // console.log('R6', this.reg[6]);
          this.reg[5] = 0; // disable further interrupts
          this.reg[6] = 0; // reset IS register
          // PC TO STACK
          this.reg[7] -= 1;
          this.ram.write(this.reg[7], this.reg.PC);
          // console.log('PC address before:', this.reg.PC);
          // FL register??
          // add registers R0-R6 to stack
          for (let i = 0; i <= 6; i++) {
            this.reg[7] -= 1;
            this.ram.write(this.reg[7], this.reg[i]);
            // console.log(`Register${i}: ${this.reg[i]}`)
          }
          this.reg.PC = this.ram.read(0xf8);
        } else {
          return;
        }
      }
    }
  }

  /**
   * Advances the CPU one cycle
   */
  tick() {
    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the next instruction.)

    let IR = this.ram.read(this.reg.PC);
    let callHandler;
    let interruptHandler;
    let JMPhandler;
    let JEQhandler;
    let JNEhandler;

    let operandA = this.ram.read(this.reg.PC + 1);
    let operandB = this.ram.read(this.reg.PC + 2);

    const handle_HLT = () => {
      this.stopClock();
    };

    const handle_LDI = (index, value) => {
      this.reg[index] = value;
    };

    const handle_PRN = index => {
      console.log(this.reg[index]);
    };

    const handle_MUL = () => {
      this.alu("MUL", operandA, operandB);
    };

    const handle_PUSH = () => {
      this.reg[7] -= 1;
      this.ram.write(this.reg[7], this.reg[operandA]);
    };

    const handle_POP = () => {
      this.reg[operandA] = this.ram.read(this.reg[7]);
      this.reg[7] += 1;
    };
    const handle_RET = () => {
      callHandler = this.ram.read(this.reg[7]);
      this.reg[7] += 1;
      return callHandler;
    };
    const handle_CALL = () => {
      this.reg[7] -= 1;
      this.ram.write(this.reg[7], this.reg.PC + (IR >>> 6) + 1);
      callHandler = this.reg[operandA];
      return callHandler;
    };
    const handle_ADD = () => {
      this.alu("ADD", operandA, operandB);
    };

    const handle_ST = () => {
      this.ram.write(this.reg[operandA], this.reg[operandB]);
    };

    const handle_PRA = () => {
      console.log(String.fromCharCode(this.reg[operandA]));
    };

    const handle_IRET = () => {
      for (let i = 6; i >= 0; i--) {
        this.reg[i] = this.ram.read(this.reg[7]);
        this.reg[7] += 1;
        // console.log(`Register${i} after: ${this.reg[i]}`)
      }
      interruptHandler = this.ram.read(this.reg[7]);
      // console.log('PC After interrupt', this.reg.PC);
    };

    const handle_JMP = () => {
      // console.log('JMP', this.reg[operandA]);
      JMPhandler = this.reg[operandA];
    };

    const handle_CMP = () => {
      let valA = this.reg[operandA];
      let valB = this.reg[operandB];

      if (valA === valB) {
        this.reg.FL |= 0b00000001;
      } else if (valA < valB) {
        this.reg.FL |= 0b00000100;
      } else if (valA > valB) {
        this.reg.FL |= 0b00000010; 
      }
    }

    const handle_JEQ = () => {
      if (this.reg.FL & 0b00000001) {
        JEQhandler = this.reg[operandA];
      } else {
        return;
      }
    }

    const handle_JNE = () => {
      if (!(this.reg.FL & 0b00000001)) {
        JNEhandler = this.reg[operandA];
      } else {
        return;
      }
    }

    const branchTable = {
      [LDI]: handle_LDI,
      [PRN]: handle_PRN,
      [HLT]: handle_HLT,
      [MUL]: handle_MUL,
      [POP]: handle_POP,
      [PUSH]: handle_PUSH,
      [CALL]: handle_CALL,
      [RET]: handle_RET,
      [ADD]: handle_ADD,
      [ST]: handle_ST,
      [PRA]: handle_PRA,
      [IRET]: handle_IRET,
      [JMP]: handle_JMP,
      [CMP]: handle_CMP,
      [JEQ]: handle_JEQ,
      [JNE]: handle_JNE
    };

    branchTable[IR](operandA, operandB);
    
    if (interruptHandler) {
      this.reg.PC = interruptHandler;
    } else if (callHandler) {
      this.reg.PC = callHandler;
    } else if (JMPhandler) {
      this.reg.PC = JMPhandler;
    } else if (JEQhandler) {
      this.reg.PC = JEQhandler; 
    } else if (JNEhandler) {
      this.reg.PC = JNEhandler;
    } else {
      this.reg.PC += (IR >>> 6) + 1;
    }
  }
}

module.exports = CPU;
