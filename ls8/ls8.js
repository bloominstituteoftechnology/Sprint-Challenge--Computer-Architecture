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

    // const program = [ // print8.ls8
    //     "10011001", // LDI R0,8  Store 8 into R0
    //     "00000000",
    //     "00001000",
    //     "01000011", // PRN R0    Print the value in R0
    //     "00000000",
    //     "00000001"  // HLT       Halt and quit
    // ];

    // const program = [
        
        // "10011001", //# LDI R0,8
        // "00000000",
        // "00001000",
        // "10011001", //# LDI R1,9
        // "00000001",
        // "00001001",
        // "10101010", //# MUL R0,R1
        // "00000000",
        // "00000001",
        // "01000011", //# PRN R0
        // "00000000",
        // "00000001", //# HLT
        // ]; 

    let program = fs.readFileSync(process.argv[2], { encoding: 'binary' });
    while (program.includes('#')) {
        let startIndex = program.indexOf('#');
        if (startIndex > 0) startIndex--;
        const endIndex = program.indexOf('\n', startIndex);
        if (endIndex === -1) program = program.slice(0, startIndex);
        else program = program.slice(0, startIndex) + program.slice(endIndex);
    }
    program = program.replace(/' '/g, '').trim().split('\n');
    // Load the program into the CPU's memory a byte at a time
    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, program[i]);
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