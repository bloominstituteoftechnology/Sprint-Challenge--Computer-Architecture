/**
 * RAM access
 */
class RAM {
    constructor(size) {
        this.mem = new Array(size);
        this.mem.fill(0);
    }

    /**
     * Write (store) MDR value at address MAR
     */
    //&&& MAR will be a binary code that gets converted to a number
    //&&& MAR can be 0 up to 256 for 8-bit binary ex: 00000000 or 00000001
    //&&& MDR is the value. a binary value that gets converted to a number
    write(MAR, MDR) {
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
        this.mem[MAR] = MDR;
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * @returns MDR
     */
    read(MAR) {
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
        return this.mem[MAR]
    }
}

module.exports = RAM;