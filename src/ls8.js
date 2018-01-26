
const fs = require('fs');
const RAM = require('./ram');
const CPU = require('./cpu');
const KEYBOARD = require('./keyboard');

/**
 * Process a loaded file
 */
function processFile(content, cpu, onComplete) {
    // Pointer to the memory address in the CPU that we're
    // loading a value into:
    let curAddr = 0;
    
    // Split the lines of the content up by newline
    const lines = content.split('\n');

    // Loop through each line of machine code

    for (let line of lines) {

        // !!! IMPLEMENT ME

        // Strip comments
        let hashIdx = line.indexOf('#');
        if (hashIdx !== -1) {
            line = line.substr(0, hashIdx);
        } 
        // Remove whitespace from either end of the line
        line = line.trim();
        // Ignore empty lines
        if (line.length === 0) {
            continue;
        } 
        // Convert from binary string to numeric value
        let val = parseInt(line, 2);
        // Store in the CPU with the .poke() function
        cpu.poke(curAddr, val)
        // And on to the next one
        curAddr++;
    }

    onComplete(cpu);
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
 */
function loadFile(filename, cpu, onComplete) {
    const content = fs.readFileSync(filename, 'utf-8');
    processFile(content, cpu, onComplete);
}

/**
 * On File Loaded
 * 
 * CPU is set up, start it running
 */
function onFileLoaded(cpu) {
    // things start to happen here
    cpu.startClock();
}

/**
 * Main
 */

let ram = new RAM(256);
let cpu = new CPU(ram);
let keyboard = new KEYBOARD();
keyboard.connectToCPU(cpu);

// Get remaining command line arguments and even include the name of the
// program you are running. Slice cuts all of that off so we only get what 
// we need
const argv = process.argv.slice(2);

// Check arguments
if (argv.length === 0) {
    // Read from stdin
    loadFileFromStdin(cpu, onFileLoaded);
} else if (argv.length == 1) {
    // Read from file and onFileLoaded is what we call when we are done.
    loadFile(argv[0], cpu, onFileLoaded);
} else {
    console.error('usage: ls8 [machinecodefile]');
    process.exit(1);
}
