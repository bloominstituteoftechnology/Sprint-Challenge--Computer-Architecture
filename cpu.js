/* eslint-disable
 camelcase, no-console, no-bitwise, max-statements, no-plusplus
 */

/**
 * LS-8 v2.0 emulator skeleton code
 */

/**
 * Class for simulating a simple Computer (CPU & memory)
 */
const ADD = 0b10101000;
const CALL = 0b01001000;
const HLT = 0b00000001;
const IRET = 0b00001011;
const JMP = 0b01010000;
const LDI = 0b10011001;
const MUL = 0b10101010;
const POP = 0b01001100;
const PRA = 0b01000010;
const PRN = 0b01000011;
const PUSH = 0b01001101;
const RET = 0b00001001;
const ST = 0b10011010;
const DEC = 0b01111001;
const DIV = 0b10101011;
const INC = 0b01111000;
const SUB = 0b10101001;
const CMP = 0b10100000;

const SP = 7;
const IS = 6;
const IM = 5;

class CPU {

    /* Initialize the CPU */
    constructor(ram) {
        this.ram = ram;
        this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
        
        this.reg[SP] = this.ram.read(Number(0xf4)); // Stack Pointer (SP)

        // Special-purpose registers
        this.reg.PC = 0; // Program Counter
        this.reg.FL = 0; // Flag
    }
    
    /* Store value in memory address, useful for program loading */
    poke(address, value) {
        this.ram.write(address, value);
    }

    /* Starts the clock ticking on the CPU */
    startClock() {
        this.clock = setInterval(() => {
            this.intTimer();
            this.tick();
        }, 1); // 1 ms delay == 1 KHz clock == 0.000001 GHz
        // this.clock = setInterval(() => {
        //     this.intTimer();
        // }, 1000); // Fires every 1 second (1000ms)
    }

