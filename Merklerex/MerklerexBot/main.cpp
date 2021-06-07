//
//  main.cpp
//  Merklerex
//
//  Created by Anastasia Kashkinova on 2020/10/24.
//  Copyright © 2020 Anastasia Kashkinova. All rights reserved.
// 

// std - accessing standard library
// cout - console out
// cin - console input
// << feed
// endl - end of line cross-platform
// std:: - namespace (standard library)

// void - doesn't return anything

#include <iostream>
#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "MerkleMain.h"
#include "CSVReader.h"
#include "Wallet.h"

int main() {

	// create the app
	MerkleMain app{};
	
	Wallet wallet;
	app.init();

	// // ++i is more efficient than i++ (makes copy and then increments)
	// // ++i just increments it directly

	return 0;
}
