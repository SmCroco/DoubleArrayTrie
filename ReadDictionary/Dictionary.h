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

struct Character_List {
	long encode;
	Character_List* next;
};

class Dictionary {
public:
	Dictionary();
	~Dictionary();

	bool InitDictionary(string path);

private:
	bool InitDictionary(string path);

	int getCharacterLen(character c);

	void BuildDictionary(ST_Dictionary* dic);

	void BuildTrie(ST_Dictionary* dic);

	void BuildTrieNode(Character_List* list);

	bool isExist(long encode, Character_List* p);

private:
	Trie* trie;
	ST_Dictionary* dictionary;
};

#endif /* READDICTIONARY_DICTIONARY_H_ */
