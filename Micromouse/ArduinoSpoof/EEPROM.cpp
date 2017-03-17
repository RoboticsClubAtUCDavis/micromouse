#include "EEPROM.h"

mock::CEEPROM EEPROM;

inline mock::CEEPROM::CEEPROM() {
    try {
        std::ifstream memory("EEPROM", std::ios::binary | std::ios::in);

        if (!memory)
            throw std::runtime_error("Could not load file");

        char c;

        for (unsigned i = 0; memory && i < MEM_SIZE; i++) {
            memory.get(c);
            mem.at(i) = (uint8_t)c;
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

inline uint8_t mock::CEEPROM::read(int addr) {
    return mem.at(addr);
}

inline void mock::CEEPROM::write(int addr, uint8_t val) {
    mem.at(addr) = val;

    try {
        std::ofstream memory("EEPROM", std::ios::binary | std::ios::out);

        if (!memory)
            throw std::runtime_error("Could not create file");

        for (auto i : mem) {
            memory.write((char *)&i, 1);
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

inline void mock::CEEPROM::update(int addr, uint8_t val) {
    if (read(addr) != val) {
        write(addr, val);
    }
}
