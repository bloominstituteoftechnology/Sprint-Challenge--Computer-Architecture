const RAM = require('./ram');
const CPU = require('./cpu');
const fileName = process.argv[2];
const fs = require('fs');
/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {

    // Hardcoded program to print the number 8 on the console

    // const program = [ //  mult.ls8
    //     "10011001", // LDI R0,8  Load R0 with value 8
    //     "00000000",
    //     "00001000",
    //     "10011001", // LDI R1,9  Load R1 with value 9
    //     "00000001",
    //     "00001001",
    //     "10101010", // MUL R0,R1 Multiply R0*R1, storing result in R0
    //     "00000000",
    //     "00000001",
    //     "01000011", // PRN R0    Print value in R0
    //     "00000000",
    //     "00000001", // HLT       Halt

   // ];
    
    // Load the program into the CPU's memory a byte at a time
    // for (let i = 0; i < program.length; i++) {
    //    cpu.poke(i, parseInt(program[i], 2));
    // }
    // const fileName = process.argv[2];
    // let counter = 0;

    // let lineReader = require('readline').createInterface({
    //     input: require('fs').createReadStream(`./${fileName}`)
    //   });
    //   lineReader.on('line', function (line) {
    //     cpu.poke(counter, parseInt(line, 2));
    //     //console.log('Line from file:',counter, line);
    //     counter++;
    //   });
    //   lineReader.on('close', () => {
    //     cpu.startClock();
    //     lineReader.close();
    //   })
      //end of file
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

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

cpu.startClock();