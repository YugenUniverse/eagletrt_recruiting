#ifndef DATA_UNIT_H
#define DATA_UNIT_H
#include <chrono>
#include <cstdint>
#include <vector>

/**
 * @class Data_Unit
 * @brief Represents a CAN message and provides parsing and log formatting.
 * @details Stores raw data and parsed data, identify messages (start/stop).
 */

class Data_Unit {
    /**@brief value for the start and stop recognition*/
    static constexpr uint16_t ID_SS =0x0A0;
    static constexpr uint8_t PAYLOAD_66=0x66;
    static constexpr uint8_t PAYLOAD_FF=0xFF;
    static constexpr uint8_t PAYLOAD_01=0x01;

    /**@brief raw msg from CAN*/
    std::string raw;
    /**@brief bytes of the parsed payload*/
    std::vector<uint8_t> payload;

    /**
     * @brief effectively parse the string in T
     *
     * @tparam T return type of the parser (uint16_t or uint8_t)
     * @param s hex digits string
     * @return s parsed in T
     */
    template <typename T>
        T parser(const std::string& s);

    public:
    /**@brief timestamp of the message reception from CAN, microsecond accuracy*/
    long long timestamp;
    /**@brief parsed id*/
    uint16_t id;

    /**
     * @brief Parse the raw CAN message and extract ID and payload
     */
    void parse();

    /**
     * @brief Constructor of a new Data_Unit object
     *
     * @param msg message from the CAN
     * @param ts timestamp of the message reception from CAN, microsecond accuracy
     */
    Data_Unit(const std::string& msg,const long long& ts);

    /**
     * @brief Copy Constructor of a new Data_Unit object equal to other
     * @param other source Data_Unit object
     */
    Data_Unit(const Data_Unit& other);

    /**
     * @brief check if the parsed value correspond to a start or stop
     * @return bool 1 if start or stop,0 if not
     */
    bool isStart() const;
    bool isStop() const;

    /**
     * @brief setup timestamp and row in the required way for log
     * @return string formatted log
     */
    std::string getLog() const;

    friend std::ostream& operator<<(std::ostream& os, const Data_Unit& unit);
};

template<typename T>
T Data_Unit::parser(const std::string& s) {
    return static_cast<T>(std::stoul(s,nullptr,16));
}

#endif //DATA_UNIT_H
