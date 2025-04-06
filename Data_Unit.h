//
// Created by yugen on 4/6/25.
//

#ifndef DATA_UNIT_H
#define DATA_UNIT_H
#include <iostream>
#include <cstdint>
#include <vector>

class Data_Unit {
    const static uint16_t ID_SS =0x0A0;
    const static uint32_t PAYLOAD_START_1=0x6601;
    const static uint32_t PAYLOAD_START_2=0xFF01;
    const static uint32_t PAYLOAD_STOP=0x66FF;

    uint16_t id;
    std::vector<uint8_t> payload;


public:

    Data_Unit(std::string msg);
    template <typename T>
    T parse(std::string s);
    friend std::ostream& operator<<(std::ostream& os, const Data_Unit& unit);
};


template<typename T>
T Data_Unit::parse(std::string s) {
    return static_cast<T>(std::stoul(s,nullptr,16));
}
#endif //DATA_UNIT_H
