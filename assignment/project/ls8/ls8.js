const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory(cpu, filename) {
    const program = [];
    const file = fs.readFileSync(filename, 'utf8');
    file.split('\n').forEach(x => {
        const op = x.split(' ')[0];
        if (op.length === 8) program.push(op);
    });

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
if (process.argv.length <= 2) {
    console.log('Please supply a program file to load as the first argument.');
} else {
    loadMemory(cpu, process.argv[2]);
    cpu.startClock();
}