#pragma once
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdint.h>

namespace mock {
class CEEPROM {
    static const int MEM_SIZE = 4096;
    uint8_t mem[MEM_SIZE];

    void checkBounds(int addr) {
        if (addr < 0 || addr >= MEM_SIZE) {
            printf("Memory access out of bounds\n");
            exit(1);
        }
    }

  public:
    CEEPROM() {
        try {
            std::ifstream memory("EEPROM", std::ios::binary | std::ios::in);

            if (!memory)
                throw std::runtime_error("Could not load file");

            char c;

            for (unsigned i = 0; memory && i < MEM_SIZE; i++) {
                memory.get(c);
                mem[i] = (uint8_t)c;
            }
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    };

    uint8_t read(int addr) {
        checkBounds(addr);
        return mem[addr];
    }

    void write(int addr, uint8_t val) {
        checkBounds(addr);
        mem[addr] = val;

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

    void update(int addr, uint8_t val) {
        if (read(addr) != val) {
            write(addr, val);
        }
    }

    template <typename T> T &get(int addr, T &t) {
        uint8_t *ptr = (uint8_t *)&t;
        for (int count = sizeof(T); count > 0; --count)
            *ptr++ = read(addr++);
        return t;
    }

    template <typename T> const T &put(int addr, const T &t) {
        const uint8_t *ptr = (const uint8_t *)&t;
        for (int count = sizeof(T); count; --count)
            update(addr++, *ptr++);
        return t;
    }
};
}

extern mock::CEEPROM EEPROM;