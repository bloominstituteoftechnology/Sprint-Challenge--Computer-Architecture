/**
 * RAM access
 */
class RAM {
    constructor(size) {
        this.mem = new Array(size);
        this.mem.fill(0);
        // console.log(`RAM size: ${size}`);
    }

    /**
     * Write (store) MDR value at address MAR
     */
    write(MAR, MDR) {
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
        // console.log(`RAM write MAR: ${MAR}  MDR: ${MDR.toString(2)}`);
        this.mem[MAR] = MDR
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * @returns MDR
     */
    read(MAR) {
        return this.mem[MAR]
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
    }
}

module.exports = RAM;