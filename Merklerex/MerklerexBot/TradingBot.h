#pragma once
#include <vector>
#include <queue>
#include <tuple>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "Logger.h"

class TradingBot {
  public:
    /** trading bot constructor */
    TradingBot();
    /** Call to start the bot sim */
    void init();
  private:
    void runSimulator();
    void printWallet();
    void goToNextTimeFrame();
    std::tuple<float, float, float, float, float> getStats(std::string const product);
    void makeBids(std::string const product, float mean);
    void makeAsks(std::string const product, float mean);
    bool isProfitable(float price, std::string product, OrderBookType orderType);
    std::string currentTime;
    std::string maxTime;
    std::deque<std::string> pastTimeStamps;
    OrderBook orderBook{"20200601.csv"};
    Wallet wallet;
    Logger logger;
};
