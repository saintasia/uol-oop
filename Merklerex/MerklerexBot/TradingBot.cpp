#include <iostream>
#include <vector>
#include <tuple>
#include "TradingBot.h"
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

TradingBot::TradingBot(){}

void TradingBot::init(){
  // clear the old log.txt
  logger.clear();

  std::cout << "**** TRADING BOT MODE START **** " << std::endl;
  
  int input;
	currentTime = orderBook.getEarliestTime();
  maxTime = orderBook.getLatestTime();
  
  // inser currency
	wallet.insertCurrency("BTC", 100);
  wallet.insertCurrency("ETH", 2000);
  wallet.insertCurrency("USDT", 500000);
  wallet.insertCurrency("DOGE", 200000000);

  while(true){
    // populate/update the pastTimes array
    if(pastTimeStamps.size() < 10){
      pastTimeStamps.push_back(currentTime);
    } else {
      pastTimeStamps.push_back(currentTime);
      pastTimeStamps.pop_front();
    }

    // add current time to all logs
    logger.add("========= current time ========= ", LogType::assets);
    logger.add(currentTime, LogType::assets);
    // run bot
    runSimulator();
    logger.add("========= ============ ========= ", LogType::assets);
    // break out of the loop if we're on the last frame
    if (currentTime == maxTime){
      break;
    }
    // otherwise increment the time frame
    goToNextTimeFrame();
  }
  printWallet();
  std::cout << "**** TRADING BOT MODE END **** " << std::endl;
}

void TradingBot::runSimulator() {
  printWallet();
  for (std::string const p : orderBook.getKnownProducts()) {
    // initialize variables
    float mean, meanChange, futurePrice, minPrice, maxPrice;
    // populate variables from stat function
    std::tie(mean, meanChange, futurePrice, minPrice, maxPrice) = getStats(p);
    logger.add("--------");
    logger.add("Product: " + p);
    logger.add("Current min: " + std::to_string(minPrice));
    logger.add("Current max: " + std::to_string(maxPrice));
    logger.add("Up untill now mean: " + std::to_string(mean));
    logger.add("Future predicted: " + std::to_string(futurePrice));
    bool isGoingUp = meanChange > 0;
    // if the price is going up, make a bid, else make an ask
    if (isGoingUp){
      makeBids(p, minPrice);
    }else{
      makeAsks(p, maxPrice);
    };
    logger.add("--------");
	}
}

void TradingBot::printWallet() {
  logger.add("===== WALLET =====", LogType::assets);
  logger.add(wallet.toString(), LogType::assets);
  logger.add("===== ===== =====", LogType::assets);
}

std::tuple<float, float, float, float, float> TradingBot::getStats(std::string const product) {
  // get all orders from the current and past saved timeframes for more accurate stats
  std::vector<OrderBookEntry> liveEntries = orderBook.getLiveOrders(
    OrderBookType::ask,
    product,
    pastTimeStamps
  );

  // get the current timeframe orders
  std::vector<OrderBookEntry> currentEntries = orderBook.getOrders(
    OrderBookType::ask,
    product,
    currentTime
  );
  
  // initiate float vars
  float mean = 0, historicalMean = 0, sum = 0, meanChange = 0, futurePrice, maxPrice, minPrice;
  
  // get historical mean price from some past entires
  historicalMean = OrderBook::getMeanPrice(liveEntries);
  // get the current timeframe mean price
  mean = OrderBook::getMeanPrice(currentEntries);
  // get the current timeframe max price
  maxPrice = OrderBook::getHighPrice(currentEntries);
  // get the current timeframe min price
  minPrice = OrderBook::getLowPrice(currentEntries);

  // is current mean higher than historical mean
  meanChange = mean - historicalMean;

  //calculate potential future price
  futurePrice = mean + meanChange/pastTimeStamps.size();

  //return all values
  return {
    mean,
    meanChange,
    futurePrice,
    minPrice,
    maxPrice
  };
}

void TradingBot::makeAsks(std::string const product, float price){
  //generate ask
  //sell 100
  OrderBookEntry ask{
    price,
    100,
    currentTime,
    product,
    OrderBookType::ask,
  };
  ask.username = "botuser";
  if(wallet.canFulfillOrder(ask)){
    logger.add("Placing an ask: " + ask.product + " " + std::to_string(ask.amount) + " for " + std::to_string(ask.price), LogType::askbid);
    orderBook.insertOrder(ask);
  }
  else {
    logger.add("Insufficient funds to place ask " + ask.product, LogType::askbid);
  }
}

void TradingBot::makeBids(std::string const product, float price){
  //generate bid
  //buy 10
  OrderBookEntry bid{
    price,
    10,
    currentTime,
    product,
    OrderBookType::bid,
  };
  bid.username = "botuser";
  if(wallet.canFulfillOrder(bid)){
    logger.add("Placing a bid: " + bid.product + " " + std::to_string(bid.amount) + " for " + std::to_string(bid.price), LogType::askbid);
    orderBook.insertOrder(bid);
  }
  else {
    logger.add("Insufficient funds to place bid " + bid.product, LogType::askbid);
  }
}

void TradingBot::goToNextTimeFrame(){
  logger.add("*** Going to next time frame ***");
  for (std::string p : orderBook.getKnownProducts()){
    logger.add("Matching " + p);
    //get all the sales
		std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
    // get all asks and bids placed by the bot
    std::vector<OrderBookEntry> myAsks = orderBook.getOrders(OrderBookType::ask, p, currentTime, "botuser");
    std::vector<OrderBookEntry> myBids = orderBook.getOrders(OrderBookType::bid, p, currentTime, "botuser");
    logger.add("Sales " + std::to_string(sales.size()));
    // if there are no sales, then none of my bids or asks have been processed and should be withdrawn
    if(sales.size() == 0){
      for (OrderBookEntry& ask : myAsks){
        logger.add("Withdrawing ask: " + p + " " + std::to_string(ask.price) + " amount " + std::to_string(ask.amount), LogType::askbid);
        orderBook.removeOrder(ask);
      }
      for (OrderBookEntry& bid : myBids){
        logger.add("Withdrawing bid: " + p + " " + std::to_string(bid.price) + " amount " + std::to_string(bid.amount), LogType::askbid);
        orderBook.removeOrder(bid);
      }
    } else {
      for (OrderBookEntry& sale : sales){
        // search for my asks in sales
        for (OrderBookEntry& ask : myAsks){
          // if the ask hasn't been fulfilled, then withdraw
          if (ask.product != sale.product && ask.orderType != sale.orderType){
            orderBook.removeOrder(ask);
            logger.add("Withdrawing ask: " + p + " " + std::to_string(ask.price) + " amount " + std::to_string(ask.amount), LogType::askbid);
          }
        }
        // search for my bids in sales
        for (OrderBookEntry& bid : myBids){
          // if the bid hasn't been fulfilled, then withdraw
          if (bid.product != sale.product && bid.orderType != sale.orderType){
            orderBook.removeOrder(bid);
            logger.add("Withdrawing bid: " + p + " " + std::to_string(bid.price) + " amount " + std::to_string(bid.amount), LogType::askbid);
          }
        }
        // log the sale
        logger.add("Sale price: " + p + " " + std::to_string(sale.price) + " amount " + std::to_string(sale.amount), LogType::sale);
        if (sale.username == "botuser"){
          // process sale in the wallet
          wallet.processSale(sale);
        }
      }
    }
	}
  currentTime = orderBook.getNextTime(currentTime);
}
