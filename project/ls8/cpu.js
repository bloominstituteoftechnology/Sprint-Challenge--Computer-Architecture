/**
 * LS-8 v2.0 emulator skeleton code
 */

const instructions = {
    0b10101000: 'ADD',
    0b10110011: 'AND',
    0b01001000: 'CALL',
    0b10100000: 'CMP',
    0b01111001: 'DEC',
    0b10101011: 'DIV',
    0b00000001: 'HLT',
    0b01111000: 'INC',
    0b01001010: 'INT',
    0b00001011: 'IRET',
    0b01010001: 'JEQ',
    0b01010100: 'JGT',
    0b01010011: 'JLT',
    0b01010000: 'JMP',
    0b01010010: 'JNE',
    0b10011000: 'LD',
    0b10011001: 'LDI',
    0b10101100: 'MOD',
    0b10101010: 'MUL',
    0b00000000: 'NOP',
    0b01110000: 'NOT',
    0b10110001: 'OR',
    0b01001100: 'POP',
    0b01000010: 'PRA',
    0b01000011: 'PRN',
    0b01001101: 'PUSH',
    0b00001001: 'RET',
    0b10011010: 'ST',
    0b10101001: 'SUB',
    0b10110010: 'XOR'
}

// Indices of registers reserved by the cpu
const IM = 5
const IS = 6
const SP = 7

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
        this.FL = 0; // Comparison flags
        this.reg[SP] = 0xf3 // Initialize stack pointer

        // Spec demands interrupt priority
        this.interruptsEnabled = true
    }
    
    /**
     * Store value in memory address, useful for program loading
     */
    poke(address, value) {
        this.ram.write(address, value)
    }

    /**
     * Starts the clock ticking on the CPU
     */
    startClock() {
        // Every millisecond, perform one cpu cycle
        this.clock = setInterval(() => {
            this.tick()
        }, 1) // 1 ms delay == 1 KHz clock == 0.000001 GHz

        // Fire the timer interrupt
        // Every second, set the 0th bit of the Interrupt status to 1
        this.timer = setInterval(() => {
            this.reg[IS] |= 1
        }, 1000)
    }

    /**
     * Stops the clock
     */
    stopClock() {
        clearInterval(this.clock)
        clearInterval(this.timer)
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
        const lval = this.reg[regA]
        const rval = this.reg[regB]
        switch (op) {
            case 'ADD':
                return lval + rval & 0xff
            case 'SUB':
                return lval - rval & 0xff
            case 'MUL':
                return lval * rval & 0xff
            case 'DIV':
                if (regB === 0) {
                    console.log('Error: Division by zero')
                    this.HLT()
                }
                return Math.floor(lval / rval) & 0xff
            case 'INC':
                return lval + 1 & 0xff
            case 'DEC':
                return lval - 1 & 0xff
            case 'CMP':
                this.FL &= 0xf8
                const lval = this.reg[regA]
                const rval = this.reg[regB]
                if (lval < rval) {
                    this.FL |= 4  // Set the L bit
                } else if (lval > rval) {
                    this.FL |= 2  // Set the G bit
                } else {
                    this.FL |= 1  // Set the E bit
                }
                break
        }
    }

    /**
     * Advances the CPU one cycle
     */
    tick() {
        // Interrupt handling - prior to instruction fetch, check the Interrupt Mask
        // against the Interrupt Status to see if any desired interrupts occurred.
        // If so, suspend the current state by pushing to the stack, and change PC
        // to the location of the handler for the interrupt

        let interrupts = this.reg[IM] & this.reg[IS]
        for (let i = 0; i < 8; i++) {
            if ((interrupts >> i) & 1) {
                // Save all of our registers on the stack
                this._pushState()
                
                // Disallow any further interrupts until IRET
                this.interruptsEnabled = false
                
                // Clear the bit for the interrupt we are handling
                this.reg[IS] &= ~(1 << i)
                
                // The first instruction for the handler is stored in this byte, per the spec
                this.PC = this.ram.read(0xf8 + i)

                // console.log('next instruction: ', instructions[this.ram.read(this.PC)])
                break // Handle one interrupt per tick
            }
        }

        // Load the instruction register (IR--can just be a local variable here)
        // from the memory address pointed to by the PC. (I.e. the PC holds the
        // index into memory of the instruction that's about to be executed
        // right now.)

        const IR = this.ram.read(this.PC)

        // Debugging output
        // console.log(`${this.PC}: ${IR.toString(2)}`);

        // Get the two bytes in memory _after_ the PC in case the instruction
        // needs them.

        const b1 = this.ram.read(this.PC + 1)
        const b2 = this.ram.read(this.PC + 2)

        // Execute the instruction. Perform the actions for the instruction as
        // outlined in the LS-8 spec.

        const instruction = instructions[IR]
        // console.log(instruction)
        this[instruction](b1, b2)

        // Increment the PC register to go to the next instruction. Instructions
        // can be 1, 2, or 3 bytes long. Hint: the high 2 bits of the
        // instruction byte tells you how many bytes follow the instruction byte
        // for any particular instruction.
        
        // Implement the PC unless the instruction was CALL or JMP
        if (!['CALL', 'JMP', 'RET', 'JEQ', 'JNE'].includes(instruction)) {
            // Increment PC by 1 + the value of the two leftmost bits of the instruction
            this.PC += (IR >> 6) + 1
        }
        // console.log(`new PC: ${this.PC}`)
    }
    
    //
    // I/O functions
    //
    
    // Store immediate value into register
    LDI(register, immediate) {
        // console.log(`LDI ${register} ${immediate}`)
        this.reg[register] = immediate
    }
    
    // Write the value in reg2 to the address in reg1
    ST(reg1, reg2) {
        this.ram.write(this.reg[reg1], this.reg[reg2])
    }

    // Print character for value in register
    PRA(register) {
        console.log(String.fromCharCode(this.reg[register]))
    }

    // Print number value from register
    PRN(register) {
        console.log(this.reg[register])
    }

    //
    // Math functions
    //

    ADD(reg1, reg2) {
        this.reg[reg1] = this.alu('ADD', reg1, reg2)
    }

    MUL(reg1, reg2) {
        this.reg[reg1] = this.alu('MUL', reg1, reg2)
    }

    //
    // Comparisons
    // 

    CMP(reg1, reg2) {
        this.alu('CMP', reg1, reg2)
    }

    //
    // Control flow functions
    //

    // Stop the system
    HLT() {
        console.log('registers: ', this.reg)
        this.stopClock()
    }

    // Copy value in register to a new address in stack memory
    PUSH(register) {
        this._push(this.reg[register])
    }

    // Copy the latest value in stack memory to provided register
    POP(register) {
        this.reg[register] = this._pop()
    }

    // Change the PC to point to an instruction stored in memory
    CALL(register) {
        this._push(this.PC + 2)

        // Set the PC to the address stored in register
        this.PC = this.reg[register]
    }

    // Change the PC to point to the address on top of the stack
    RET() {
        this.PC = this._pop()
    }

    // Return from an interrupt handler
    IRET() {
        this.PC = this._popState()
        this.interruptsEnabled = true
    }

    // Change the PC to point to the address in given register
    JMP(register) {
        this.PC = this.reg[register]
    }

    // If the internal E flag is set, jump to the address in register
    JEQ(register) {
        // Check if least bit is set
        if (this.FL & 1) {
            this.PC = this.reg[register]
        } else {
            this.PC += 2
        }
    }

    // If the E flag is *not* set, jump to the address in register
    JNE(register) {
        // Check if least bit is not set
        if (~this.FL & 1) {
            this.PC = this.reg[register]
        } else {
            this.PC += 2
        }
    }

    // Do nothing
    NOP() {

    }
    
    //
    //  Internal methods
    //

    // Get a new stack frame and write the provided value
    _push(value) {
        this.reg[SP]--
        this.ram.write(this.reg[SP], value)
    }

    // Discard the current stack frame and return its value
    _pop() {
        const value = this.ram.read(this.reg[SP])
        this.reg[SP]++
        return value
    }
    
    // Push all registers onto the stack in preparation for an interrupt
    _pushState() {
        this._push(this.PC)
        this._push(this.FL)
        for (let i = 0; i < 7; i++) {
            this._push(this.reg[i])
        }
    }

    // Recover registers from the stack after an interrupt
    _popState() {
        for (let i = 6; i >= 0; i--) {
            this.reg[i] = this._pop()
        }
        this.FL = this._pop()
        return this._pop()
    }
}

module.exports = CPU;
