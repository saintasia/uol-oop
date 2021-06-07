#include <iostream>
#include <vector>
#include "MerkleMain.h"
#include "TradingBot.h"
#include "CSVReader.h"
#include "OrderBookEntry.h"

MerkleMain::MerkleMain(){}

void MerkleMain::init(){
  int input;
	bool isUserManual = !isBotPrompt();
	currentTime = orderBook.getEarliestTime();
	wallet.insertCurrency("BTC", 1000);
	wallet.insertCurrency("ETH", 20000);
  if(isUserManual){
		while(true){
    	printMenu();
    	input = getUserOption();
    	processUserOption(input);
		}
  }else{
		TradingBot bot{};
		bot.init();
	}
}

bool MerkleMain::isBotPrompt() {
	std::cout << "========== " << std::endl;
	std::cout << "Would you like to run a bot? Y/N" << std::endl;
	std::cout << "========== " << std::endl;
	std::string input;
	std::getline(std::cin, input);
	if(input == "Y" || input == "y") {
		return true;
	} else return false;
}

void MerkleMain::printMenu() {
	std::cout << "========== " << std::endl;
	// 1 print help
	std::cout << "1. Print help " << std::endl;
	// 2 print exchange stats
	std::cout << "2. Print exchange stats " << std::endl;
	// 3 make an offer
	std::cout << "3. Make an ask " << std::endl;
	// 4 make a bid
	std::cout << "4. Make a bid " << std::endl;
	// 5 print wallet
	std::cout << "5. Print wallet " << std::endl;
	// 6 continue
	std::cout << "6. Continue " << std::endl;

	std::cout << "========== " << std::endl;

	std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkleMain::printHelp() {
	std::cout << "Invalid choice, Choose 1-6" << std::endl;
}

void MerkleMain::printMarketStats() {
	for (std::string const p : orderBook.getKnownProducts()) {
		std::cout << "-----" << std::endl;
		std::cout << "Product: " << p << std::endl;
		std::vector<OrderBookEntry> entries = orderBook.getOrders(
			OrderBookType::ask,
			p,
			currentTime
		);
		std::cout << "Asks seen: " << entries.size() << std::endl;
		std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
		std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
	}
}

void MerkleMain::enterAsk() {
	// I will sell aetherium for 200 bitcoins, and I have 0.5 to sell
	std::cout << "Make an ask - enter the: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
	std::string input;
	std::getline(std::cin, input);

	std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
	if (tokens.size() != 3) {
		std::cout << "MerkleMain::enterAsk Bad input! " << input << std::endl;
	} else {
		try{
			OrderBookEntry obe = CSVReader::stringsToOBE(
				tokens[1],
				tokens[2],
				currentTime,
				tokens[0],
				OrderBookType::ask
			);
			obe.username = "simuser";
			if(wallet.canFulfillOrder(obe)){
				std::cout << "Wallet looks good! " << std::endl;
				orderBook.insertOrder(obe);
			}
			else {
				std::cout << "Insufficient funds. " << std::endl;
			}
		}
		catch(const std::exception& e){
			std::cout << "MerkleMain::enterAsk Bad input! " << input << std::endl;
		}
	}

	std::cout << "Yout typed: " << input << std::endl;
}

void MerkleMain::enterBid() {
	std::cout << "Make a bid - enter the: product, price, amount, eg ETH/BTC,200,0.5 " << std::endl;
	std::string input;
	std::getline(std::cin, input);

	std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
	if (tokens.size() != 3) {
		std::cout << "MerkleMain::enterBid Bad input! " << input << std::endl;
	} else {
		try{
			OrderBookEntry obe = CSVReader::stringsToOBE(
				tokens[1],
				tokens[2],
				currentTime,
				tokens[0],
				OrderBookType::bid
			);
			obe.username = "simuser";
			if(wallet.canFulfillOrder(obe)){
				std::cout << "Wallet looks good! " << std::endl;
				orderBook.insertOrder(obe);
			}
			else {
				std::cout << "Insufficient funds. " << std::endl;
			}
		}
		catch(const std::exception& e){
			std::cout << "MerkleMain::enterBid Bad input! " << input << std::endl;
		}
	}

	std::cout << "Yout typed: " << input << std::endl;
}

void MerkleMain::printWallet() {
	std::cout << wallet.toString() << std::endl;
}

void MerkleMain::goToNextTimeframe() {
	std::cout << "Going to next time frame" << std::endl;
	for (std::string p : orderBook.getKnownProducts()){
		std::cout << "matching " << p << std::endl;
		std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
		std::cout << "Sales: " << sales.size() << std::endl;
		for (OrderBookEntry& sale : sales){
			std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
			if (sale.username == "simuser"){
				//update the wallet
				wallet.processSale(sale);
			}
		}
	}
	currentTime = orderBook.getNextTime(currentTime);
}

int MerkleMain::getUserOption() {
	int userOption;
	std::string line;

	std::cout << "Type in 1-6 " << std::endl;
	std::getline(std::cin, line);
	try{
		userOption = std::stoi(line);
	} catch(const std::exception& e){
		//
	}
	std::cout << "You chose: " << userOption << std::endl;

	return userOption;
}

void  MerkleMain::processUserOption(int userOption) {
	// bad input
	if (userOption == 0) {
		printHelp();
	}
	// other options
	if (userOption == 1) {
		printMarketStats();
	}
	if (userOption == 2) {
		printMarketStats();
	}
	if (userOption == 3) {
		enterAsk();
	}
	if (userOption == 4) {
		enterBid();
	}
	if (userOption == 5) {
		printWallet();
	}
	if (userOption == 6) {
		goToNextTimeframe();
	}
}
