const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
// function loadMemory() {

//     // Hardcoded program to print the number 8 on the console

//     // Step 2-5

//     // const program = [ // print8.ls8
//     //     "10011001", // LDI R0,8  Store 8 into R0
//     //     "00000000",
//     //     "00001000",
//     //     "01000011", // PRN R0    Print the value in R0
//     //     "00000000",
//     //     "00000001"  // HLT       Halt and quit
//     // ];

//     // Step 6 

//     // const program = [
//     //     "10011001", // LDI R0,8  Store 8 into R0
//     //     "00000000",
//     //     "00001000",
//     //     "10011001", // LDI R1,9
//     //     "00000001",
//     //     "00001001",
//     //     "10101010", // MUL R0,R1
//     //     "00000000",
//     //     "00000001",
//     //     "01000011", // PRN R0    Print the value in R0
//     //     "00000000",
//     //     "00000001"  // HLT       Halt and quit
//     // ]

//     // Load the program into the CPU's memory a byte at a time
//     // for (let i = 0; i < program.length; i++) {
//     //     cpu.poke(i, parseInt(program[i], 2));
//     // }
// }

// Stores the filename in an array
const argv = process.argv.slice(2);

// Access the filename
const filename = argv[0];

// Define the regex
const regexp = /[0-1]{8}/gi;

// Reads the filename and 
const filedata = fs.readFileSync(filename, "utf8").match(regexp);

function unHardcode() {
    for (let i = 0; i < filedata.length; i++ ) {
        cpu.poke(i, parseInt(filedata[i], 2))
    }
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

// loadMemory(cpu);

unHardcode()

cpu.startClock();