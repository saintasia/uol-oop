#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <queue>
#include <algorithm>
#include <vector>

class OrderBook {
  public:
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all known products in the dataset */
    std::vector<std::string> getKnownProducts();
    /** return vector of orders according to the sent filters */
    std::vector<OrderBookEntry> getOrders(
      OrderBookType type,
      std::string product,
      std::string timestamp,
      std::string username = ""
    );
    /** return vector of orders according to the sent filters up until the current timestamp */
    std::vector<OrderBookEntry> getLiveOrders(
      OrderBookType type,
      std::string product,
      std::deque<std::string> pastTimeStamps
    );
    /** returns the earliest(first) time in the order book */
    std::string getEarliestTime();
    /** returns the latest(last) time in the order book */
    std::string getLatestTime();
    /** returns the next time after whatever you pass to it
     * if there's no next line, sets timestamp back to the first element
    */
    std::string getNextTime(std::string timestamp);

    /** inserst an order */
    void insertOrder(OrderBookEntry& order);
    /** remove an order */
    void removeOrder(OrderBookEntry& order);
    /** process the given timeframe's asks to bids */
    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    // static as it doesn't need to look at any other data in class
    static double getHighPrice(std::vector<OrderBookEntry>& orders);
    static double getLowPrice(std::vector<OrderBookEntry>& orders);
    static double getMeanPrice(std::vector<OrderBookEntry>& orders);

  private:
    std::vector<OrderBookEntry> orders;
};
