/*
 * Trie.cpp
 *
 *  Created on: 2016年3月22日
 *      Author: smallcroco
 */

#include "Trie.h"
#include <string.h>

Trie::Trie() {
	// TODO Auto-generated constructor stub

	this->trie_size = 65535;
	this->base = new long[trie_size];
	memset(base, 0, trie_size*sizeof(long));

	this->check = new long[trie_size];
	memset(check, 0, trie_size*sizeof(long));
}

Trie::~Trie() {
	// TODO Auto-generated destructor stub
}

long Trie::getSize() {
	return trie_size;
}
