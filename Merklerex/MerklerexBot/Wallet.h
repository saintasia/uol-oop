#pragma once
#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet {
  public:
    Wallet();
    /** Insert currencty to the wallet */
    void insertCurrency(std::string type, double amount);
    /** Remove currencty from the wallet, success/fail */
    bool removeCurrency(std::string type, double amount);
    /** Check if the wallet contains that much or more currency */
    bool containsCurrency(std::string type, double amount);
    /** Check if the wallet can fulfill the order */
    bool canFulfillOrder(const OrderBookEntry& order);
    /** Update the contents of the wallet
     * assumes the order was made by the owner of the wallet
     */
    void processSale(OrderBookEntry& sale);
    /** Generate a string representation of wallet contents */
    std::string toString();

  private:
    std::map<std::string, double> currencies;
};
