const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {

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

    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, parseInt(program[i], 2));
    }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

loadMemory(cpu);

cpu.startClock();