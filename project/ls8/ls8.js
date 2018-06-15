const RAM = require('./ram');
const CPU = require('./cpu');

const fs = require('fs');
const args = process.argv;

function loadMemory() {

    var regexp = /[01]{8}/g;
    const data = fs.readFileSync(`${args[2]}`, "utf-8").match(regexp); // result an array of only numerical values 

    //Load the program into the CPU's memory a byte at a time
    for (let i = 0; i < data.length; i++) {
        // console.log(data[i])
        cpu.poke(i, parseInt(data[i], 2));// call poke with index, data[i] (in binary)
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