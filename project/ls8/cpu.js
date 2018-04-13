/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');

// Instructions

const HLT = 0b00000001; // Halt CPU
const LDI = 0b10011001;
const MUL = 0b10101010;
const PRN = 0b01000011;
const ADD = 0b10101000;
const AND = 0b10110011;
const NOP = 0b00000000;
const POP = 0b01001100;
const PUSH = 0b01001101;
const INC = 0b01111000;
const DEC = 0b01111001;
const DIV = 0b10101011;
const CMP = 0b10100000;
const CALL = 0b01001000;
const JEQ = 0b01010001;
const MOD = 0b10101100;
const NOT = 0b01110000;
const OR = 0b10110001;
const SUB = 0b10101001;
const XOR = 0b10110010;
const RET = 0b00001001;
const JGT = 0b01010100;
const JLT = 0b01010011;
const JMP = 0b01010000;
const JNE = 0b01010010;
const LD = 0b10011000;
const PRA = 0b01000010;
const ST = 0b10011010;
const IRET = 0b00001011;

// General registers
const IM = 0b00000101;
const IS = 0b00000110;
const SP = 0b00000111;

// Flag values
const FL_EQ = 0b00000001;
const FL_GT = 0b00000010;
const FL_LT = 0b00000100;

// Interrupt mask
const interruptMask = [
    0b00000001,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000
];
/**
 * Class for simulating a simple Computer (CPU & memory)
 */
class CPU {
    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;
        this.reg = new Array(8).fill(0); // General-purpose registers

        // initialize SP of stack in ram;
        this.reg[SP] = 0xf4;
        // initialize IM and IS
        this.reg[IM] = 0;
        this.reg[IS] = 0;

        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register
        this.reg.FL = 0; // Flag Register

        // initialize flag of interrupt
        this.interruptEnabled = true;

