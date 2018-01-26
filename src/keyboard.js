
const fs = require('fs');
const readline = require('readline');
// const Readline = new readline();
const krl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    terminal: true
});
const kkl = 'multi-keystroke.logger.txt';
var KKeystrokeOutput = null;
fs.unlink(kkl, (err) => KKeystrokeOutput = fs.openSync(kkl, 'w'));

const keyboardInterrupt = 2;
const keyAddress = 0xf7;
class Keyboard {
    constructor() {
        var stdin = process.stdin;

        // without this, we would only get streams once enter is pressed
        stdin.setRawMode(true);

        // resume stdin in the parent process (node aapp won't quit all by itself
        // unless an error or process.exit() happens)
        stdin.resume();

        // i don't want binary, do you?
        stdin.setEncoding('utf8');

        // on any data into stdin
        stdin.on('data', (key) => {
            // ctrl-c ( end of text )
            if (key === '\u0003') {
                process.exit();
            }
            this.handleKey(key);
        });
    }

    ConnectToCPU(cpu) {
        this.cpu = cpu;
    }

    handleKey(key) {
        if (key.length > 1) {
            for (let i = 0; i < key.length; i++) {
                fs.writeSync(KKeystrokeOutput,`${key[i].codePointAt(0).toString(16)} `)
            }
            fs.writeSync(KKeystrokeOutput,`\n`)
        }
        //console.log("keyboard: " + key);
        for (let i = 0; i < key.length; i++) {
            const Key = key[i];
            if (Key < 0x20 || key >= 0x7F) this.cpu.poke(keyAddress, Key); else
            this.cpu.poke(keyAddress, Key.charCodeAt(0));
            this.cpu.raiseInterrupt(keyboardInterrupt);
        }
    }
}

module.exports = Keyboard