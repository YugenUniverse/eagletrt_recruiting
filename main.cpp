#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include "Data_Unit.h"

using namespace std;

extern "C"{
    #include "fake_receiver.h"
}

const char* filepath="/home/yugen/Desktop/eagletrt_recruiting/candump.log";
bool running=true;
mutex mtx;

void read(queue<Data_Unit>& b) {
    open_can(filepath);
    while (running){
        char raw[MAX_CAN_MESSAGE_SIZE];
        int bytes=can_receive(raw);
        lock_guard<mutex> lock(mtx);
        try{
            b.push(Data_Unit(string(raw, bytes)));
        }catch (invalid_argument& e) {
            cerr<<e.what()<<endl;
        }
    }
    close_can();
}

void compute(queue<Data_Unit>& b) {
    while (running) {
        if (!b.empty()) {
            lock_guard<mutex> lock(mtx);
            cout<<b.front()<<endl;
            b.pop();
        }
    }
}


int main(void) {
    queue<Data_Unit> buffer;
    thread t1([&buffer](){read(buffer);});
    thread t2([&buffer](){compute(buffer);});

    cout<<"Press ENTER for stop running"<<endl;
    cin.get();

    running=false;
    t1.join();
    t2.join();
    return 0;
}