        this.setupBranchTable();
    }

    /**
     * Sets up the branch table
     */
    setupBranchTable() {
        let bt = {};

        bt[HLT] = this.HLT; // Halt CPU
        bt[LDI] = this.LDI; // ADD R R
        bt[MUL] = this.MUL; // MUL R R
        bt[PRN] = this.PRN; // PRN R
        bt[ADD] = this.ADD; // ADD R R
        bt[AND] = this.AND; // AND R R
        bt[NOP] = this.NOP; // NOP
        bt[INC] = this.INC; // INC R
        bt[DEC] = this.DEC; // DEC R
        bt[DIV] = this.DIV; // DIV R R
        bt[CMP] = this.CMP; // CMP R R
        bt[POP] = this.POP; // POP R
        bt[PUSH] = this.PUSH; // PUSH R
        bt[CALL] = this.CALL; // CALL R
        bt[JEQ] = this.JEQ; // JEQ R
        bt[MOD] = this.MOD; // MOD R, R
        bt[NOT] = this.NOT; // NOT R
        bt[OR] = this.OR; // OR R,R
        bt[SUB] = this.SUB; // SUB R,R
        bt[XOR] = this.XOR; // XOR R,R
        bt[RET] = this.RET; // RET
        bt[JGT] = this.JGT; // JGT R
        bt[JLT] = this.JLT; // JLT R
        bt[JMP] = this.JMP; // JMP R
        bt[JNE] = this.JNE; // JNE R
        bt[LD] = this.LD; // LD R,R
        bt[PRA] = this.PRA; // PRA R
        bt[ST] = this.ST; // ST R,R
        bt[IRET] = this.IRET; // IRET

        this.branchTable = bt;
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
            _this.tick();
        }, 1);

        this.interruptClock = setInterval(() => {
            _this.interrupt(0);
        }, 1000);
    }

    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
        clearInterval(this.interruptClock);
    }

    /**
     * Set Flag
     */
    setFlag(flag, value) {
        if (value === true) {
            //Set the Flag
            this.reg.FL = this.reg.FL | flag;
        } else {
            // Clear the Flag to 0
            this.reg.FL = this.reg.FL & ~flag;
        }
    }

    /**
     * ALU functionality
     *
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                this.reg[regA] = this.reg[regA] * this.reg[regB];
                break;
            case 'ADD':
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                break;
            case 'AND':
                this.reg[regA] = this.reg[regA] & this.reg[regB];
                break;
            case 'INC':
                this.reg[regA] = (this.reg[regA] + 1) & 0xff;
                break;
            case 'DEC':
                this.reg[regA] = (this.reg[regA] - 1) & 0xff;
                break;
            case 'DIV':
                if (this.reg[regB] === 0) {
                    console.error('Divider cannot be zero', this.reg[regB]);
                    this.HLT();
                }
                this.reg[regA] = this.reg[regA] / this.reg[regB];
                break;
            case 'CMP':
                this.setFlag(FL_EQ, this.reg[regA] === this.reg[regB]);
                this.setFlag(FL_GT, this.reg[regA] > this.reg[regB]);
                this.setFlag(FL_LT, this.reg[regA] < this.reg[regB]);
                break;
            case 'MOD':
                if (this.reg[regB] === 0) {
                    console.error('Divder cannot be zero', this.reg[regB]);
                    this.HLT();
                }
                this.reg[regA] = this.reg[regA] % this.reg[regB];
                break;
            case 'NOT':
                this.reg[regA] = ~this.reg[regA];
                break;
            case 'OR':
                this.reg[regA] = this.reg[regA] | this.reg[regB];
                break;
            case 'SUB':
                this.reg[regA] = this.reg[regA] - this.reg[regB];
                break;
            case 'XOR':
                this.reg[regA] = this.reg[regA] ^ this.reg[regB];
                break;
        }
    }

    /**
     * Start interrupt
     */
    interrupt(n) {
        this.reg[IS] = this.reg[IS] | interruptMask[n];
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        if (this.interruptEnabled) {
            let interrupts = this.reg[IS] & this.reg[IM];
            for (let i = 0; i < 8; i++) {
                let interruptHappened = ((interrupts >> i) & 1) === 1;
                if (interruptHappened) {
                    this.interruptEnabled = false;
                    this.reg[IS] = this.reg[IS] & ~interruptMask[i];
                    this._push(this.reg.PC);
                    this._push(this.reg.FL);
                    for (let i = 0; i < 6; i++) {
                        this._push(this.reg[i]);
                    }
                    this.reg.PC = this.ram.read(0xf8 + i);
                    break;
                }
            }
        }
        // Load the instruction register (IR) from the current PC
        this.reg.IR = this.ram.read(this.reg.PC);
        // Debugging output
        // console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, locate the
        // appropriate hander in the branchTable
        let offset = (this.reg.IR >> 6) & 0b00000011;

        const operandA = this.ram.read(this.reg.PC + 1);
        const operandB = this.ram.read(this.reg.PC + 2);

        let handler = this.branchTable[this.reg.IR];
        // Check that the handler is defined, halt if not (invalid
        // instruction)
        if (!handler) {
            this.HLT();
            return undefined;
        }

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)
        let nextPC = handler.call(this, operandA, operandB);

        if (nextPC !== undefined) {
            this.reg.PC = nextPC;
        } else {
            // Increment the PC register to go to the next instruction
            this.reg.PC += offset + 1;
        }
    }

    // INSTRUCTION HANDLER CODE:

    /**
     * ADD R R
     */
    ADD(regA, regB) {
        this.alu('ADD', regA, regB);
    }

    /**
     * HLT
     */
    HLT() {
        this.stopClock();
    }

    /**
     * LDI R,I
     */
    LDI(reg, value) {
        this.reg[reg] = value;
    }

    /**
     * MUL R,R
     */
    MUL(regA, regB) {
        // Call the ALU
        this.alu('MUL', regA, regB);
    }

    /**
     * PRN R
     */
    PRN(regA) {
        console.log(this.reg[regA]);
    }

    /**
     * AND R,R
     */
    AND(regA, regB) {
        this.alu('AND', regA, regB);
    }

    /**
     * NOP
     */
    NOP() {
        return undefined;
    }

    /**
     * INC R
     */
    INC(regA) {
        this.alu('INC', regA);
    }

    /**
     * DEC R
     */
    DEC(regA) {
        this.alu('DEC', regA);
    }

    /**
     * DIV R,R
     */
    DIV(regA, regB) {
        this.alu('DIV', regA, regB);
    }

    /**
     * CMP R,R
     */
    CMP(regA, regB) {
        this.alu('CMP', regA, regB);
    }

    _pop() {
        const value = this.ram.read(this.reg[SP]);
        this.alu('INC', SP);
        return value;
    }

    POP(regA) {
        this.reg[regA] = this._pop();
    }

    _push(value) {
        this.alu('DEC', SP);
        this.ram.write(this.reg[SP], value);
    }

    PUSH(regA) {
        this._push(this.reg[regA]);
    }

    JEQ(regA) {
        if ((this.reg.FL & FL_EQ) === 1) {
            return this.reg[regA];
        }
    }

    JGT(regA) {
        if ((this.reg.FL & FL_GT) === 2) {
            return this.reg[regA];
        }
    }

    JLT(regA) {
        if ((this.reg.FL & FL_LT) === 4) {
            return this.reg[regA];
        }
    }

    JMP(regA) {
        return this.reg[regA];
    }

    JNE(regA) {
        if ((this.reg.FL & FL_EQ) === 0) {
            return this.reg[regA];
        }
    }

    CALL(regA) {
        this._push(this.reg.PC + 2);
        return this.reg[regA];
    }

    MOD(regA, regB) {
        this.alu('MOD', regA, regB);
    }

    NOT(regA) {
        this.alu('NOT', regA);
    }

    OR(regA, regB) {
        this.alu('OR', regA, regB);
    }

    SUB(regA, regB) {
        this.alu('SUB', regA, regB);
    }

    XOR(regA, regB) {
        this.alu('XOR', regA, regB);
    }

    RET() {
        return this._pop();
    }

    LD(regA, regB) {
        this.reg[regA] = this.ram.read(this.reg[regB]);
    }

    PRA(regA) {
        const value = this.reg[regA];
        console.log(String.fromCharCode(value));
    }

    ST(regA, regB) {
        const value = this.reg[regB];
        this.ram.write(this.reg[regA], value);
    }

    IRET() {
        for (let i = 0; i < 7; i++) {
            this.reg[i] = this._pop();
        }
        this.reg.FL = this._pop();
        this.interruptEnabled = true;
        const newPC = this._pop();
        return newPC;
    }
}

