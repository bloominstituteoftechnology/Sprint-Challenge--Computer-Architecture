/**
 * LS-8 v2.0 emulator skeleton code
 */

const LDI = 0b10011001;
const PRN = 0b01000011;
const HLT = 0b00000001;

const ADD = 0b10101000;
const MUL = 0b10101010;
const SUB = 0b10101001;
const DIV = 0b10101011;
const AND = 0b10110011;
const INC = 0b01111000;
const DEC = 0b01111001;

const LD = 0b10011000;
const PRA = 0b01000010;

const POP = 0b01001100;
const PUSH = 0b01001101;
const CALL = 0b01001000;
const RET = 0b00001001;
const ST = 0b10011010;

const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

const SP = 7;

class CPU {

    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0);
        
        this.reg[SP] = 0xf4;

        this.PC = 0;

        this.FL = 0b00000000;
    }
    
    poke(address, value) {
        this.ram.write(address, value);
    }

    startClock() {
        this.clock = setInterval(() => {
            this.tick();
        }, 1);
    }

    stopClock() {
        clearInterval(this.clock);
    }

    alu(op, regA, regB) {
        switch (op) {
            case 'ADD':
                this.reg[regA] += this.reg[regB];
                break;

            case 'MUL':
                this.reg[regA] *= this.reg[regB];
                break;

            case 'SUB':
                this.reg[regA] -= this.reg[regB];

            case 'DIV':
                if (regB === 0) {
                    // console.error('Denominator cannot be zero.');
                    this.stopClock();
                } else {
                    this.reg[regA] /= this.reg[regB];
                }
                break;

            case 'AND':
                this.reg[regA] &= this.reg[regB];
                break;

            case 'INC':
                this.reg[regA]++;
                break;

            case 'DEC':
                this.reg[regA]--;
                break;

            default:
                console.log("Default");
                break;
        }
    }

    tick() {
        const push_it = x => {
            this.alu('DEC', SP);
            this.ram.write(this.reg[SP], x);
        };
      
        const pop_it = () => {
            const popped = this.ram.read(this.reg[SP]);
            this.alu('INC', SP);
            return popped;
        };

        const IR = this.ram.read(this.PC);

        const operandA = this.ram.read(this.PC + 1);
        const operandB = this.ram.read(this.PC + 2);

        switch(IR) {
            case LDI:
                this.reg[operandA] = operandB;
                // console.log(this.regs[operandA]);
                break;

            case PRN:
                console.log(this.reg[operandA]);
                // this.PC += 2;
                break;

            case HLT:
                this.stopClock();
                // this.PC += 1;
                break;

            case ADD:
                this.alu('ADD', operandA, operandB);
                break;

            case MUL:
                this.alu('MUL', operandA, operandB);
                break;

            case SUB:
                this.alu('SUB', operandA, operandB);
                break;

            case DIV:
                this.alu('DIV', operandA, operandB);
                break;

            case AND:
                this.alu('AND', operandA, operandB);
                break;

            case INC:
                this.alu('INC', operandA);
                break;

            case DEC:
                this.alu('DEC', operandA);
                break;

            case LD:
                this.reg[operandA] = this.reg[operandB];
                break;

            case PRA:
                console.log(String.fromCharCode(this.reg[operandA]));
                break;

            case POP:
                this.reg[operandA] = pop_it();
                break;

            case PUSH:
                push_it(this.reg[operandA]);
                break;

            case CALL:
                this.alu('DEC', SP);
                push_it(this.PC + 2);
                this.PC = this.reg[operandA];
                break;

            case RET:
                this.PC = this.ram.read(this.reg[SP]);
                this.alu('INC', SP);
                break;

            case ST:
                this.ram.write(this.reg[operandA], this.reg[operandB]);
                break;

            case CMP:
                if (this.reg[operandA] < this.reg[operandB]) this.FL = 0b00000100;
                if (this.reg[operandA] > this.reg[operandB]) this.FL = 0b00000010;
                if (this.reg[operandA] === this.reg[operandB]) this.FL = 0b00000001;
                break;

            case JMP:
                this.PC = this.reg[operandA];
                break;

            case JEQ:
                if (this.FL === 0b00000001) {
                  this.PC = this.reg[operandA];
                } else {
                  this.PC += 1 + (IR >> 6);
                }
                break;

            case JNE:
                if (this.FL !== 0b00000001) {
                  this.PC = this.reg[operandA];
                } else {
                  this.PC += 1 + (IR >> 6);
                }
                break;

            default:
                // console.log("Unknown Instruction: " + IR.toString(2));
                this.stopClock();
                return;
        }

        if (IR !== CALL && IR !== JMP && IR !== RET && IR !== JEQ && IR !== JNE) {
            this.PC += 1 + (IR >> 6);
        }
        // const instLen = (IR >> 6) + 1;
        // this.PC += instLen;

        // console.log(InstLen);
    }
}

module.exports = CPU;
