const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');

function loadMemory() {
    const programToLoad = fs.readFileSync(process.argv[2], 'utf8');
    const formattedProgram = programToLoad.split('').filter(char => (char === '0' || char === '1' || char === '\n')).join('').split('\n').filter(line => line).map(line => line.slice(0,8));
    console.log('Ready to load program onto ls8!', formattedProgram);
    for (let i = 0; i < formattedProgram.length; i++) {
        cpu.poke(i, parseInt(formattedProgram[i], 2));
    }
}

let ram = new RAM(256);
let cpu = new CPU(ram);

loadMemory(cpu);

cpu.startClock();