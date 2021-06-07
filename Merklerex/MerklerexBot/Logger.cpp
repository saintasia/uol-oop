#include <iostream>
#include <fstream>
#include "Logger.h"

Logger::Logger(){};

void Logger::add(std::string text, LogType type){
  // add to the main log
  std::ofstream mainLog;
  mainLog.open ("log.txt", std::ios_base::app);
  // also log out into the console
  std::cout << text << std::endl;
  mainLog << text << "\n";
  mainLog.close();
  // add to the ask-bid
  if (type == LogType::askbid) {
    std::ofstream askBidLog;
    askBidLog.open ("ask-bid.txt", std::ios_base::app);
    askBidLog << text << "\n";
    askBidLog.close();
  }
  // add to the purchase-sale
  if (type == LogType::sale) {
    std::ofstream saleLog;
    saleLog.open ("purchase-sale.txt", std::ios_base::app);
    saleLog << text << "\n";
    saleLog.close();
  }
  // add to the assets
  if (type == LogType::assets) {
    std::ofstream assetLog;
    assetLog.open ("assets.txt", std::ios_base::app);
    assetLog << text << "\n";
    assetLog.close();
  }
}

void Logger::clear(){
  // clear all logs
  std::ofstream myfile;
  myfile.open ("log.txt");
  myfile << "";
  myfile.close();
  // clear ask-bid
  std::ofstream askBidLog;
  askBidLog.open ("ask-bid.txt");
  askBidLog << "";
  askBidLog.close();
  // clear purchaseSale
  std::ofstream saleLog;
  saleLog.open ("purchase-sale.txt");
  saleLog << "";
  saleLog.close();
  // clear seets
  std::ofstream assetLog;
  assetLog.open ("assets.txt");
  assetLog << "";
  assetLog.close();
}
