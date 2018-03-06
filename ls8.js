const fs = require('fs');
const RAM = require('./ram');
const CPU = require('./cpu');

/**
 * Process a loaded file
 * @param content raw contents of a file containing our bin instructions
 */
function processFile(content, cpu, onComplete) {
    // Pointer to the memory address in the CPU that we're
    // loading a value into:
    let currentAddress = 0;
    
    // Split the lines of the content up by newline, trim whitespace
    const lines = content.split('\n').forEach((lineItem) => {
        let currentLine = parseInt(lineItem.trim().substring(0, 8), 2);
        let ifConditional = !isNaN(currentLine);
        if (ifConditional) { cpu.poke(currentAddress++, currentLine); }
        
    });
    // console.log(cpu.ram.mem);
    onComplete(cpu); // Takes a callback but returns nothing itself in the end.
}

/**
 * Load the instructions into the CPU from stdin
 */
function loadFileFromStdin(cpu, onComplete) {
    let content = '';

    // Read everything from standard input, stolen from:
    // https://stackoverflow.com/questions/13410960/how-to-read-an-entire-text-stream-in-node-js
    process.stdin.resume();
    process.stdin.on('data', function(buf) { content += buf.toString(); });
    process.stdin.on('end', () => { processFile(content, cpu, onComplete); });
}

/**
 * Load the instructions into the CPU from a file
 * @operation calls the processFile method
 */
function loadFile(filename, cpu, onComplete) { // onComplete is not called
    const content = fs.readFileSync(filename, 'utf-8');
    processFile(content, cpu, onComplete);
}

/**
 * On File Loaded
 * @param cpu passing a reference to the CPU
 * CPU is set up, start it running
 */
function onFileLoaded(cpu) {
    cpu.startClock();
    // console.log('hit');
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);

// Get remaining command line arguments
const argv = process.argv.slice(2);
// console.log(argv);
// Check arguments
if (argv.length === 0) {
    // Read from stdin
    loadFileFromStdin(cpu, onFileLoaded); // onFileLoaded is passed all the way to processFile
} else if (argv.length == 1) {
    // Read from file
    loadFile(argv[0], cpu, onFileLoaded);
} else {
    console.error('usage: ls8 [machinecodefile]');
    process.exit(1);
}
