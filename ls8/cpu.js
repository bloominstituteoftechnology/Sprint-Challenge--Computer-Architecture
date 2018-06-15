/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */

//  Print correct values
 const LDI = 0b10011001;
 const PRN = 0b01000011;
 const HLT = 0b00000001;
 const MUL = 0b10101010;

// Stack
 const PUSH = 0b01001101;
 const POP = 0b01001100;

// Subroutine call
 const CALL = 0b01001000;
 const RET = 0b00001001;
 const ADD = 0b10101000;

// Interrupt
 const ST = 0b10011010;
 const JMP = 0b01010000;
 const PRA = 0b01000010;
 const IRET = 0b00001011;
 const CMP = 0b10100000;

 // Sprint
 const JEQ = 0b01010001;
 const JNE = 0b01010010;

 // Step 8 progress
    //  const branchTable = [];

    //  function LDI_Handler(regA, regB) {
    //     this.reg[regA] = regB;
    //  }
    //  function PRN_Handler(regA) {
    //     console.log(this.reg[regA]);
    //  }
    //  function HLT_Handler() {
    //     this.stopClock();
    //  }
    //  function MULT_Handler(regA, regB) {
    //     this.reg[regA] = this.reg[regA] * this.reg[regB]
    //  }

    //  branchTable['LDI'] = LDI_Handler
    //  branchTable['PRN'] = PRN_Handler
    //  branchTable['HLT'] = HLT_Handler
    //  branchTable['MULT'] = MULT_Handler
    //  console.log(branchTable)

class CPU {

    /**
     * Initialize the CPU
     */
    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.IM = 5; // R5, interrupt mask 
        this.IS = 6; // R6, interrupt status
        this.SP = 7; // 7 = R7, SP = stack pointer

        // Register
        this.FL = {E: false, G: false, L: false}; // R0-R7, R5-R7 is reserved

        this.reg[this.SP] = 0xF4; // 244 = F4
        this.reg[this.IM] = 0xF8; // 248 = F8
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
            case ADD: 
                this.reg[regA] = this.reg[regA] + this.reg[regB];
                this.PC += 3;
                break;
            case MUL:
                this.reg[regA] = this.reg[regA] * this.reg[regB];
                this.PC += 3;
                break;
            case LDI:
                this.reg[regA] = regB;
                this.PC += 3;
                break;
            case PRN:
                console.log(this.reg[regA]);
                this.PC += 2;
                break;
            case HLT:
                this.stopClock();
                break;
            case PUSH:
                this.SP--;
                this.ram.write(this.reg[this.SP] - 1, this.reg[regA])
                this.PC += 2;
                // console.log('register before decrement', this.reg)
                this.reg[this.SP] -= 1;
                // console.log('after decrementing', this.reg)
                break;
            case POP:
                this.reg[regA] = this.ram.read(this.reg[this.SP]);
                this.SP++;
                this.PC += 2;
                // console.log('before increment', this.reg)
                this.reg[this.SP] += 1;
                // console.log(this.reg)
                break;
            case CALL:
                this.reg[this.SP] - 1; 
                this.ram.write(this.reg[this.SP], this.PC + 2) // The address of the _next_ instruction that will execute is pushed onto the stack.
                this.PC = this.reg[regA]
                break;
            case RET: 
                this.PC = this.ram.read(this.reg[this.SP]);
                this.reg[this.SP] += 1;
                console.log(this.reg)
                break;
            case ST:
                this.reg[regA] = this.reg[regB]
                this.PC += 3;
                break;
            case PRA:
                console.log(String.fromCharCode(this.reg[regA])) // String.fromCharCode
                this.PC += 2;
                break;
            case IRET:
                break;
            case JMP: 
                this.PC = this.reg[regA];
                break;
            case CMP: 
                if (this.reg[regA] === this.reg[regB]) this.FL["E"] = true;
                else if (this.reg[regA] > this.reg[regB]) this.FL["L"] = true;
                else if (this.reg[regA] > this.reg[regB]) this.FL["G"] = true;
                this.PC += 3;
                break;
            case JEQ:
                if (this.FL["E"] === true) this.PC = this.reg[regA];
                this.PC += 2;
                break;
            case JNE:
                if (this.FL["E"] === false) this.PC = this.reg[regA];
                this.PC +=2;
                break;
            default:
                this.stopClock();
                break;
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
        const IR = this.ram.read(this.PC);
        
        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);
        
        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.
        
        // !!! IMPLEMENT ME
        const operandA = this.ram.read(this.PC + 1);
        const operandB = this.ram.read(this.PC + 2);
        
        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        
        // Step 2-5, switch case in tick
            // switch(IR) {
            //     case LDI: 
            //         this.reg[operandA] = operandB;
            //         this.PC += 3;
            //         break;
            //     case PRN: 
            //         console.log(this.reg[operandA]);
            //         this.PC += 2;
            //         break;
            //     case HLT: 
            //         this.stopClock();
            //         break;
            //     default:
            //         this.stopClock();
            //         return;
            // }

        // Multiply - Step 6
        this.alu(IR, operandA, operandB)
        // branchTable[IR](operandA, operandB)
        
        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
        // const instLen = (IR >> 6) + 1;
        // this.PC += instLen;
    }
}

module.exports = CPU;
