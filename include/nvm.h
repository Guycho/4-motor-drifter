#ifndef NVM_H
#define NVM_H

#include <EEPROM.h>

struct EepromData {};

class Eeprom {
   public:
    Eeprom();      // Constructor
    ~Eeprom();     // Destructor
    void init();   // Method to start the EEPROM
    void write();  // Method to write the EEPROM
    void read();   // Method to read the EEPROM
   private:
    EepromData m_data;  // Variable to store the EEPROM data
};
#endif  // NVM_H