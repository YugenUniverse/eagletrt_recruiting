#ifndef COMPUTE_H
#define COMPUTE_H
#include <condition_variable>
#include <queue>
#include <map>
#include <fstream>
#include <iostream>
#include <mutex>
#include "Stat_Unit.h"
#include "Data_Unit.h"

enum Status {IDLE,RUN};
void compute(bool& running,std::queue<Data_Unit>& buffer, std::mutex& mtx,std::condition_variable& cv);
void log(const Data_Unit &unit);
void addTime(const uint16_t& id,const long long& ts);
bool newSession();
bool endSession();
#endif //COMPUTE_H
