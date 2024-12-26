#ifndef NVM_H
#define NVM_H

#include <EEPROM.h>

struct NVMData {
    // Add your data members here
};

class NVM {
   public:
    NVM();         // Constructor
    ~NVM();        // Destructor
    void init();   // Method to start the NVM
    void write();  // Method to write the NVM
    void read();   // Method to read the NVM
    void checkForChanges();  // Method to check for changes and write if necessary

   private:
    NVMData m_data;  // Variable to store the NVM data
    NVMData m_last_data;  // Variable to store the last known state of the EEPROM data
};

#endif  // NVM_H