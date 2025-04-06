//
// Created by yugen on 4/6/25.
//
#include "Data_Unit.h"

Data_Unit::Data_Unit(std::string msg) {
    auto hashPos = msg.find("#");
    if (hashPos == std::string::npos) {
        throw std::invalid_argument("# not found");
    }

    std::string s_id = msg.substr(0, hashPos);
    std::string s_payload = msg.substr(hashPos + 1);

    id = parse<uint16_t>(s_id);
    for (int i=0; i<s_payload.length(); i+=2) {
        payload.push_back(parse<uint8_t>(s_payload.substr(i, 2)));
    }
}

std::ostream &operator<<(std::ostream &os, const class Data_Unit &unit) {
    os<<"ID: "<<unit.id<<" Payload: ";
    for (const auto& byte : unit.payload) {
        os<<static_cast<int>(byte)<<" ";
    }
    return os;
}
