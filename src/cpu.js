const fs = require('fs');

const HLT = 0b00011011; // Halt CPU
const LDI = 0b00000100; // Load Register Immediate
const CMP = 0b00010110; // Compare Register
const JEQ = 0b00010011; // JEQ Register
const JNE = 0b00010100; // JNE Register
const JMP = 0b00010001; // Jump R R
const PRN = 0b00000110; // PRN Register

const IM = 5; // R5
const IS = 6; // R6
const SP = 7; // R7

class CPU {
	/**
	 * Initialize the CPU
	 */
	constructor(ram) {
		this.ram = ram;

		this.reg = new Array(8).fill(0); // General-purpose registers R0-R7
		this.reg[7] = 0xf8;

		this.reg.PC = 0; // Program Counter
		this.reg.IR = 0; // Instruction Register

		this.setupBranchTable();
	}

	/**
	 * Sets up the branch table
	 */
	setupBranchTable() {
		let bt = {};

		bt[HLT] = this.HLT;
		bt[LDI] = this.LDI;
		bt[PRN] = this.PRN;
		bt[CMP] = this.CMP;
		bt[JEQ] = this.JEQ;
		bt[JNE] = this.JNE;
		bt[JMP] = this.JMP;

		this.branchTable = bt;
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
		/*
      console.log("RAM dump");
      for (let i = 0; i < 15; i++) {
          console.log(this.ram.read(i).toString(2));
      }
      */
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

	/*
   * ALU functionality
   * 
   * op can be: ADD SUB MUL DIV INC DEC CMP
   */
	alu(op, regA, regB) {
		let valA = this.reg[regA];
		let valB = this.reg[regB];
		switch (op) {
			case 'MUL':
				// !!! IMPLEMENT ME
				// value_in_regA = valA * valB
				this.reg[regA] = (valA * valB) & 0b11111111; // 255 is the limit. 0b11111111 is binary for 255
				break;
			case 'ADD':
				this.reg[regA] = (valA + valB) & 255;
				break;
		}
	}

	/**
	 * Advances the CPU one cycle
	 */
	tick() {
		// !!! IMPLEMENT ME
		// Check if an interrupt happened
		const maskedInterrupts = this.reg[IS] & this.reg[IM];
		if (maskedInterrupts !== 0) {
			for (let i = 0; i <= 7; i++) {
				if (((maskedInterrupts >> i) & 1) === 1) {
					console.log('interrupt! ' + i);
				}
			}
			this.reg[IS] = 0;
		}

		// Load the Instruction Register [IR] from the memory address pointed by the PC
		this.reg.IR = this.ram.read(this.reg.PC);

		// Debugging output
		// console.log(`${this.reg.PC}: ${this.reg.IR.toString(2)}`);

		// Based on the value in the Instruction Register, jump to the
		// appropriate hander in the branchTable
		const handler = this.branchTable[this.reg.IR];

		// Check that the handler is defined, halt if not (invalid
		// instruction)
		if (!handler) {
			console.error(
				`Invalid instruction at address ${this.reg.PC}: ${this.reg.IR.toString(
					2
				)}`
			);
			this.stopClock();
			return;
		}

		// We need to use call() so we can set the "this" value inside
		// the handler (otherwise it will be undefined in the handler)
		handler.call(this);
	}

	HLT() {
		// !!! IMPLEMENT ME
		this.stopClock();
		console.log('Halt!');
	}

	LDI() {
		// !!! IMPLEMENT ME
		const regA = this.ram.read(this.reg.PC + 1);
		const val = this.ram.read(this.reg.PC + 2); //immediate value

		this.reg[regA] = val;
		// Move the PC
		this.reg.PC += 3;
	}

	PRN() {
		// !!! IMPLEMENT ME
		const reg = this.ram.read(this.reg.PC + 1);
		console.log(this.reg[reg]);
		this.reg.PC += 2;
	}

	JMP() {
		const regA = this.ram.read(this.reg.PC + 1);

		// Jump to the addresss stored in the regA
		this.reg.PC = this.reg[regA];
	}
}
