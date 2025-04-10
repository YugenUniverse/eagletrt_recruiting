#include "Data_Unit.h"

#include <iostream>


Data_Unit::Data_Unit(const std::string& msg,const time_t& ts) {
    timestamp = ts;
    raw = msg;
    parse(raw);
}

void Data_Unit::parse(const std::string& raw) {
    size_t hashPos = raw.find('#');
    if (hashPos == std::string::npos) {
        throw std::invalid_argument("# not found");
    }

    std::string s_id = raw.substr(0, hashPos);
    std::string s_payload = raw.substr(hashPos + 1);
    id = parser<uint16_t>(s_id);
    for (int i=0; i<s_payload.length(); i+=2) {
        payload.push_back(parser<uint8_t>(s_payload.substr(i, 2)));
    }
}

bool Data_Unit::isStart() const{
    if (id !=ID_SS) return false;
    if (payload.size() != 2) return false;
    //valid: 6601, FF01
    if (payload[0] != PAYLOAD_B_1 && payload[0] != PAYLOAD_B_2) return false;
    if (payload[1] != PAYLOAD_B_3) return false;
    return true;
}

bool Data_Unit::isStop() const{
    if (id !=ID_SS) return false;
    if (payload.size() != 2) return false;
    //valid: 66FF
    if (payload[0] != PAYLOAD_B_1) return false;
    if (payload[1] != PAYLOAD_B_2) return false;
    return true;
}

std::string Data_Unit::getLog() const{ return "("+std::to_string(timestamp)+")"+raw;}

std::ostream &operator<<(std::ostream &os, const class Data_Unit &unit) {
    os<<"ID: "<<unit.id<<" Payload: ";
    for (const auto& byte : unit.payload) {
        os<<static_cast<int>(byte)<<" ";
    }
    return os;
}
