#include "Stat_Unit.h"

Stat_Unit::Stat_Unit(const long long &t1, const long long &t2) {
    messageCount=1;
    lastTwo[0]=t1;
    lastTwo[1]=t2;
}

void Stat_Unit::addTime(const long long &t) {
    messageCount++;
    lastTwo[0]=lastTwo[1];
    lastTwo[1]=t;
}

std::string Stat_Unit::getCSV() const {
    double delta;
    if ((delta=lastTwo[1]-lastTwo[0])==lastTwo[1]) delta = -1;
    else delta/=1000;
    delta=static_cast<long>(delta*100)/100;
    std::stringstream ss;
    ss<<std::fixed<<std::setprecision(2)<<delta;
    return std::to_string(messageCount)+","+ss.str();
}