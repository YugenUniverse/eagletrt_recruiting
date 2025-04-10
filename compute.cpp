#include "compute.h"
using namespace std;
Status state = IDLE;
const string&& path ="../logs/";
ofstream file;
int i=-1;


void compute(bool& running,queue<string>& raws,queue<time_t>& times,mutex& mtx,condition_variable& cv) {
    while (running||!raws.empty()) {
        Data_Unit *ptrUnit=nullptr;
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return !raws.empty() && !times.empty() || !running; });
        if (!raws.empty()&&!times.empty()) {
            //Critical section
            try {
                log(raws.front(),times.front());
                ptrUnit= new Data_Unit(raws.front(),times.front());
                raws.pop();
                times.pop();
                cout <<*ptrUnit<<endl;
                if (ptrUnit->isStop()) cerr<<"Stop"<<endl;
            }catch (invalid_argument& e) {
                cerr << e.what() << endl;
            }
        }
        if (ptrUnit != nullptr) {
            switch (state) {
                case IDLE:
                    if (ptrUnit->isStart()) {
                        state= RUN;
                        cout<<"start"<<endl;
                        cout<<newSession();
                        log(*ptrUnit);
                    }
                    break;
                case RUN:
                    if (ptrUnit->isStop()) {
                        state=IDLE;
                        cout<<"stop"<<endl;
                        file.close();
                    }
                    break;
            }
            delete ptrUnit;
        }
    }
}
void log(const string& s,const time_t& ts) {
    if (state==RUN&&file.is_open()) {
        file<<"("<<ts<<")"<<s<<endl;
    }
}
void log(const Data_Unit& unit) {
    if (file.is_open()) {
        file<<unit.getLog()<<endl;
    }
}

bool newSession() {
    if (!file.is_open()) {
        file.open (string(path+"session_"+to_string(++i)+".log"));
    }
    return file.is_open();
}