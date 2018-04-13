const IM = 0x05;
const IS = 0x06;
const SP = 0x07;

const LDI = 0b10011001;
const HLT = 0b00000001;
const PRN = 0b01000011;
const MUL = 0b10101010;
const ADD = 0b10101000;
const PUSH= 0b01001101;
const POP = 0b01001100;
const CALL= 0b01001000;
const RET = 0b00001001;
const CMP = 0b10100000;
const JMP = 0b01010000;
const JEQ = 0b01010001;
const JNE = 0b01010010;

class CPU {

    constructor(ram) {
        this.ram = ram;
        this.reg = new Array(8).fill(0);
        this.reg[SP] = 0xf4;
        this.reg.PC = 0;
        this.reg.FL = 0b00000000;
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
        const firstVal = this.reg[regA];
        const secondVal = this.reg[regB];

        switch (op) {
            case 'ADD':
                return this.reg[regA] = firstVal + secondVal;
            case 'DEC':
                return this.reg[regA] = this.reg[regA] - 1;
            case 'DIV':
                if (firstVal || secondVal === 0){
                   return this.reg[regA] = 0;
                } else {
                    return this.reg[regA] = firstVal / secondVal;
                }
            case 'INC':
                return this.reg[regA] = this.reg[regA] + 1;
            case 'MOD':
                return this.reg[regA] = firstVal % secondVal;
            case 'MUL':
                return this.reg[regA] = firstVal * secondVal;
            case 'SUB':
                return this.reg[regA] = firstVal - secondVal;
            case 'CMP':
                if (firstVal < secondVal) return this.reg.FL = 0b00000100;
                if (firstVal > secondVal) return this.reg.FL = 0b00000010;
                if (firstVal === secondVal) return this.reg.FL = 0b00000001;
        }
    }

    tick() {
        const PC = this.reg.PC;
        const IR = this.ram.read(PC);

        console.log("\n");
        console.log(`${this.reg.PC}: ${IR.toString(2)}`);

        const operandA = this.ram.read(PC + 1);
        console.log("First potential operand: ", operandA.toString(2));
        const operandB = this.ram.read(PC + 2);
        console.log("Second potential operand: ", operandB.toString(2));


        const execute_LDI = () => {
            this.reg[operandA] = operandB;
        };
        const execute_HLT = () => {
            this.stopClock();
        };
        const execute_PRN = () => {
            console.log("Printline output: ", this.reg[operandA]);
        };
        const execute_ADD = () => {
            this.alu('ADD', operandA, operandB);
        }
        const execute_MUL = () => {
            this.alu('MUL', operandA, operandB);
        }
        const execute_PUSH = () => {
            this.alu('DEC', SP);
            this.ram.write(this.reg[SP], this.reg[operandA]);
        };
        const execute_POP = () => {
            const val = this.ram.read(this.reg[SP]);
            this.alu('INC', SP);
            this.reg[operandA] = val;
            return;
        }
        const execute_CALL = () => {
            console.log("About to make a Call elsewhere");
            this.alu('DEC', SP);
            const temp = PC + 1;
            console.log("Is this an address or an instruction? ", temp.toString(2));
            this.ram.write(this.reg[SP], PC+2);
            return this.reg[operandA];
        };
        const execute_RET = () => {
            const val = this.ram.read(this.reg[SP]);
            this.alu('INC', SP);
            this.reg.PC = val;
            return val;
        };
        const execute_CMP = () => {
            this.alu('CMP', operandA, operandB);
        };
        const execute_JMP = () => {
            return this.reg[operandA];
        };
        const execute_JEQ = () => {
            if (this.reg.FL === 0b00000001) return this.reg[operandA];
        };
        const execute_JNE = () => {
            if (this.reg.FL !== 0b00000001) return this.reg[operandA];
        };

        const opIndex = [];
        opIndex[LDI] = execute_LDI;
        opIndex[HLT] = execute_HLT;
        opIndex[PRN] = execute_PRN;
        opIndex[MUL] = execute_MUL;
        opIndex[PUSH] = execute_PUSH;
        opIndex[POP] = execute_POP;
        opIndex[CALL] = execute_CALL;
        opIndex[RET] = execute_RET;
        opIndex[ADD] = execute_ADD;
        opIndex[CMP] = execute_CMP;
        opIndex[JMP] = execute_JMP;
        opIndex[JEQ] = execute_JEQ;
        opIndex[JNE] = execute_JNE;

        if (!opIndex[IR]) {
            console.log("Received a malformed instruction: ", IR.toString(2));
            this.stopClock();
        }
        console.log("About to carry out this instruction: ", IR.toString(2));
        const addressToGoTo = opIndex[IR]();
        
        if (addressToGoTo) {
            this.reg.PC = addressToGoTo;
        } else {
            const operandCount = IR >>> 6
            this.reg.PC++;
            this.reg.PC += operandCount;
        }
    }
}

module.exports = CPU;