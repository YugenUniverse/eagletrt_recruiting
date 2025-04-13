#include "compute.h"

using namespace std;
map<uint16_t,Stat_Unit> ids;
int i=-1;


void compute(bool& running,queue<Data_Unit>& buffer,mutex& mtx,condition_variable& cv) {
    ofstream file;
    Status state = IDLE;
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
            log(file,*ptrUnit);
            ptrUnit->parse();
            cout<<*ptrUnit<<endl;
        }catch (invalid_argument& e) {
            cerr << e.what() << endl;
        }

        if (ptrUnit != nullptr) {
            switch (state) {
                case IDLE:
                    if (ptrUnit->isStart()) {
                        state= RUN;
                        cout<<"start"<<endl;
                        cerr<<newSession(file)<<endl<<endl;
                        log(file,*ptrUnit);
                        recordTime(ptrUnit->id,ptrUnit->timestamp);
                    }
                    break;
                case RUN:
                    recordTime(ptrUnit->id,ptrUnit->timestamp);
                    if (ptrUnit->isStop()) {
                        state=IDLE;
                        cout<<"stop"<<endl;
                        cerr<<endSession(file)<<endl<<endl;
                    }
                    break;
            }
            delete ptrUnit;
        }
    }
    endSession(file);
}

void log(ofstream& file,const Data_Unit& unit) {
    if (file.is_open()) {
        file<<unit.getLog()<<endl;
    }
}

bool newSession(ofstream& file) {
    if (!file.is_open()) {
        file.open (string("../logs/log_"+getTimestamp()+".log"));
    }
    return file.is_open();
}

bool endSession(ofstream& file) {
    file.close();
    ofstream stat;
    stat.open(string("../stats/stat_"+getTimestamp()+".csv"));
    if (!stat.is_open()) {
        return false;
    }
    for (const auto& x:ids) {
        stat<<std::hex<<uppercase<<setfill('0')<<setw(3)<<x.first<<",";
        stat<<std::dec<<x.second.getCSV()<<endl;
    }
    stat.close();
    ids.clear();
    return true;
}

void recordTime(const uint16_t& id,const long long& ts) {
    auto it = ids.find(id);
    if (it!=ids.end())
        it->second.addTime(ts);
    else
        ids.emplace(id,Stat_Unit(0,ts));
}

string getTimestamp() {
    auto time_t_now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm tm_now;
    localtime_r(&time_t_now, &tm_now);
    std::stringstream ss;
    ss<< std::put_time(&tm_now, "%Y_%m_%d_%H_%M_%S");
    return ss.str();
}
