const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */

function loadMemory() {
    const filename = process.argv[2] + '';
    const file = fs.readFileSync(filename, 'binary').split('\n');
    const cleanFile = file.map(line => {
        return line.replace(/\#.*/,'').replace(/[^0-9]+/, '');
    });

    let program = [];
    cleanFile.forEach(line => {
        if (line === '' || line === '\r') return;
        program.push(line);
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

loadMemory();

cpu.startClock();