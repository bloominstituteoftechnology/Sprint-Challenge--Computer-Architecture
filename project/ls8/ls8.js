const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');
/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
const argv = process.argv[2];
// console.log(argv);

const fileData = fs.readFileSync(argv, "utf8").match(/[0-1]{8}/g);
// console.log(fileData);

function loadMemory() {
    // Hardcoded program to print the number 8 on the console

    const program = [];
    fileData.forEach(data => program.push(data));
    // console.log(program)

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