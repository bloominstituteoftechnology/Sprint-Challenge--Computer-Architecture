const fs = require('fs');
// const path = require('path');

const RAM = require('./ram');
const CPU = require('./cpu');

const checkForFile = () => {
  // remove the first two args, node and this file and just load the file afterwards
  const argv = process.argv.slice(2);

  // add an error check to check the number of params, to make sure a user passes in a file
  if (argv.length !== 1) {
    console.error('usage: [filename]');
    process.exit(1);
  }

  return argv[0];
};

const loadFile = () => {
  // check if user passed a file
  const filename = checkForFile();

  // read the file that was passed to our program
  const filedata = fs.readFileSync(filename, 'utf8');

  // split the file into lines
  const programLines = filedata.trim().split(/[\r\n]+/g);
  return programLines;
};

const loadMemory = (cpu, programCode) => {
  // only read the binary part
  const program = programCode
    .map(line => parseInt(line, 2).toString(2))
    .filter(num => !isNaN(num))
    .map(byte => {
      const len = byte.length;

      if (len < 8) {
        for (let i = 0; i < 8 - len; i++) {
          byte = '0' + byte;
        }
      }
      return byte;
    });

  program.forEach((code, i) => cpu.poke(i, parseInt(code, 2)));
};

const ram = new RAM(256);
const cpu = new CPU(ram);

// load file program
const file = loadFile();

// load the cpu with the file code
loadMemory(cpu, file);

// start the program
cpu.startClock();