module.exports = CPU;


// /**
//  * LS-8 v2.0 emulator skeleton code
//  */

// const fs = require('fs');

// // Instructions

// const HLT = 0b00000001; // Halt CPU
// const LDI = 0b10011001;
// const MUL = 0b10101010;
// const PRN = 0b01000011;
// const ADD = 0b10101000;
// const AND = 0b10110011;
// const NOP = 0b00000000;
// const POP = 0b01001100;
// const CALL = 0b01001000;
// const RET = 0b00001001;
// const PUSH = 0b01001101;

// const SP = 7;
// /**
//  * Class for simulating a simple Computer (CPU & memory)
//  */
// class CPU {
//   /**
//    * Initialize the CPU
//    */
//   constructor(ram) {
//     this.ram = ram;

//     this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

//     // Special-purpose registers
//     this.reg.PC = 0; // Program Counter
//     this.reg.IR = 0; // Instruction Register

//     this.setupBranchTable();
//     this.reg[111] = 0xF4;
//   }
//   /**
//    * Set up the branch table
//    */
//   setupBranchTable() {
//     let bt   = {};
//     bt[HLT]  = this.HLT;
//     bt[LDI]  = this.LDI;
//     bt[MUL]  = this.MUL;
//     bt[PRN]  = this.PRN;
//     bt[ADD]  = this.ADD;
//     bt[AND]  = this.AND;
//     bt[NOP]  = this.NOP;
//     bt[PUSH] = this.PUSH;
//     bt[POP]  = this.POP;
//     bt[CALL] = this.CALL;
//     bt[RET]  = this.RET;
//     this.branchTable = bt;
//   }

//   /**
//    * Store value in memory address, useful for program loading
//    */

//   poke(address, value) {
//     this.ram.write(address, value);
//   }

//   /**
//    * Starts the clock ticking on the CPU
//    */
//   startClock() {
//     this.clock = setInterval(() => {
//       this.tick();
//     }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
//   }

//   /**
//    * Stops the clock
//    */
//   stopClock() {
//     clearInterval(this.clock);
//   }

//   /**
//    * ALU functionality
//    *
//    * The ALU is responsible for math and comparisons.
//    *
//    * If you have an instruction that does math, i.e. MUL, the CPU would hand
//    * it off to it's internal ALU component to do the actual work.
//    *
//    * op can be: ADD SUB MUL DIV INC DEC CMP
//    */
//   alu(op, regA, regB) {
//     switch (op) {
//       case 'MUL':
//         this.reg[regA] = this.reg[regA] * this.reg[regB];
//         break;
//       case 'ADD':
//         this.reg[regA] = this.reg[regA] + this.reg[regB];
//         break;
//       case 'AND':
//         this.reg[regA] = this.reg[regA] & this.reg[regB];
//     }
//   }

