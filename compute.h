//
// Created by yugen on 4/6/25.
//

#ifndef COMPUTE_H
#define COMPUTE_H
#include <condition_variable>
#include <queue>
#include <fstream>
#include <iostream>
#include <mutex>
#include "Data_Unit.h"

enum Status {IDLE,RUN};
void compute(bool& running,std::queue<std::string>& raws,std::queue<time_t>& times, std::mutex& mtx,std::condition_variable& cv);
void log(const std::string& s,const time_t& ts);
void log(const Data_Unit &unit);
bool newSession();
#endif //COMPUTE_H
