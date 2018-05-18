/**
 * LS-8 v2.0 emulator skeleton code
 */

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
        this.reg[7] = 243;
        // Special-purpose registers
        this.PC = 0; // Program Counter
        this.FL = 0; // Flag
        this.operandA = null;
        this.operandB = null;
        this.PCmoved = false;
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
    
    startInterruptClock() {
        this.intClock = setInterval(() => {
            this.interuptTimer();
        }, 1000);
    }

    stopInterruptClock() {
        clearInterval(this.intClock);
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
            break;
        }
    }
    
    //handler methods
    PUSH(item) {
        this.reg[7]--;
        if (item || item === 0)
            this.ram[this.reg[7]] = item;
        else
            this.ram[this.reg[7]] = this.reg[this.operandA];
    }

    POP() {
        this.reg[this.operandA] = this.ram[this.reg[7]];
        
        return this.ram[this.reg[7]++]; 
    }

    rPOP() {
        return this.ram[this.reg[7]++];
    }
    
    ADD() {
        this.reg[this.operandA] += this.reg[this.operandB];
    }

    PRN() {
        console.log(this.reg[this.operandA]);
    }

    LDI() {
        this.reg[this.operandA & 0b111] = this.operandB;
    }

    AND() {
        this.reg[this.operandA] = this.reg[this.operandA] & this.reg[this.operandB];
    }

    MUL() {
        this.reg[this.operandA] *= this.reg[this.operandB];
    }
    
    CALL() {
        // this.reg[7]--;
        // this.ram[this.reg[7]] = this.PC + 2;
        this.PUSH(this.PC + 2);
        this.PC = this.reg[this.operandA];
        this.PCmoved = true;
    }

    RET() {
        // this.PC = this.ram[this.reg[7]];
        // this.reg[7]++;
        this.PC = this.POP();
        this.PCmoved = true;
    }

    ST() {
        this.ram[this.reg[this.operandA]] = this.reg[this.operandB];
    }

    JMP() {
        this.PC = this.reg[this.operandA];
        this.PCmoved = true;
    }

    JEQ() {
        
        if (this.FL & 0b1) {
            this.PC = this.reg[this.operandA];
            this.PCmoved = true;
        }
    }

    JNE() {
        if (!(this.FL & 0b1)) {
            this.PC = this.reg[this.operandA];
            this.PCmoved = true;
        }
    }

    IRET() {
        for (let i = 6; i >= 0; i--)
            this.reg[i] = this.rPOP();
        // this.FL = this.POP();
        this.PC = this.rPOP();
        this.PCmoved = true;
    }

    PRA() {
        console.log(String.fromCharCode(this.reg[this.operandA]));
    }

    CMP() {
        if (this.reg[this.operandA] > this.reg[this.operandB])
            this.FL |= 0b010;
        else
            this.FL &= 0b101;
            
        if (this.reg[this.operandA] < this.reg[this.operandB])
            this.FL |= 0b100;
        else
            this.FL &= 0b011;

        if (this.reg[this.operandA] === this.reg[this.operandB])
            this.FL |= 0b001;
        else    
            this.FL &= 0b110;

    }

    HLT() {
        this.stopClock();
        this.stopInterruptClock();
    }
    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)

        //check for interupts
        
        if (this.reg[6]) {
            let maskedInterrupts = this.reg[6] & this.reg[5];
            for (let i = 0; i < 8; i++) {
                let interruptHappened = ((maskedInterrupts >> i) & 1) === 1;
                if (interruptHappened) {
                    this.reg[5] = 0;
                    //this.reg[6] = this.reg[6] & ~Math.pow(2, i);
                    this.reg[6] = 0;
                    // start pushing
                    this.PUSH(this.PC);
                    //this.PUSH(this.FL)
                    for (let j = 0; j <= 6; j++) {
                        this.PUSH(this.reg[j]);
                    }

                    let vector = this.ram[255 - 7 - i];
                    this.PC = vector;
                }
                    
            }

        }
        
        // !!! IMPLEMENT ME
        const IR = this.ram.read(this.PC);
        this.PCmoved = false;
        // Debugging output
        //console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.
        this.operandA = this.ram.read(this.PC + 1);
        this.operandB = this.ram.read(this.PC + 2);
        
        // !!! IMPLEMENT ME
        //const category = (IR & 0b11000) >> 3;
        
        
        

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        // !!! IMPLEMENT ME
        const table = {
            0: function() {
                //do nothing
            },
            0b00000001: () => this.HLT(),
            0b01000011: () => this.PRN(),
            0b10011001: () => this.LDI(),
            0b10101000: () => this.ADD(),
            0b10110011: () => this.AND(),
            0b10101010: () => this.MUL(),
            0b01001100: () => this.POP(),
            0b01001101: () => this.PUSH(),
            0b01001000: () => this.CALL(),
            0b00001001: () => this.RET(),
            0b10011010: () => this.ST(),
            0b01010000: () => this.JMP(),
            0b00001011: () => this.IRET(),
            0b01000010: () => this.PRA(),
            0b10100000: () => this.CMP(),
            0b01010001: () => this.JEQ(),
            0b01010010: () => this.JNE(),
        };
        
        if (table[IR]){
            table[IR]();
        }
        else
            console.log(IR.toString(2));

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.

        if(!this.PCmoved)
            this.PC += (IR >> 6) + 1;
        
        
        // !!! IMPLEMENT ME
    }

    interuptTimer() {
        this.reg[6] = 1;
        
    }
}

module.exports = CPU;
