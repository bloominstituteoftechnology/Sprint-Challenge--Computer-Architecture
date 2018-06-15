/**
 * LS-8 v2.0 emulator skeleton code
 */
const HLT = 0b00000001,
      PRN = 0b01000011,
      LDI = 0b10011001, 
      MUL = 0b10101010, 
      ADD = 0b10101000,
      AND = 0b10110011,
      CALL= 0b01001000,
      CMP = 0b10100000,
      DEC = 0b01111001,
      DIV = 0b10101011,
      INC = 0b01111000,
      INT = 0b01001010,
      IRET= 0b00001011,
      JEQ = 0b01010001,
      JGT = 0b01010100,
      JLT = 0b01010011,
      JMP = 0b01010000,
      JNE = 0b01010010,
       LD = 0b10011000,
      MOD = 0b10101100,
      NOP = 0b00000000,
      NOT = 0b01110000,
       OR = 0b10110001,
      POP = 0b01001100,
      PRA = 0b01000010,
      PUSH= 0b01001101,
      RET = 0b00001001,
       ST = 0b10011010,
      SUB = 0b10101001,
      XOR = 0b10110010,
       SP = 7,
        E = 0,
        G = 0,
        L = 0,
       IM = 0x05,
       IS = 0x06;
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
        this.PC = 0;
        this.intEnabled = true;
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
            case 'SUB': 
                this.reg[regA] -= this.reg[regB];
                break;
            case 'DIV':
                this.reg[regA] /= this.reg[regB];
                break;
            case 'INC':
                this.reg[regA]++;
                break;
            case 'DEC':
                this.reg[regA]--;
                break;
            case 'CMP':
            this.reg[regA] > this.reg[regB] ? this.G = 1 : this.G = 0;
                this.reg[regA] === this.reg[regB] ? this.E = 1 : this.E = 0;
                this.reg[regA] < this.reg[regB] ? this.L = 1 : this.L = 0;
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
        let poo = this.ram.read(this.PC + 1);
        let dung = this.ram.read(this.PC + 2);

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME
        switch(IR) {
            case ADD: 
                this.alu('ADD', poo, dung);
                break;
            case 'AND': 
               this.reg[poo] = (poo & dung);  
                break;
            case CMP:
                this.alu('CMP', poo, dung);
                break;
            case CALL:
                this.reg[SP]--;
                this.ram.write(this.reg[SP], this.PC + 2);
                this.PC = this.reg[poo];
            break;
            case RET:
                this.PC = this.ram.read(this.reg[SP]);
                this.reg[SP]++;
            break;
            case 'DEC':
                this.alu('DEC', poo);
                break;
            case 'DIV':
                this.alu('DIV', poo, dung);
                break;
            case 'INC':
                this.alu('INC', poo);
                break;
            case MUL: 
                this.alu('MUL', poo, dung);
                break;
            case LDI: 
                this.reg[poo] = dung;
                break;
            case JEQ:
                if(this.E === 1) this.PC = this.reg[poo];
                else this.PC += (IR >> 6) + 1;
                break;
            case JNE: 
                if(this.E === 0) this.PC = this.reg[poo];
                else this.PC += (IR >> 6) + 1;
                break;
            case PRN:
                console.log(this.reg[poo]);
                break;
            case PUSH:
            this.reg[SP]++;
            this.reg[SP] = this.reg[poo];
            break;
            case POP:
            this.reg[poo] = this.reg[SP];
            this.reg[SP]--;
            break;
            case PRA: 
            console.log(String.fromCharCode(this.reg[poo]));
            break;
            case IRET:
            for(let i = 6; i >= 0; i--) {
                this.reg[i] = this.popcorn();
            }
                this.PC = this.popcorn();
                this.intEnabled = true;
            break;
            case JMP:
            this.PC = this.reg[poo];
            break;
            case ST:
            this.ram.write(this.reg[poo], this.reg[dung]);
            break;
            case HLT:
                this.stopClock();
                break;
        }

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // !ME!! IMPLEMENT 
        if(IR != CALL && IR != RET && IR != JMP && IR != JEQ && IR != JNE){
       this.PC += (IR >> 6) + 1;
        }
    }
}

module.exports = CPU;
