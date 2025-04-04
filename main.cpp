#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstring>
#include <queue>

using namespace std;

extern "C"{
    #include "fake_receiver.h"
}

class Data_Unit {
    int bytes;
    char message[MAX_CAN_MESSAGE_SIZE];
public:
    Data_Unit(int _bytes, char _message[MAX_CAN_MESSAGE_SIZE]): bytes(_bytes) {
        strcpy(message, _message);
    }
    ~Data_Unit(){}
    friend ostream& operator<<(ostream& os, const Data_Unit& unit);
};

ostream& operator<<(ostream& os, const Data_Unit& du) {
    os <<"Bytes:"<<du.bytes<<" message:"<< du.message;
    return os;
}

const char* filepath="/home/yugen/Desktop/eagletrt_recruiting/candump.log";
bool running=true;
mutex mtx;


void read(queue<Data_Unit>& b) {
    open_can(filepath);
    while (running){
        int bytes;
        char message[MAX_CAN_MESSAGE_SIZE];
        bytes=can_receive(message);
        lock_guard<mutex> lock(mtx);
        b.push(Data_Unit(bytes, message));
    }
    close_can();
}

void print(queue<Data_Unit>& b) {
    while (running) {
        lock_guard<mutex> lock(mtx);
        if (!b.empty()) {
            cout<<b.front()<<endl;
            b.pop();
        }
    }
}

int main(void) {
    queue<Data_Unit> buffer;
    thread t1([&buffer](){read(buffer);});
    thread t2([&buffer](){print(buffer);});

    cout<<"Press ENTER for stop running"<<endl;
    cin.get();

    running=false;
    t1.join();
    t2.join();
    return 0;
}