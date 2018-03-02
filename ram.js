/**
 * RAM access
 */
class RAM {
    /**
     * Constructor
     * 
     * @param {*} size Size of RAM in bytes MUST be a power of 2
     */
    constructor(size) {
        this.mem = new Array(size);
        this.mem.fill(0);
    }

    /**
     * Write (store) MDR value at address MAR
     */
    write(MAR, MDR) {
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
        this.mem[MAR] = MDR;
        // console.log('MAR = ', MAR, 'MDR = ', MDR, 'mem[MAR]', this.mem[MAR])
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * returns MDR
     */
    read(MAR) {
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
        return this.mem[MAR];
    }
}

module.exports = RAM;