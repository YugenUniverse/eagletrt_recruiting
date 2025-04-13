#ifndef STAT_UNIT_H
#define STAT_UNIT_H
#include <chrono>
#include <string>

/**
 * @class Stat_Unit
 * @brief Tracks the last two timestamps and counts message entries; provides CSV output.
 * @details The CSV includes the delta time (in milliseconds) between the two most recent timestamps.
 */
class Stat_Unit {
    /**@brief counter of the number of message*/
    int messageCount=0;

    /**
    * @brief Stores the last two message timestamps.
    * @details Always contains exactly two valid values (in microseconds).
    */
    long long lastTwo[2]{};
    public:

    /**
     * @brief Constructor new Stat_Unit object
     *
     * @param t1,t2 timestamps in microsecond accuracy
     */
    Stat_Unit(const long long& t1,const long long& t2);

    /**
    * @brief add to lastTwo, increase the counter
    *
    * @param t time t add
    */
    void addTime(const long long& t);

    /**
    * @brief calculate the mean time in milliseconds and form the CSV msg
    *
    * @return get the formatted CSV msg as string
    */
    std::string getCSV() const;
};
#endif //STAT_UNIT_H
