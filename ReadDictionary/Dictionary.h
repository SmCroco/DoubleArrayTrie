/*
 * Dictionary.h
 *
 *  Created on: 2016年3月22日
 *      Author: smallcroco
 */

#ifndef READDICTIONARY_DICTIONARY_H_
#define READDICTIONARY_DICTIONARY_H_

#include <string>
#include "Trie.h"
#include "TrieNode.h"

using namespace std;

class Dictionary {
public:
	Dictionary();
	~Dictionary();

	bool InitDictionary(string path);

private:
	bool ReadDictionary(string path);

	int getCharacterLen(character c);

private:
	Trie* trie;
	ST_Dictionary* dictionary;
};

#endif /* READDICTIONARY_DICTIONARY_H_ */
