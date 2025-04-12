#ifndef STAT_UNIT_H
#define STAT_UNIT_H
#include <chrono>
#include <string>

class Stat_Unit {
    int numMsg=0;
    long long lastTwo[2];
    public:
    Stat_Unit(const long long& t1,const long long& t2);
    void addTime(const long long& t);
    std::string getCVS() const;
};
#endif //STAT_UNIT_H
