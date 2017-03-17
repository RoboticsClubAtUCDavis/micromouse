#pragma once
#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdint.h>

namespace mock {
class CEEPROM {
    static const int MEM_SIZE = 4096;
    std::array<uint8_t, MEM_SIZE> mem;

  public:
    CEEPROM();
    uint8_t read(int addr);
    void write(int addr, uint8_t val);
    void update(int addr, uint8_t val);
    template <typename T> T &get(int addr, T &t);
    template <typename T> const T &put(int addr, const T &t);
};

template <typename T> T &CEEPROM::get(int addr, T &t) {
    uint8_t *ptr = (uint8_t *)&t;
    for (int count = sizeof(T); count > 0; --count)
        *ptr++ = read(addr++);
    return t;
}

template <typename T> const T &CEEPROM::put(int addr, const T &t) {
    const uint8_t *ptr = (const uint8_t *)&t;
    for (int count = sizeof(T); count; --count)
        update(addr++, *ptr++);
    return t;
}
}

extern mock::CEEPROM EEPROM;
