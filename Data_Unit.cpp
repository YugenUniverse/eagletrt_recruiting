#include "Data_Unit.h"


Data_Unit::Data_Unit(const std::string& msg) {
    timestamp = time(nullptr);
    auto hashPos = msg.find('#');
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

bool Data_Unit::isStart() {
    if (id !=ID_SS) return false;
    if (payload.size() != 2) return false;
    //valid: 6601, FF01
    if (payload[0] != PAYLOAD_B_1 && payload[0] != PAYLOAD_B_2) return false;
    if (payload[1] != PAYLOAD_B_3) return false;
    return true;
}

bool Data_Unit::isStop() {
    if (id !=ID_SS) return false;
    if (payload.size() != 2) return false;
    //valid: FF66
    if (payload[0] != PAYLOAD_B_2 && payload[1] != PAYLOAD_B_1) return false;
    return true;
}

std::string Data_Unit::getLog() const{
    std::stringstream ss;
    ss<<"("<<timestamp<<")";
    ss<<std::hex<<std::uppercase<<std::setfill('0')<<std::setw(3)<<id<<"#";
    for (const auto& byte : payload) {
        ss<<static_cast<int>(byte);
    }
    return ss.str();
}


std::ostream &operator<<(std::ostream &os, const class Data_Unit &unit) {
    os<<"ID: "<<unit.id<<" Payload: ";
    for (const auto& byte : unit.payload) {
        os<<static_cast<int>(byte)<<" ";
    }
    return os;
}
