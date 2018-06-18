const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');
const argv = process.argv.slice(2);

if(argv.length != 1) {
    console.error("usage: filename capacity");
    process.exit(1);
}


/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
    const filename = argv[0];
    const filedata = fs.readFileSync(filename, "utf-8");
    const lines = filedata.trim().split(/[\r\n]+/g);
    const program = [];

    for (let l of lines) {
        l = l.slice(0,8);
        if(isNaN(l)) continue;
        program.push(l);
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