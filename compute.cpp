#include "compute.h"

using namespace std;

Status state = IDLE;
const string&& path ="../logs/";
ofstream file;
map<uint16_t,Stat_Unit> ids;
int i=-1;


void compute(bool& running,queue<Data_Unit>& buffer,mutex& mtx,condition_variable& cv) {
    while (running||!buffer.empty()) {
        Data_Unit* ptrUnit=nullptr;
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return !buffer.empty() || !running; });
        if (!buffer.empty()) {
            //Critical section
            {
                ptrUnit = new Data_Unit(buffer.front());
                buffer.pop();
            }
        }
        //Log/Parse
        try {
            log(*ptrUnit);
            ptrUnit->parse();
            cout <<*ptrUnit<<endl;
        }catch (invalid_argument& e) {
            cerr << e.what() << endl;
        }

        if (ptrUnit != nullptr) {
            switch (state) {
                case IDLE:
                    if (ptrUnit->isStart()) {
                        state= RUN;
                        cout<<"start"<<endl;
                        cerr<<newSession();
                        log(*ptrUnit);
                        addTime(Data_Unit::ID_SS,ptrUnit->timestamp);
                    }
                    break;
                case RUN:
                    addTime(ptrUnit->id,ptrUnit->timestamp);
                    if (ptrUnit->isStop()) {
                        state=IDLE;
                        cout<<"stop"<<endl;
                        cerr<<endSession();
                    }
                    break;
            }
            delete ptrUnit;
        }
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

bool endSession() {
    file.close();
    ofstream stat;
    stat.open(string("../stat/session"+to_string(i)+".csv"));
    if (!stat.is_open()) {
        return false;
    }
    for (const auto& x:ids) {
        stat<<std::hex<<uppercase<<setfill('0')<<setw(3)<<x.first<<",";
        stat<<std::dec<<x.second.getCVS()<<endl;
    }
    stat.close();
    ids.clear();
    return true;
}

void addTime(const uint16_t& id,const long long& ts) {
    auto it = ids.find(id);
    if (it!=ids.end())
        it->second.addTime(ts);
    else
        ids.emplace(id,Stat_Unit(0,ts));
}