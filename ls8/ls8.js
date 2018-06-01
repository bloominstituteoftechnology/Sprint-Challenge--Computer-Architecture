const RAM = require('./ram');
const CPU = require('./cpu');
const fileName = process.argv[2];
const fs = require('fs');

function loadMemory() {

    const program = [];
      const loadFile = fs.readFileSync(`./${fileName}`, {encoding: 'utf-8'}).split('\n');
    
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

let ram = new RAM(256);
let cpu = new CPU(ram);



loadMemory(cpu);

cpu.startClock();