/**
 * LS-8 v2.0 emulator skeleton code
 */

const fs = require('fs');
const ram = require('./ram.js');

// Instructions

const HLT  = 0b00000001; // Halt CPU
// !!! IMPLEMENT ME
const LDI = 0b10011001;  // LDI R,I(mmediate)
const MUL = 0b10101010;  // MUL R,R
const PRN = 0b01000011;  // Print numeric register
const ADD = 0b10101000;  // ADD R R
const AND = 0b10110011;  // AND R R
const NOP = 0b00000000;  // NOP
const NOT = 0b01110000;  // NOT R
const XOR = 0b10110010;  // XOR R R
const OR = 0b10110001;   // OR R R
const CALL = 0b01001000; // CALL R
const CMP  = 0b10100000; // CMP R R
const DEC  = 0b01111001; // DEC R
const DIV  = 0b10101011; // DIV R R
const HLT  = 0b00000001; // Halt CPU
const INC  = 0b01111000; // INC R
const INT  = 0b01001010; // Software interrupt R
const IRET = 0b00001011; // Return from interrupt
const JEQ  = 0b01010001; // JEQ R
const JGT  = 0b01010100; // JGT R
const JLT  = 0b01010011; // JLT R
const JMP  = 0b01010000; // JMP R
const JNE  = 0b01010010; // JNE R
const LD   = 0b10011000; // Load R,R
const MOD  = 0b10101100; // MOD R R
const POP  = 0b01001100; // Pop R
const PRA  = 0b01000010; // Print alpha char
const PRN  = 0b01000011; // Print numeric register
const PUSH = 0b01001101; // Push R
const RET  = 0b00001001; // Return
const ST   = 0b10011010; // Store R,R
const SUB  = 0b10101001; // SUB R R

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

        this.reg[IM] = 0; // All interrupts masked
        this.reg[IS] = 0; // No interrupts active
        this.reg[SP] = 0xf4; // Stack empty

        
        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.IR = 0; // Instruction Register
       // this.reg.FL = 0; // Flags
       // this.interruptsEnabled = true;
        this.reg[SP] = 0xf4;
        this.reg[SP] = 0b11110100;
        this.reg[SP] = 244;

		this.setupBranchTable();
    }
	
	/**
	 * Sets up the branch table
	 */
	setupBranchTable() {
		let bt = {};

        bt[HLT] = this.HLT;
        bt[ADD] = this.ADD;
        bt[MUL] = this.MUL;
        bt[LDI] = this.LDI;
        bt[PRN] = this.PRN;
        bt[AND] = this.AND;
        bt[NOP] = this.NOP;
        bt[OR] = this.OR;
        bt[XOR] = this.XOR;
        bt[NOT] = this.NOT;
        bt[CALL] = this.CALL;
        bt[CMP]  = this.CMP;
        bt[DEC]  = this.DEC;
        bt[DIV]  = this.DIV;
        bt[INC]  = this.INC;
        bt[INT]  = this.INT;
        bt[IRET] = this.IRET;
        bt[JEQ]  = this.JEQ;
        bt[JGT]  = this.JGT;
        bt[JLT]  = this.JLT;
        bt[JMP]  = this.JMP;
        bt[JNE]  = this.JNE;
        bt[LD]   = this.LD;
        bt[MOD]  = this.MOD;
        bt[POP]  = this.POP;
        bt[PRA]  = this.PRA;
        bt[PUSH] = this.PUSH;
        bt[RET]  = this.RET;
        bt[ST]   = this.ST;
        bt[SUB]  = this.SUB;
      
    // Bind all the functions to this so we can call them later
    
    
    for (let k of Object.keys(bt)) {
       bt[k] = bt[k].bind(this)  //
    }
    this.branchTable = bt;

   
  }
    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value);
    }
     /**
   * Raise an interrupt
   * 
   * @param n Interrupt number, 0-7
   */
    raiseInterrupt(n) {
    this.reg[IS] |= intMask[n];
    }

  /**
    /**
     * Starts the clock ticking on the CPU
     */
    startClock() {
        const _this = this;

        this.clock = setInterval(() => {
            _this.tick();
        }, 1);
    }

    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock);
    }
        // 00000100 //FL
        // 00000001 
    /**
     * ALU functionality
     * 
     * op can be: ADD SUB MUL DIV INC DEC CMP
     */
    alu(op, regA, regB) {
        switch (op) {
            case 'MUL':
                // !!! IMPLEMENT ME
                this.reg[regA] = this.reg[A] * this.reg[B];

                break;
            case 'ADD':
                // IMPLEMENT ME
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                regA = regA + regB
                break;
            
            case 'AND':
                // IMPLEMENT ME
                this.reg[regA] = this.reg[regA] & this.reg[regB];
                regA = regA & regB
                break;
            case 'OR':
                this.reg[regA] = valA | valB;
                break;
        
            case 'NOT':
                this.reg[regA] = ~valA;
                break;
    
            case 'XOR':
                this.reg[regA] = valA ^ valB;
                break;

            case 'CMP':
                this.setFlag(FLAG_EQ, valA === valB);
                this.setFlag(FLAG_GT, valA > valB);
                this.setFlag(FLAG_LT, valA < valB);
                break;    
            }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR) from the current PC
        // !!! IMPLEMENT ME
        this.reg.IR = this.ram.read(this.reg.PC);
        // Debugging output
        console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

        // Based on the value in the Instruction Register, locate the
        // appropriate hander in the branchTable
        // !!! IMPLEMENT ME
        let handler = this.branchTable[this.reg.IR];

        // Check that the handler is defined, halt if not (invalid
        // instruction)

        if(handler === undefined) {
            console.error('Unknown opcode ' + this.reg.IR);
            this.stopClock();
            return;
        }
        // !!! IMPLEMENT ME
        let operandA = this.mem.read(this.reg.PC +1);
        let operandB = this.mem.read(this.reg.PC +2);

        // We need to use call() so we can set the "this" value inside
        // the handler (otherwise it will be undefined in the handler)
        handler.call(this, operandA, operandB);
        if (nextPC === undefined) {
            
        // Increment the PC register to go to the next instruction
        // !!! IMPLEMENT ME
        // need to know how many bytes to move along
        this.reg.PC += ((this.reg.IR >> 6) & 0b00000011) + 1; 
        } else {
            this.reg.PC = nextPC;
        }
    }
    // INSTRUCTION HANDLER CODE:
     /**
       * Internal push helper, doesn't move PC
       */
    _push(value) {
    // Decrement SP, stack grows down from address 0xF7
    this.alu('DEC', SP);

    // Store value at the current SP
    this.ram.write(this.reg[SP], value);
    }
    pushHelper(value) {
        this.reg[SP]--
        this.ram.write(this.reg[SP], value);
    }
    popHelper() {
        let val = this.ram.read(this.reg[SP]);
        this.reg[SP]++;
        return val;
    }
    _pop() {
        const val = this.ram.read(this.reg[SP]);
    
        // Increment SP, stack grows down from address 255
        this.alu('INC', SP);
    
        return val;
      }
    /**
     * ADD RR
     */
    ADD(regA, regB) {
        this.alu('ADD', regA, regB);
    }
    /**
     * AND RR
     */
    AND(regA, regB) {
        this.alu('AND', regA, regB);
    }   
    /**
     * CALL 
     */
    CALL(regNum) {
        //this.PUSH(...);
        pushHelper(this.reg.PC + 2);
        // Now we need an address
        const addr = this.reg[regNum];
        // instruct the PC to start executing at that address
        return addr;
    }
    /**
     * HLT
     */
    HLT(regA, regB) {
        // !!! IMPLEMENT ME
    //     this.alu('HLT', regA, regB);
    // }
    // HLT() {
        this.stopClock();
    }

    /**
     * LDI R,I
     */
    LDI(regNum, value) {
        this.reg[regNum] = value & 255;    // !!! IMPLEMENT ME
    }

    /**
     * MUL R,R
     */
    MUL(regA, regB) {
        // !!! IMPLEMENT ME
        // Call the ALU
        this.alu('MUL', regA, regB);
    }
    /**
     * NOP 
     */
    NOP() {
        return;
     }  
     /**
      * OR RR
      */  
    OR(regA, regB) {
         this.alu('OR', regA, regB)
     }
    
    /**
     * PRN R
     */
    PRN(regA) {
        // !!! IMPLEMENT ME
        console.log(this.reg[regA]);
    }
     /**
     * PUSH 
     */
    PUSH(regNum) {
        // !!! IMPLEMENT ME
        let value = this.reg[regNum];
        // let currentSP = this.reg[SP];
        // let newSP = currentSP - 1;
        // this.reg[SP] = newSP;

        // this.ram.write(this.reg[SP], value);
        pushHelper(value)
    }
   

    /**
     * POP R
     */
    POP(regNum) {
    this.reg[regNum] = this._pop();
    // let val = popHelper();
    
    }

    /**
     * PRA R
     */
    PRA(regNum) {
    fs.writeSync(process.stdout.fd, String.fromCharCode(this.reg[reg]));
    }


    /***************SPRINT CHALLENGE***************** */
    /**
       * CMP RR
       */
    CMP(regA, regB) {
        this.alu('CMP', regA, regB);
    }
    /**
       * JMP R
       */
    JMP(regNum) {
        //execute at specified register
        return this.reg[regNum];
    }
    /**
       * JEQ R
       */
    JEQ(regNum) {
        //execute at specified register when equal flag is set true
        if (this.getFlag(FLAG_EQ)) {
            return this.reg[regNum];
          } 
    }
    /**
       * JNE R
       */
    JNE(regNum) {
        // if equal flag is false, execute at specified register
        if (!this.getFlag(FLAG_EQ)) {
            return this.reg[regNum]; 
    }
    
     
     
    
     
    

}

module.exports = CPU;
