#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

// new type Orderbook with enum
enum class LogType{askbid, assets, sale, unknown};
class Logger{
  public:
    Logger();
    /** Add a string to log file depending on type */
    void add(std::string text, LogType type = LogType::unknown);
    /** Clear all text log files */
    void clear();
};
