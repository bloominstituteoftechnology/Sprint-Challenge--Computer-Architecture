const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs');


function loadMemory() {

    const argv = process.argv.slice(2);
    if (argv.length != 1) {
        console.log('specify one file to read instructions from')
        process.exit(1);
    }
    const filename = argv[0];

    const filedata = fs.readFileSync(filename, 'utf8');

    const lines = filedata.trim().split(/[\r\n]+/g).filter(line => line[0] != '#');
    const program = [];

    lines.forEach(line => program.push(line.split(' ')[0]));

  

    for (let i = 0; i < program.length; i++) {
        cpu.poke(i, parseInt(program[i], 2));
    }
}



let ram = new RAM(256);
let cpu = new CPU(ram);



loadMemory(cpu);

cpu.startClock();
cpu.startInterruptClock();