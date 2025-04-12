#ifndef DATA_UNIT_H
#define DATA_UNIT_H
#include <chrono>
#include <cstdint>
#include <vector>



class Data_Unit {
    static constexpr uint8_t PAYLOAD_66=0x66;
    static constexpr  uint8_t PAYLOAD_FF=0xFF;
    static constexpr uint8_t PAYLOAD_01=0x01;

    std::string raw;
    std::vector<uint8_t> payload;

    template <typename T>
        T parser(const std::string& s);

public:
    long long timestamp;
    uint16_t id;
    void parse();
    static constexpr uint16_t ID_SS =0x0A0;
    Data_Unit(const std::string& msg,const long long& ts);
    Data_Unit(const Data_Unit& other);

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
