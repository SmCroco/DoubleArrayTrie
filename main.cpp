/*
 * main.cpp
 *
 *  Created on: 2016年3月22日
 *      Author: smallcroco
 */

#include <string>
#include "Dictionary.h"
#include <iostream>

using namespace std;

int main() {

	string path = "common-1.txt";

	time_t begin, end;
	begin = clock();

	Dictionary dic;
	dic.InitDictionary(path);

	end = clock();

	cout<<"runtime: "<<double(end-begin)/CLOCKS_PER_SEC<<endl;

	int a = 0;
	return 0;
}


