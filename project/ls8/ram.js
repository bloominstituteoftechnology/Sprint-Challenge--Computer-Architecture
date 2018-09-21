
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
    write(MAR, MDR) {
        // !!! IMPLEMENT ME
        // write the value in the MDR to the address MAR
        this.mem[MAR] = MDR;
        //at the Memory Address Register, 
        //the value found is set to the Memory Data Register.
    }

    /**
     * Read (load) MDR value from address MAR
     * 
     * @returns MDR
     */
    read(MAR) {
        // !!! IMPLEMENT ME
        // Read the value in address MAR and return it
        return this.mem[MAR];
        //return what we just set above to 
        //read the MDR value now at MAR index
    }
}

module.exports = RAM;
