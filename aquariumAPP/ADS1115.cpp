#include "ADS1115.h"

ADS1115::ADS1115()
{
}

ADS1115::~ADS1115()
{
}

bool ADS1115::openI2CBus(char *bus){
    if ((fd = open(bus, O_RDWR)) < 0) {
        std::cout << "Error: Couldn't open i2c device!: " << fd << std::endl;
        return false;
    }
    return true;
}

bool ADS1115::setI2CSlave(unsigned char deviceAddr){
    if (ioctl(fd, I2C_SLAVE, deviceAddr) < 0) {
        std::cout << "Error: Couldn't find i2c device on address!: " << fd << std::endl;
        return false;
    }
    return true;
}

bool ADS1115::readADS1115(int channel){
    
    // writeBuf -> set config register and start conversion
    uint8_t writeBuf[3];
    // readBuf -> stores the read data
    uint8_t readBuf[2];

    /* config register is 1 */
    writeBuf[0] = 1;
    
    /**  writeBuf[1]
    * bit 15 flag bit for single shot
    * Bits 14-12 input selection:
        100 A0; 101 A1; 110 A2; 111 A3
    * Bits 11-9 Amp gain. Default to 010 here 001 : FSR = ±4.096 V
    * Bit 8 Operational mode of the ADS1115.
        0 : Continuous conversion mode
        1 : Power-down single-shot mode (default) 
    */
    writeBuf[1] = (channel >= 0 && channel <= 3) ? (0xC3 | (channel << 3)) : (writeBuf[1]); //4

    /**  writeBuf[2]
    *Bits 7-5 data rate default to 100 for 128SPS
    *Bits 4-0  comparator function 
    */
    writeBuf[2] = 0x85; 

    /* begin conversion */
    if (write(fd, writeBuf, 3) != 3) {
        std::cout << "Write to register" << std::endl;
        return false;
    }

    /* wait for conversion complete */ 
    do {
        if (read(fd, readBuf, 2) != 2) {
            std::cout << "Read conversion" << std::endl;
            return false;
        }
    }
    while ((readBuf[0] & 0x80) == 0);   /* bit15 */

    // read conversion register
    // write register pointer first
    readBuf[0] = 0;   // conversion register is 0
    if (write(fd, readBuf, 1) != 1) {
        std::cout << "Write register select" << std::endl;
        return false;
    }

    // read 2 bytes
    if (read(fd, readBuf, 2) != 2) {
        std::cout << "Read conversion" << std::endl;
        return false;
    }

    int16_t val = readBuf[0] << 8 | readBuf[1];

    if (val < 0)
        val = 0;

    sensorValue = val * 4.096 / 32768.0; // convert to voltage

    close(fd);
    return true;
}

float ADS1115::getSensorValue()
{
    return sensorValue;
}
