#include <iostream>
#include <vector>
#include <fstream>
#include "CSVReader.h"
#include "OrderBookEntry.h"

CSVReader::CSVReader(){}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string filename){
  std::vector<OrderBookEntry> entries;

  std::ifstream csvFile{filename};
  std::string line;

  if(csvFile.is_open()){
    while(std::getline(csvFile, line)){
      try{
        OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
        entries.push_back(obe);
      }catch(const std::exception& e){
        std::cout << "Bad data" << std::endl;
      }
    }
  }
  
  // csvFile.close();

  std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
  return entries;
};


std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator){
  std::vector<std::string> tokens;

  signed int start, end;

  std::string token;

  start = csvLine.find_first_not_of(separator, 0);

  do{
    end = csvLine.find_first_of(separator, start);
    if(start == csvLine.length() || start == end) break;
    if(end >= 0) token = csvLine.substr(start, end - start);
    else token = csvLine.substr(start, csvLine.length() - start);
    start = end + 1;

    tokens.push_back(token);
  }

  while(end > 0);

  return tokens;
};


OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens){
  double price, amount;

  // check number of items
  if(tokens.size() != 5) {
    std::cout << "CSVReader::stringsToOBE Bad line " << std::endl;
    throw std::exception{};
  }

  //we have 5 tokens stod = string to double
  try {
    price = std::stod(tokens[3]);
    amount = std::stod(tokens[4]);
  }catch(const std::exception& e){
    std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << tokens[4] << std::endl;
    throw;
  }

  OrderBookEntry obe{
    price,
    amount,
    tokens[0],
    tokens[1],
    OrderBookEntry::stringToOrderBookType(tokens[2])
  };

  return obe;
};

OrderBookEntry CSVReader::stringsToOBE(
  std::string priceString,
  std::string amountString,
  std::string timestamp,
  std::string product,
  OrderBookType orderType
){
  double price, amount;
  try {
    price = std::stod(priceString);
    amount = std::stod(amountString);
  }catch(const std::exception& e){
    std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
    std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
    throw;
  }

  OrderBookEntry obe{
    price,
    amount,
    timestamp,
    product,
    orderType
  };

  return obe;
};
