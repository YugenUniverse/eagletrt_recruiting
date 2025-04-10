//
// Created by yugen on 4/6/25.
//

#ifndef DATA_UNIT_H
#define DATA_UNIT_H
#include <cstdint>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>



class Data_Unit {
    const static uint16_t ID_SS =0x0A0;
    const static uint8_t PAYLOAD_B_1=0x66;
    const static uint8_t PAYLOAD_B_2=0xFF;
    const static uint8_t PAYLOAD_B_3=0x01;

    std::string raw;
    uint16_t id;
    std::vector<uint8_t> payload;
    time_t timestamp;

    void parse(const std::string& raw);
    template <typename T>
        T parser(const std::string& s);

public:
    Data_Unit(const std::string& msg,const time_t& ts);

    bool isStart() const;
    bool isStop() const;

    std::string getLog() const;

    friend std::ostream& operator<<(std::ostream& os, const Data_Unit& unit);
};

template<typename T>
T Data_Unit::parser(const std::string& s) {
    return static_cast<T>(std::stoul(s,nullptr,16));
}

#endif //DATA_UNIT_H
