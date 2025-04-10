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

const char* filepath="../candump.log";
bool running=true;
mutex mtx;
condition_variable cv;

void read(queue<string>& r,queue<time_t>& t) {
    open_can(filepath);
    while (running){
        try {
            char raw[MAX_CAN_MESSAGE_SIZE];
            int bytes=can_receive(raw);
            {
                unique_lock<mutex> lock(mtx);
                r.emplace(raw, bytes);
                t.emplace(time(nullptr));
            }
            cv.notify_one();
        }catch(const length_error& e) {
            cerr << e.what() << endl;
            running=false;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    close_can();
}

int main() {
    queue<string> raw_queue;
    queue<time_t> timestamp_queue;
    thread t1([&raw_queue,&timestamp_queue](){read(raw_queue,timestamp_queue);});
    thread t2([&raw_queue,&timestamp_queue](){compute(running,raw_queue,timestamp_queue,mtx,cv);});

    cout<<"Press ENTER for stop running"<<endl;
    cin.get();
    running=false;
    t1.join();
    t2.join();
    return 0;
}