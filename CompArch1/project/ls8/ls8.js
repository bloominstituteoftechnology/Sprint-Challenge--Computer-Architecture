const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {

    // Hardcoded program to print the number 8 on the console

    // load and print 8 program
    // const program = [ // print8.ls8
    //     "10011001", // LDI R0,8  Store 8 into R0
    //     "00000000",
    //     "00001000",
    //     "01000011", // PRN R0    Print the value in R0
    //     "00000000",
    //     "00000001"  // HLT       Halt and quit
    // ];

    // load, multiply and print program
    // const program = [
    //     "10011001", // # LDI R0,8  Load R0 with value 8
    //     "00000000",
    //     "00001000",
    //     "10011001", // # LDI R1,9  Load R1 with value 9
    //     "00000001",
    //     "00001001",
    //     "10101010", // # MUL R0,R1 Multiply R0*R1, storing result in R0
    //     "00000000",
    //     "00000001",
    //     "01000011", // # PRN R0    Print value in R0
    //     "00000000",
    //     "00000001", // # HLT       Halt
    // ];

    // stack program
    // const program = [
    //     "10011001", // # LDI R0,1
    //     "00000000",
    //     "00000001",
    //     "10011001", // # LDI R1,2
    //     "00000001", //
    //     "00000010", //
    //     "01001101", // # PUSH R0
    //     "00000000", //
    //     "01001100", // # POP R1
    //     "00000001", //
    //     "01000011", // # PRN R1
    //     "00000001", //
    //     "10011001", // # LDI R0,2
    //     "00000000", //
    //     "00000010", //
    //     "01001101", // # PUSH R0
    //     "00000000", //
    //     "10011001", // # LDI R0,3
    //     "00000000", 
    //     "00000011",
    //     "01001100", // # POP R0
    //     "00000000",
    //     "01000011", //# PRN R0
    //     "00000000",
    //     "00000001"  // # HLT
    // ]

    // const program = [
    //     "10011001", //# LDI R1,MULT2PRINT  R1 points to the MULT2PRINT subroutine
    //     "00000001",
    //     "00011000",

    //     "10011001", //# LDI R0,10          Load R0 with 10
    //     "00000000",
    //     "00001010",
    //     "01001000", //# CALL R1            Call MULT2PRINT, prints 20
    //     "00000001",

    //     "10011001", //# LDI R0,15          Load R0 with 15
    //     "00000000",
    //     "00001111",
    //     "01001000", //# CALL R1            Call MULT2PRINT, prints 30
    //     "00000001",

    //     "10011001", //# LDI R0,18          Load R0 with 18
    //     "00000000",
    //     "00010010",
    //     "01001000", //# CALL R1            Call MULT2PRINT, prints 36
    //     "00000001",

    //     "10011001", //# LDI R0,30          Load R0 with 30
    //     "00000000",
    //     "00011110",
    //     "01001000", //# CALL R1            Call MULT2PRINT, prints 60
    //     "00000001",

    //     "00000001", //# HLT

    //     //# MULT2PRINT (24):

    //     "10101000", //# ADD R0,R0         Mult R0 by 2 (add it to itself)
    //     "00000000",
    //     "00000000",
    //     "01000011", //# PRN R0            Print R0
    //     "00000000",
    //     "00001001" //# RET
    // ]

    const program = [];
    const loadFile = fs
        .readFileSync(`./${process.argv[2]}`, { encoding: 'utf-8' })
        .split('\n');

    for (let line of loadFile) {
        const comment = line.indexOf('#');

    if (comment !== -1) {
      line = line.substr(0, comment);
    }
    line = line.trim();

    if (line === '') {
      continue;
    }
    program.push(line);
    }

    // Load the program into the CPU's memory a byte at a time
    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, parseInt(program[i], 2));
    }
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

cpu.startClock();