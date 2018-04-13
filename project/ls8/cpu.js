const IM = 0x05;
const IS = 0x06;
const SP = 0x07;

class CPU {

    constructor(ram) {
        this.ram = ram;

        this.reg = new Array(8).fill(0);
        this.reg[SP] = 0xf4;
        this.reg.PC = 0;
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

        const LDI = 0b10011001;
        const HLT = 0b00000001;
        const PRN = 0b01000011;
        const MUL = 0b10101010;
        const PUSH = 0b01001101;
        const POP = 0b01001100;

        const execute_LDI = () => {
            this.reg[operandA] = operandB;
        };
        const execute_HLT = () => {
            this.stopClock();
        };
        const execute_PRN = () => {
            console.log("Printline output: ", this.reg[operandA]);
        };
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

        const opIndex = [];
        opIndex[LDI] = execute_LDI;
        opIndex[HLT] = execute_HLT;
        opIndex[PRN] = execute_PRN;
        opIndex[MUL] = execute_MUL;
        opIndex[PUSH] = execute_PUSH;
        opIndex[POP] = execute_POP;

        opIndex[IR]();

        const operandCount = IR >>> 6
        this.reg.PC++;
        this.reg.PC += operandCount;
    }
}

module.exports = CPU;