#include "nvm.h"

NVM::NVM() {
    // Constructor implementation
}

NVM::~NVM() {
    // Destructor implementation
}

void NVM::init() {
    EEPROM.begin(sizeof(NVMData));
    read();
}

void NVM::write() {
    EEPROM.put(0, m_data);
    EEPROM.commit();
}

void NVM::read() { EEPROM.get(0, m_data); }

void NVM::checkForChanges() {
    if (memcmp(&m_data, &m_last_data, sizeof(NVMData)) != 0) {
        write();
        m_last_data = m_data;  // Update the last known state
    }
}