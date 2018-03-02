const keyboardInterrupt = 2;

class Keyboard {
    constructor() {
        const stdin = process.stdin;

        // without this, we would only get streams once enter is pressed
        stdin.setRawMode( true );

        // resume stdin in the parent process (node app won't quit all by itself
        // unless an error or process.exit() happens)
        stdin.resume();

        // i don't want binary, do you?
        stdin.setEncoding( 'utf8' );

        // on any data into stdin
        stdin.on( 'data', ( key ) => {
            // ctrl-c ( end of text )
            if ( key === '\u0003' ) {
                process.exit();
            }
            this.handleKey(key);
        });
    }

    connectToCPU(cpu) {
        this.cpu = cpu;
    }

    handleKey(key) {
        // console.log("keyboard: " + key);
        this.cpu.poke(0x7f, key.charCodeAt(0));
        this.cpu.raiseInterrupt(keyboardInterrupt);
    }
}

module.exports = Keyboard;