//   /**
//    * Advances the CPU one cycle
//    */
//   tick() {
//     // Load the instruction register (IR--can just be a local variable here)
//     // from the memory address pointed to by the PC. (I.e. the PC holds the
//     // index into memory of the instruction that's about to be executed
//     // right now.)

//     // !!! IMPLEMENT ME
//     this.reg.IR = this.ram.read(this.reg.PC);
//     // Debugging output
//     //console.log(`${this.reg.PC}: ${IR.toString(2)}`);

//     // Get the two bytes in memory _after_ the PC in case the instruction
//     // needs them.

//     // !!! IMPLEMENT ME

//     let offset = (this.reg.IR >> 6) & 0b00000011;

//     const operandA = this.ram.read(this.reg.PC + 1);
//     const operandB = this.ram.read(this.reg.PC + 2);
//     // console.log('operands', operandA, operandB);
//     let handler = this.branchTable[this.reg.IR];
//     // Execute the instruction. Perform the actions for the instruction as
//     // outlined in the LS-8 spec.

//     // !!! IMPLEMENT ME

//     if (!handler) {
//       this.HLT();
//       return undefined;
//     }
//     // Increment the PC register to go to the next instruction. Instructions
//     // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
//     // instruction byte tells you how many bytes follow the instruction byte
//     // for any particular instruction.
//     handler.call(this, operandA, operandB);

//     // !!! IMPLEMENT ME
//     this.reg.PC += offset + 1;
//   }

//   // Instruction Handler Code

//   ADD(regA, regB) {
//     this.alu('ADD', regA, regB);
//   }
//   HLT() {
//     this.stopClock();
//   }
//   LDI(reg, value) {
//     this.reg[reg] = value;
//   }
//   MUL(regA, regB) {
//     this.alu('MUL', regA, regB);
//   }
//   PRN(regA) {
//     console.log(this.reg[regA]);
//   }
//   AND(regA, regB) {
//     this.alu('AND', regA, regB);
//   }
//   NOP() {
//     return undefined;
//   }
//   PUSH(reg) {
//     this.reg[SP]--;
//     this.ram.write(this.reg[SP], this.reg[reg]);
//   }
//   POP(reg) {
//     this.reg[reg] = this.ram.read(this.reg[SP]);
//     this.reg[SP]++;
//   }
//   CALL(reg) {

//   }
//   RET(reg) {
//     this.reg.PC = this.POP();
//   }
// }

// module.exports = CPU;



// /** 
//  * LS-8 v2.0 emulator skeleton code
//  */

// const fs = require('fs');

// const HLT = 0b00000001; // Halt CPU
// const LDI = 0b10011001;
// const MUL = 0b10101010;
// const PRN = 0b01000011;
// const ADD = 0b10101000;
// const AND = 0b10101000;
// const NOP = 0b00000000;
// const PUSH = 0b01001101;
// const POP = 0b01001100;

// /**
//  * Class for simulating a simple Computer (CPU & memory)
//  */
// class CPU {
//   /**
//    * Initialize the CPU
//    */
//   constructor(ram) {
//     this.ram = ram;

//     this.reg = new Array(8).fill(0); // General-purpose registers R0-R7

//     // Special-purpose registers
//     this.reg.PC = 0; // Program Counter
//     this.reg.IR = 0; // Instruction Register

//     this.setupBranchTable();
//     this.reg[111] = 0xF4;
//   }
//   /**
//    * Set up the branch table
//    */
//   setupBranchTable() {
//     let bt = {};
//     bt[HLT] = this.HLT;
//     bt[LDI] = this.LDI;
//     bt[MUL] = this.MUL;
//     bt[PRN] = this.PRN;
//     bt[ADD] = this.ADD;
//     bt[AND] = this.AND;
//     bt[NOP] = this.NOP;
//     bt[PUSH] = this.PUSH;
//     bt[POP] = this.POP;
//     this.branchTable = bt;
//   }

//   /**
//    * Store value in memory address, useful for program loading
//    */

//   poke(address, value) {
//     this.ram.write(address, value);
//   }

//   /**
//    * Starts the clock ticking on the CPU
//    */
//   startClock() {
//     this.clock = setInterval(() => {
//       this.tick();
//     }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
//   }

