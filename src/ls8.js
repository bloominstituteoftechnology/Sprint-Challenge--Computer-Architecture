const fs = require('fs');
const RAM = require('./ram');
const CPU = require('./cpu');

function processFile(content, cpu, onComplete) {
    let curAddr = 0;
  
    const lines = content.split('\n');
    
    for (let line of lines) {
        let hashIndex = line.indexOf('#');

        if (hashIndex !== -1) {
            line = line.substr(0, hashIndex);
        }
        line = line.trim();
        
        if (line.length === 0) {
            continue;
        }
      
        let binaryConvert = parseInt(line, 2);
      
        cpu.poke(curAddr, binaryConvert)
        
        curAddr++;
    }

    onComplete(cpu);
}


function loadFileFromStdin(cpu, onComplete) {
    let content = '';

    process.stdin.resume();
    process.stdin.on('data', function(buf) { content += buf.toString(); });
    process.stdin.on('end', () => { processFile(content, cpu, onComplete); });
}

function loadFile(filename, cpu, onComplete) {
    const content = fs.readFileSync(filename, 'utf-8');
    processFile(content, cpu, onComplete);
}

function onFileLoaded(cpu) {
    cpu.startClock();
}


let ram = new RAM(256);
let cpu = new CPU(ram);

const argv = process.argv.slice(2);

if (argv.length === 0) {
    loadFileFromStdin(cpu, onFileLoaded);
} else if (argv.length == 1) {
    loadFile(argv[0], cpu, onFileLoaded);
} else {
    console.error('usage: ls8 [machinecodefile]');
    process.exit(1);
}
