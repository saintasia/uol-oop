#include "Wallet.h"
#include "CSVReader.h"
#include <iostream>

Wallet::Wallet(){};
/** Insert currencty to the wallet */
void Wallet::insertCurrency(std::string type, double amount){
  double balance;
  if(amount < 0){
    throw std::exception{};
  }
  //not there yet
  if (currencies.count(type) == 0){
    balance = 0;
  }
  else {
    balance = currencies[type];
  }
  balance += amount;
  currencies[type] = balance;
};

/** Check if the wallet contains that much or more currency */
bool Wallet::containsCurrency(std::string type, double amount){
  if (currencies.count(type) == 0) return false;
  else return currencies[type] >= amount;
};

/** Generate a string representation of wallet contents */
std::string Wallet::toString(){
  std::string s;
  for (std::pair<std::string, double> pair : currencies) {
    std::string currency = pair.first;
    double amount = pair.second;
    s += currency + " : " + std::to_string(amount) + "\n";
  }
  return s;
};

bool Wallet::removeCurrency(std::string type, double amount){
  double balance;
  if(amount < 0){
    return false;
  }
  //not there yet
  if (currencies.count(type) == 0){
    std::cout << "No currency of type: " << type << std::endl;
    return false;
  }
  else {
    //had enough
    if(containsCurrency(type, amount)){
      std::cout << "Removing " << type << ": " << amount << std::endl;
      currencies[type] -= amount;
      return true;
    }
    // didn't have enough
    else return false;
  }
};

bool Wallet::canFulfillOrder(const OrderBookEntry& order){
  // split currencies
  std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');
  // analyze ask
  if (order.orderType == OrderBookType::ask){
    double amount = order.amount;
    std::string currency = currs[0];
    std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
    return containsCurrency(currency, amount);
  }
  // analyze bid
  if (order.orderType == OrderBookType::bid){
    double amount = order.amount * order.price;
    std::string currency = currs[1];
    return containsCurrency(currency, amount);
  }
  return false;
};


void Wallet::processSale(OrderBookEntry& sale){
  // split currencies
  std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
  // offering to buy (ask)
  if (sale.orderType == OrderBookType::asksale){
    double outgoingAmount = sale.amount;
    std::string outgoingCurrency = currs[0];
    double incomingAmount = sale.amount * sale.price;
    std::string incomingCurrency = currs[1];

    currencies[incomingCurrency] += incomingAmount;
    currencies[outgoingCurrency] -= outgoingAmount;

    std::cout << "Sold: " << outgoingAmount << outgoingCurrency << " for " << incomingAmount << incomingCurrency << std::endl;
  }
  // offering to sell (bid)
  if (sale.orderType == OrderBookType::bidsale){
    double incomingAmount = sale.amount;
    std::string incomingCurrency = currs[0];
    double outgoingAmount = sale.amount * sale.price;
    std::string outgoingCurrency = currs[1];

    currencies[incomingCurrency] += incomingAmount;
    currencies[outgoingCurrency] -= outgoingAmount;

    std::cout << "Bought: " << incomingAmount << incomingCurrency << " for " << outgoingAmount << outgoingCurrency << std::endl;
  }
};