//   /**
//    * Stops the clock
//    */
//   stopClock() {
//     clearInterval(this.clock);
//   }

//   /**
//    * ALU functionality
//    *
//    * The ALU is responsible for math and comparisons.
//    *
//    * If you have an instruction that does math, i.e. MUL, the CPU would hand
//    * it off to it's internal ALU component to do the actual work.
//    *
//    * op can be: ADD SUB MUL DIV INC DEC CMP
//    */
//   alu(op, regA, regB) {
//     switch (op) {
//       case 'MUL':
//         this.reg[regA] = this.reg[regA] * this.reg[regB];
//         break;
//       case 'ADD':
//         this.reg[regA] = this.reg[regA] + this.reg[regB];
//         break;
//       case 'AND':
//         this.reg[regA] = this.reg[regA] & this.reg[regB];
//     }
//   }

//   /**
//    * Advances the CPU one cycle
//    */
//   tick() {
//     // Load the instruction register (IR--can just be a local variable here)
//     // from the memory address pointed to by the PC. (I.e. the PC holds the
//     // index into memory of the instruction that's about to be executed
//     // right now.)

//     // !!! IMPLEMENT ME
//     this.reg.IR = this.ram.read(this.reg.PC);
//     // Debugging output
//     //console.log(`${this.reg.PC}: ${IR.toString(2)}`);

//     // Get the two bytes in memory _after_ the PC in case the instruction
//     // needs them.

//     // !!! IMPLEMENT ME

//     let offset = (this.reg.IR >> 6) & 0b00000011;

//     const operandA = this.ram.read(this.reg.PC + 1);
//     const operandB = this.ram.read(this.reg.PC + 2);
//     // console.log('operands', operandA, operandB);
//     let handler = this.branchTable[this.reg.IR];
//     // Execute the instruction. Perform the actions for the instruction as
//     // outlined in the LS-8 spec.

//     // !!! IMPLEMENT ME

//     if (!handler) {
//       this.HLT();
//       return undefined;
//     }
//     // Increment the PC register to go to the next instruction. Instructions
//     // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
//     // instruction byte tells you how many bytes follow the instruction byte
//     // for any particular instruction.
//     handler.call(this, operandA, operandB);

//     // !!! IMPLEMENT ME
//     this.reg.PC += offset + 1;
//   }

//   // Instruction Handler Code

//   ADD(regA, regB) {
//     this.alu('ADD', regA, regB);
//   }
//   HLT() {
//     this.stopClock();
//   }
//   LDI(reg, value) {
//     this.reg[reg] = value;
//   }
//   MUL(regA, regB) {
//     this.alu('MUL', regA, regB);
//   }
//   PRN(regA) {
//     console.log(this.reg[regA]);
//   }
//   AND(regA, regB) {
//     this.alu('AND', regA, regB);
//   }
//   NOP() {
//     return undefined;
//   }
//   PUSH(reg) {
//     this.reg[111]--;
//     this.ram.write(this.reg[111], this.reg[reg]);
//   }
//   POP(reg) {
//     this.reg[reg] = this.ram.read(this.reg[111]);
//     this.reg[111]++;
//   }
// }

// module.exports = CPU;


// /**
//   * LS-8 v2.0 emulator skeleton code
//   */

//  const fs = require('fs');

//  const HLT = 0b00000001; // Halt CPU
//  const LDI = 0b10011001;
//  const MUL = 0b10101010;
//  const PRN = 0b01000011;
//  const ADD = 0b10101000;
//  const AND = 0b10101000;
//  const NOP = 0b00000000;
//  const POP = 0b01001100;
//  const INC = 0b01111000;
//  const DEC = 0b01111001;
//  const PUSH = 0b1001101;

//  const SP = 0x07;

// // /**
// //  * Class for simulating a simple Computer (CPU & memory)
// //  */
//  class CPU {
//    /**
//     * Initialize the CPU
// //    */
//    constructor(ram) {
//      this.ram = ram;
//     // this.reg[SP] = 0xf3;

//      this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
//      this.reg[SP] = 0xf3;
// //     // Special-purpose registers
//      this.reg.PC = 0; // Program Counter
//      this.reg.IR = 0; // Instruction Register

//      this.setupBranchTable();
//    }

