/**
 * LS-8 v2.0 emulator skeleton code
 */

const ADD = 0b10101000;
const AND = 0b10110011;
const CMP = 0b10100000;
const DEC = 0b01111001;
const INC = 0b01111000;
const HLT = 0b10101011;
const LDI = 0b10011001;
const PRN = 0b01000011;
const MUL = 0b10101010;
const PUSH = 0b01001101;
const POP = 0b01001100;
const JMP = 0b0101000;
const CALL = 0b01001000;
const RET = 0b00001001;
const JEQ = 0b01010001;
const JNE = 0b01010010;
// const JGT = 0b01010100;
// const FL_EQ = 0b001;
// const FL_GT = 0b010;
// const FL_LT = 0b100;

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
        this.PC = 0; // Program Counter
        this.reg[7] = 0xF4;

        this.FL = 0b00000000;

        // Setting Operations


    }

    flagger(flag){
        const flags = {
            EQ: 0b00000001,
        };
        this.FL = this.FL & ~flags[flag];
        this.FL = this.FL |  flags[flag];
        console.log('Current Flag: ', this.FL)
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
            case 'MUL':
                // !!! IMPLEMENT ME
                this.reg[regA] *= this.reg[regB];
                break;
            case 'ADD':
                this.reg[regA] += this.reg[regB];
                break;
            case 'INC':
                this.reg[regA]++;
                break;
            case 'DEC':
                this.reg[regA]--;
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
        let IR = this.ram.read(this.PC);
        // let SP = this.ram.read(this.SP)
        // console.log(this.SP)


        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        // !!! IMPLEMENT ME

        let operandA = this.ram.read(this.PC + 1);
        let operandB = this.ram.read(this.PC + 2);
        


        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME

        
        switch(IR){
            

            case LDI:
                this.reg[operandA] = operandB;
                // this.PC += 3;
                break;
            case MUL:
                this.alu('MUL', operandA, operandB);
                break;
            case ADD:
                this.alu('ADD', operandA, operandB);
                break;   
            case INC:
                this.alu('INC', operandA);
                break;
            case DEC:
                this.alu('DEC', operandA);
                break;
            case PUSH:
                // let reg;
                this.reg[7]--;
                this.ram.write(this.reg[7], this.reg[operandA]);
                break;
            case POP:
                this.reg[operandA] = this.ram.read(this.reg[7])
                this.reg[7]++;
                break;
            case CALL:
                this.reg[7]--;
                this.ram.write(this.reg[7], this.PC + 2);
                this.PC = this.reg[operandA];
                break;
            case RET:
                this.PC = this.ram.read(this.reg[7])
                this.reg[7]++;
                break;
            case CMP:
                if (this.reg[operandA] === this.reg[operandB]){
                this.flagger('EQ');
                }
                if(this.reg[operandA] < this.reg[operandB]){
                this.flagger('LT')
                }
                if(this.reg[operandA] > this.reg[operandB]){
                this.flagger('GT');
                }
                break;
            case JEQ:
                if(this.FL === 1){
                    this.PC = this.reg[operandA]
                }
                else{
                    this.PC += (IR >> 6) + 1
                }
                break;
       
            case JNE:
                if(this.FL !== 1){
                    this.PC = this.reg[operandA];
                }
                else{
                    this.PC += (IR >> 6) + 1
                }
                break;
            case JMP:
                this.PC = this.reg[operandA];
                break;
            case PRN: 
                console.log(this.reg[operandA]);
                // this.PC += 2;
                break;
            case HLT:
                this.stopClock();
                break;
            default:
                console.log(`Unknown at ${this.PC}: ${IR.toString(2)}`);
                this.stopClock();
        }


        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !!! IMPLEMENT ME
        if(IR !== CALL && IR !== JMP && IR !== JNE && IR !== JEQ && IR !== RET){
        this.PC += (IR >> 6) + 1;
        }
    }
}

module.exports = CPU;
