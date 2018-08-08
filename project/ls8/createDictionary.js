const instructions = require('./instructions');
function createDictionary(cpu, first, second) {
  return {
    [instructions.LDI]: () => {
      cpu.reg[first] = second;
      cpu.PC += 3;
    },
    [instructions.HLT]: () => {
      cpu.stopClock();
      cpu.PC += 1;
    },
    [instructions.MUL]: () => {
      console.log(cpu.alu('MUL', first, second));
      cpu.stopClock();
    },
    [instructions.PRN]: () => {
      console.log(cpu.reg[first]);
      cpu.PC += 2;
    },
    [instructions.PUSH]: () => {
      cpu.SP--;
      cpu.poke(cpu.SP, second);
    },
    [instructions.POP]: () => {
      cpu.reg[first] = cpu.ram.read(cpu.SP);
      this.SP++;
    }
  };
}
exports.createTickDictionary = createDictionary;