// //   /**
// //    * Set up the branch table
// //    */
//    setupBranchTable() {
//      let bt = {};
//      bt[HLT] = this.HLT;
//      bt[LDI] = this.LDI;
//      bt[MUL] = this.MUL;
//      bt[PRN] = this.PRN;
//      bt[ADD] = this.ADD;
//      bt[AND] = this.AND;
//      bt[NOP] = this.NOP;
//  	   bt[INC] = this.INC;
//  	   bt[DEC] = this.DEC;
//      bt[POP] = this.POP;
//      bt[PUSH] = this.PUSH;

//      this.branchTable = bt;
//    }

// //   /**
// //    * Store value in memory address, useful for program loading
// //    */

//    poke(address, value) {
//      this.ram.write(address, value);
//    }


// //   /**
// //    * Starts the clock ticking on the CPU
// //    */
//    startClock() {
//      this.clock = setInterval(() => {
//        this.tick();
//      }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
//    }

// //   /**
// //    * Stops the clock
// //    */
//    stopClock() {
//      clearInterval(this.clock);
//    }

// //   /**
// //    * ALU functionality
// //    *
// //    * The ALU is responsible for math and comparisons.
// //    *
// //    * If you have an instruction that does math, i.e. MUL, the CPU would hand
// //    * it off to it's internal ALU component to do the actual work.
// //    *
// //    * op can be: ADD SUB MUL DIV INC DEC CMP
// //    */
//    alu(op, regA, regB) {
//      switch (op) {
//        case 'MUL':
//          this.reg[regA] = this.reg[regA] * this.reg[regB];
//          break;
//        case 'ADD':
//          this.reg[regA] = this.reg[regA] + this.reg[regB];
//          break;
//        case 'AND':
//          this.reg[regA] = this.reg[regA] & this.reg[regB];
//  			case 'INC':
//  				this.reg[regA] += 1;
//  			case 'DEC':
//  				this.reg[regA] -= 1;
//      }
//    }

// //   /**
// //    * Advances the CPU one cycle
// //    */
//    tick() {
// //     // Load the instruction register (IR--can just be a local variable here)
// //     // from the memory address pointed to by the PC. (I.e. the PC holds the
// //     // index into memory of the instruction that's about to be executed
// //     // right now.)

// //     // !!! IMPLEMENT ME
//      this.reg.IR = this.ram.read(this.reg.PC);
// //     // Debugging output
// //    //  console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

// //     // Get the two bytes in memory _after_ the PC in case the instruction
// //     // needs them.

// //     // !!! IMPLEMENT ME

//      let offset = (this.reg.IR >> 6) & 0b00000011;

//      const operandA = this.ram.read(this.reg.PC + 1);
//      const operandB = this.ram.read(this.reg.PC + 2);
//   //    console.log('operands', operandA, operandB);
//      let handler = this.branchTable[this.reg.IR];
// //     // Execute the instruction. Perform the actions for the instruction as
// //     // outlined in the LS-8 spec.

// //     // !!! IMPLEMENT ME
// //    console.log(handler);
//     if (!handler) {
//       this.HLT();
//       return undefined;
//     }
//     // Increment the PC register to go to the next instruction. Instructions
//     // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
//     // instruction byte tells you how many bytes follow the instruction byte
//     // for any particular instruction.
//     handler.call(this, operandA, operandB);

//     // !!! IMPLEMENT ME
//     this.reg.PC += offset + 1;
//   }

//   // Instruction Handler Code

//   ADD(regA, regB) {
//     this.alu('ADD', regA, regB);
//   }
//   HLT() {
//     this.stopClock();
//   }
//   LDI(reg, value) {
//     this.reg[reg] = value;
//   }
//   MUL(regA, regB) {
//     this.alu('MUL', regA, regB);
//   }
//   PRN(regA) {
//     console.log(this.reg[regA]);
//   }
//   AND(regA, regB) {
//     this.alu('AND', regA, regB);
//   }
//   NOP() {
//     return undefined;
//   }
// 	INC(regA) {
// 		this.alu('INC', regA);
// 	}
//   DEC(regA) {
// 		this.alu('DEC', regA);
// 	}
// 	_pop() {
// 		const value = this.ram.read(this.reg[SP]);
// 		this.alu('INC', SP);
// 		return value;
//  	}
//  	POP(regA) {
//  		this.reg[regA] = this._pop();
//  	}
//  	_push(value) {
//  		this.alu('DEC', SP);
//  		this.ram.write(this.reg[SP], value);
//  	}
//  	PUSH(regA) {
//  		this._push(this.reg[regA]);
//  	}
//  }

//  module.exports = CPU;
