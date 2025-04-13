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

/**@brief states of the FSM*/
enum Status {IDLE,RUN};

/**
* @brief run the FSM
*
* @param running while loop keep alive
* @param buffer queue loaded by read thread
* @param mtx,cv same mutex and condition variable of read thread for data race
*/
void compute(bool& running,std::queue<Data_Unit>& buffer, std::mutex& mtx,std::condition_variable& cv);

/**
* @brief log in to file the timestamp and raw msg
*
* @param file reference to the file where to log
* @param unit reference to the unit to log
*/
void log(std::ofstream& file,const Data_Unit &unit);

/**
* @brief record new entry and add to existing one new timestamp
*
* @param id parsed value and key in the record;
* @param ts time entry to record
*/
void recordTime(const uint16_t& id,const long long& ts);

/**
* @brief open new log session
*
* @param file reference to the new log file
* @return bool 1 if opened, 0 if not opened,
*/
bool newSession(std::ofstream& file);

/**
* @brief end if existing the log session, and produce the stats CSV file
*
* @param file reference to the log file
* @return bool 1 if the stats file is opened,0 if not opened
*/
bool endSession(std::ofstream& file);

std::string getTimestamp();

#endif //COMPUTE_H
