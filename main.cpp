#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <queue>
#include "compute.h"

using namespace std;

extern "C"{
    #include "fake_receiver.h"
}


/**
 * @brief read from CAN through fake_receiver
 * @param running while loop keep alive
 * @param buffer queue computed by compute thread
 * @param mtx,cv same mutex and condition variable of compute thread for data race
 */
void read(bool& running,queue<Data_Unit>& buffer,mutex& mtx,condition_variable& cv);

/**
 * @brief return unix_timestamp of now in microsecond
 * @return long long timestamp in microsecond
 */
long long getNow();

int main() {
    bool running=true;
    mutex mtx;
    condition_variable cv;
    queue<Data_Unit> buffer;
    thread t1([&](){read(running,buffer,mtx,cv);});
    thread t2([&](){compute(running,buffer,mtx,cv);});

    cout<<"Press ENTER for stop running"<<endl;
    cin.get();
    running=false;
    t1.join();
    t2.join();
    return 0;
}

void read(bool& running,queue<Data_Unit>& buffer,mutex& mtx,condition_variable& cv) {
    const char*&& filepath="../candump.log";
    open_can(filepath);
    while (running){
            char raw[MAX_CAN_MESSAGE_SIZE];
            int bytes=can_receive(raw);
            if (bytes != -1){
                unique_lock<mutex> lock(mtx);
                buffer.emplace(string(raw,bytes), getNow());
            }
            else running=false;
            cv.notify_one();
    }
    close_can();
}

long long getNow() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}