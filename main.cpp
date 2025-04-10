#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include "compute.h"

using namespace std;

extern "C"{
    #include "fake_receiver.h"
}

const char* filepath="../candump.log";
bool running=true;
mutex mtx;

void read(queue<string>& b) {
    open_can(filepath);
    while (running){
        try {
            char raw[MAX_CAN_MESSAGE_SIZE];
            int bytes=can_receive(raw);
            lock_guard<mutex> lock(mtx);
            b.emplace(raw,bytes);
        }catch(const length_error& e) {
            cerr << e.what() << endl;
            running=false;
        }
    }
    close_can();
}

int main() {
    queue<string> buffer;
    thread t1([&buffer](){read(buffer);});
    thread t2([&buffer](){compute(running,buffer,mtx);});

    cout<<"Press ENTER for stop running"<<endl;
    cin.get();
    running=false;
    t1.join();
    t2.join();
    return 0;
}