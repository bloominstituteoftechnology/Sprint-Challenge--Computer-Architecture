const RAM = require('./ram');
const CPU = require('./cpu');
const fs = require('fs')

/**
 * Load an LS8 program into memory
 *
 * TODO: load this from a file on disk instead of having it hardcoded
 */
function loadMemory() {

    // Read the desired filename from the command line arguments
    const fileName = process.argv[2]
    if (!fileName) {
        console.log("Error: Provide a file name to load instructions into memory.")
        process.exit()
    }

    try {
        // Read the file from disk and grab only the instruction from lines that have one
        const program = fs.readFileSync(fileName, 'utf8')
            .split('\r\n')
            .filter(line => line.match(/\d{8}\b/))
            .map(line => line.slice(0, 8))
        
        // Convert each instruction to binary and have the CPU store it in RAM
        for (let i = 0; i < program.length; i++) {
            cpu.poke(i, parseInt(program[i], 2));
        }
    } catch (err) {
        console.log(`Error: No file found at ${fileName}`)
        console.log(err)
        process.exit()
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