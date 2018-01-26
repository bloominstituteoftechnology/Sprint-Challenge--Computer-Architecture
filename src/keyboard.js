const keyboardInterrupt = 2;

class KEYBOARD {
  constructor() {
    let stdin = process.stdin;
    stdin.setRawMode(true);
    stdin.resume();
    stdin.setEncoding('utf-8');
    stdin.on('data', (key) => {
      if (key === `\u0003`) {
        process.exit();
      }
      this.handleKey(key);
    });
  };
  connectToCPU(cpu) {
    this.cpu = cpu;
  };
  handleKey(key) {
    this.cpu.poke(0x7f, key.charCodeAt(0));
    this.cpu.raiseInterrupt(keyboardInterrupt);
  };
};

module.exports = KEYBOARD;