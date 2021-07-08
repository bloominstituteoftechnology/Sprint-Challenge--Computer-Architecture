const RAM = require('./ram')
const CPU = require('./cpu')

async function runProcedure() {
  /**
   * Load an LS8 program into memory
   */
  const program = String(await require('fs').promises.readFile(process.argv[2]))
    .split('\n')
    .map(str => (str.indexOf('#') > -1 ? str.substr(0, 8) : str))
    .filter(str => str.indexOf('#') === -1)
    .filter(str => str !== '')
    
  /**
   * Main
   */
  let ram = new RAM(256)
  let cpu = new CPU(ram)

  // Load the program into the CPU's memory a byte at a time
  for (let i = 0; i < program.length; i++) {
    cpu.poke(i, parseInt(program[i], 2))
  }

  cpu.startClock()
}

runProcedure()
