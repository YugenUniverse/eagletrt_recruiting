//
// Created by yugen on 4/6/25.
//

#ifndef COMPUTE_H
#define COMPUTE_H
#include <queue>
#include <fstream>
#include <iostream>
#include <mutex>
#include "Data_Unit.h"

enum Status {IDLE,RUN};
void compute(const bool& running,std::queue<std::string>& b, std::mutex& mtx);
void log(const Data_Unit& unit);
std::string newSession();
#endif //COMPUTE_H
