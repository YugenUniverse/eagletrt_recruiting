#include "compute.h"
using namespace std;
Status state = IDLE;
const string path ="../logs/";
string session;
ofstream file;
int i=0;


void compute(const bool& running,queue<string>& b,mutex& mtx) {
    while (running||!b.empty()) {
        Data_Unit *ptrUnit=nullptr;
        if (!b.empty()) {
            //Critical section
            lock_guard<mutex> lock(mtx);
            try {
                ptrUnit= new Data_Unit(b.front());
                b.pop();
            }catch (invalid_argument& e) {
                cerr << e.what() << endl;
            }
        }
        if (ptrUnit != nullptr) {
            cout <<*ptrUnit<<endl;
            switch (state) {
                case IDLE:
                    if (ptrUnit->isStart()) {
                        state= RUN;
                        cout<<"start"<<endl;
                        session=path+newSession();
                        file.open(session);
                        file.close();
                        log(*ptrUnit);
                    }
                    break;
                case RUN:
                    log(*ptrUnit);
                    if (ptrUnit->isStop()) {
                        state= IDLE;
                        cout<<"stop"<<endl;
                        session="";
                    }
                    break;
            }
            delete ptrUnit;
        }
    }
}
void log(const Data_Unit& unit) {
    file.open(session.c_str(),ios::app);
    file<<unit.getLog()<<endl;
    file.close();
}

string newSession() {
    i++;
    return string("session_"+to_string(i)+".log");
}

