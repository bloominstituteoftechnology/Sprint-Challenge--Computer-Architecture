const RAM = require('./ram');
const CPU = require('./cpu');

const fs = require('fs');

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {
  // Hardcoded program to print the number 8 on the console
  // Peter's Solution
  const readLine = require('readline').createInterface({
    input: require('fs').createReadStream(`./${process.argv[2]}`, 'utf8'),
  });

  let instructionCount = 0;

  readLine.on('line', (line) => {
    let instruction = line.split('#')[0].trim();
    if (instruction.trim().length != 0) {
      cpu.poke(instructionCount, parseInt(instruction, 2));
      instructionCount++;
    }
  });
  readLine.on('close', () => {
    cpu.startClock();
    readLine.close();
  });
  // End Peter's Solution
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// TODO: get name of ls8 file to load from command line

loadMemory(cpu);

// Eileen Solution
// function processFile(content, cpu, onComplete) {
//     // Pointer to the memory address in the CPU that we're
//     // loading a value into:
//     let curAddr = 0;

//     // Split the lines of the content up by newline
//     const lines = content.split('\n');

//     // Loop through each line of machine code

//     for (let line of lines) {

//         // !!! IMPLEMENT ME

//         // Strip comments

//         // Remove whitespace from either end of the line

//         // Ignore empty lines

//         // Convert from binary string to numeric value

//         // Store in the CPU with the .poke() function

//         // And on to the next one
//         if (line.startsWith('#') || line.trim().length === 0) continue;
//         line = parseInt(line.split('#')[0].trim(), 2);
//         cpu.poke(curAddr, line);
//         curAddr++;
//     }

//     onComplete(cpu);
// }

// /**
//  * Load the instructions into the CPU from stdin
//  */
// function loadFileFromStdin(cpu, onComplete) {
//     let content = '';

//     // Read everything from standard input, stolen from:
//     // https://stackoverflow.com/questions/13410960/how-to-read-an-entire-text-stream-in-node-js
//     process.stdin.resume();
//     process.stdin.on('data', function(buf) { content += buf.toString(); });
//     process.stdin.on('end', () => { processFile(content, cpu, onComplete); });
// }

// /**
//  * Load the instructions into the CPU from a file
//  */
// function loadFile(filename, cpu, onComplete) {
//     const content = fs.readFileSync(filename, 'utf-8');
//     processFile(content, cpu, onComplete);
// }

// /**
//  * On File Loaded
//  *
//  * CPU is set up, start it running
//  */
// function onFileLoaded(cpu) {
//     cpu.startClock();
// }

// /**
//  * Main
//  */

// let ram = new RAM(256);
// let cpu = new CPU(ram);

// // Get remaining command line arguments
// const argv = process.argv.slice(2);

// // Check arguments
// if (argv.length === 0) {
//     // Read from stdin
//     loadFileFromStdin(cpu, onFileLoaded);
// } else if (argv.length == 1) {
//     // Read from file
//     loadFile(argv[0], cpu, onFileLoaded);
// } else {
//     console.error('usage: ls8 [machinecodefile]');
//     process.exit(1);
// }
// End Eileen Solution