    /* Stops the clock */
    stopClock() {
        clearInterval(this.clock);
        clearInterval(this.intTimer);
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
            case 'ADD':
                this.reg[regA] = this.reg[regA] + this.reg[regB];
            break;
            case 'MUL':
                this.reg[regA] = this.reg[regA] * this.reg[regB];
            break;
            case 'DEC':
                this.reg[regA]--;
            break;
            case 'DIV':
                if (this.reg[regB] === 0) {
                    console.log('Error: Value of Reg. B is 0!');
                    this.stopClock();
                }
                this.reg[regA] = this.reg[regA] / this.reg[regB];
            break;
            case 'INC':
                this.reg[regA]++;
            break;
            case 'SUB':
                this.reg[regA] = this.reg[regA] - this.reg[regB];
            break;
            case 'CMP':
                if (this.reg[regA] < this.reg[regB]) {
                    this.reg.FL = 0b00000100;
                } else if (this.reg[regA] > this.reg[regB]) {
                    this.reg.FL = 0b00000010;
                } else if (this.reg[regA] === this.reg[regB]) {
                    this.reg.FL = 0b00000001;
                }
            break;
        }
    }

    intTimer() {
        if(this.reg[IM]) {
            this.reg[IS] = 0b00000001;
            let maskedInterrupts = this.reg[IM] & this.reg[IS];
            for (let i = 0; i <= 7; i++) {
                let interruptHappened = ((maskedInterrupts >> i) & 1) == 1;
                if (interruptHappened) {
                    this.reg[IM] = 0; // disable further interrupts
                    this.reg[IS] = 0;

                    // push the PC onto the stack
                    this.reg[SP]--;
                    this.ram.write(this.reg[SP], this.reg.PC);

                    // push registers 0-6 onto the stack
                    for (let i = 0; i <= 6; i++) {
                        this.reg[SP]--;
                        this.ram.write(this.reg[SP], this.reg[i]);
                    }
                    // Set the vector(address) of the interrupt handler to PC
                    this.reg.PC = this.ram.read(0xF8);
                } else {
                    return;
                }
            }
        }
    }
    
    /* Advances the CPU one cycle */
    tick() {
    // Load the instruction register (IR--can just be a local variable here)
    // from the memory address pointed to by the PC. (I.e. the PC holds the
    // index into memory of the instruction that's about to be executed
    // right now.)
        let IR = this.ram.read(this.reg.PC);
        let callHandler;
        let intHandler;
        let jmpHandler; 

        // Debugging output
        // console.log(`${this.reg.PC}: ${IR.toString(2)}`);
        // console.log(
        //     `JMP Handler: ${jmpHandler}`,
        //     `Interrupt Handler: ${intHandler}`,
        //     `Call Handler: ${callHandler}`
        // );
        
        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.
        let operandA = this.ram.read(this.reg.PC + 1);
        let operandB = this.ram.read(this.reg.PC + 2);
        
        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.
        const handle_ADD = () => {
            this.alu('ADD', operandA, operandB);
        }

        const handle_CALL = () => {
            this.reg[SP]--;
            this.ram.write(this.reg[SP], this.reg.PC + (IR >>> 6) + 1)
            callHandler = this.reg[operandA];
            return callHandler;
        }

        const handle_DEC = () => {
            this.alu('DEC', operandA);
        }

        const handle_DIV = () => {
            this.alu('DIV', operandA, operandB);
        }

        const handle_HLT = () => {
            this.stopClock();
        }

        const handle_INC = () => {
            this.alu('INC', operandA);
        }
        
        const handle_IRET = () => {
            for(let i = 6; i >= 0; i--) {
                this.reg[i] = this.ram.read(this.reg[SP])
                this.reg[SP]++;
            }
            intHandler = this.ram.read(this.reg[SP])
        }

        const handle_JMP = () => {
            jmpHandler = this.reg[operandA];
        }

        const handle_LDI = () => {
            this.reg[operandA] = operandB;
        }
        
        const handle_MUL = () => {
            this.alu('MUL', operandA, operandB);
        }

        const handle_POP = () => {
            this.reg[operandA] = this.ram.read(this.reg[SP]);
            this.reg[SP]++;
        }
        
        const handle_PRA = () => {
           console.log(String.fromCharCode(this.reg[operandA]));
        } 

        const handle_PRN = () => {
            console.log(this.reg[operandA]);
        }

        const handle_PUSH = () => {
            this.reg[SP]--;
            this.ram.write(this.reg[SP], this.reg[operandA]);
        }

        const handle_RET = () => {
            callHandler = this.ram.read(this.reg[SP]);
            this.reg[SP]++;
            return callHandler;
        }

        const handle_ST = () => {
            this.ram.write(this.reg[operandA], this.reg[operandB]);
        }

        const handle_SUB = () => {
            this.alu('SUB', operandA, operandB);
        }

        const handle_CMP = () => {
            this.alu('CMP', operandA, operandB);
        }
        
        const branchTable = {
            [ADD]: handle_ADD,
            [CALL]: handle_CALL,
            [DEC]: handle_DEC,
            [DIV]: handle_DIV,
            [HLT]: handle_HLT,
            [INC]: handle_INC,
            [IRET]: handle_IRET,
            [JMP]: handle_JMP,
            [LDI]: handle_LDI,
            [MUL]: handle_MUL,
            [POP]: handle_POP,
            [PRA]: handle_PRA,
            [PRN]: handle_PRN,
            [PUSH]: handle_PUSH,
            [RET]: handle_RET,
            [ST]: handle_ST,
            [SUB]: handle_SUB,
            [CMP]: handle_CMP
        }
        
        branchTable[IR](operandA, operandB);

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        if (intHandler) {
            this.reg.PC = intHandler;
        } else if (callHandler) {
            this.reg.PC = callHandler;
        } else if (jmpHandler) {
            this.reg.PC = jmpHandler
        } else {
            this.reg.PC += (IR >>> 6) + 1;            
        }
    }
}

module.exports = CPU;
