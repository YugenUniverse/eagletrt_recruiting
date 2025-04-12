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

void read(queue<Data_Unit>& buffer);
long long getNow();

int main() {
    queue<Data_Unit> buffer;
    thread t1([&buffer](){read(buffer);});
    thread t2([&buffer](){compute(running,buffer,mtx,cv);});

    cout<<"Press ENTER for stop running"<<endl;
    cin.get();
    running=false;
    t1.join();
    t2.join();
    return 0;
}

void read(queue<Data_Unit>& buffer) {
    open_can(filepath);
    while (running){
        try {
            char raw[MAX_CAN_MESSAGE_SIZE];
            int bytes=can_receive(raw);
            {
                unique_lock<mutex> lock(mtx);
                auto now=
                buffer.emplace(string(raw,bytes), getNow());
            }
            cv.notify_one();
        }catch(const length_error& e) {
            cerr << e.what() << endl;
            running=false;
        }
        this_thread::sleep_for(chrono::nanoseconds(5));
    }
    close_can();
}

long long getNow() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}