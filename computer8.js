const fs = require('fs');
const RAM = require('./ram');
const CPU = require('./cpu');
const argv = process.argv;


if (process.argv.length !== 3) {
  console.error('msg: you need 3 names  ==> node , executerFile and  dataFile ')
  process.exit(1)
}

let file = argv[2];
const fileData = fs.readFileSync(file, "utf8");
const lines = fileData.trim().split(/[\r\n]+/g);
let program = [];
for (let line of lines) {
  const value = parseInt(line, 2);
  if (isNaN(value)) {
    continue; /// continue reading the code and  do nothing 
  }
  program.push(value);
}



loadMemory = (cpu, program) => {
  for (let i = 0; i < program.length; i++) {
    cpu.pock(i, program[i])
  }
}

const ram = new RAM;
const cpu = new CPU(ram)
loadMemory(cpu, program)
cpu.startClock()