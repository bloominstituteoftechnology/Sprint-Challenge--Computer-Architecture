const RAM = require('./ram');
const CPU = require('./cpu');
const readline = require('readline');
const fs = require('fs');

/**i
 * Load an LS8 program into memory
 */
function loadMemory() {
  const instream = fs.createReadStream(process.argv[2]);
  instream.on('end', () => { 
    cpu.startClock();
  });
 
  const rl = readline.createInterface({
    input: instream
  });
  let n = -1;
  rl.on('line', function (line) {
    if (!line) {
    } else if (!isNaN(line.slice(0,8))) {
      n++;
      line = line.replace(/#/g, "//");
      cpu.poke(n, parseInt(line.slice(0,8), 2));
    }
  });
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

