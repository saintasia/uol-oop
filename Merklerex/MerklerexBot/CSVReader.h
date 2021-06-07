#pragma once
#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader {
  public:
    CSVReader();
    // static is a local function with no instance
    static std::vector<OrderBookEntry> readCSV(std::string scvFile);
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    static OrderBookEntry stringsToOBE(
      std::string price,
      std::string amount,
      std::string timestamp,
      std::string product,
      OrderBookType orderType
    );
  
  private:
    static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};
