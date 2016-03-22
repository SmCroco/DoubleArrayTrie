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
	this->base = new long[65535];
	memset(base, 0, 65535*sizeof(long));

	this->check = new long[65535];
	memset(check, 0, 65535*sizeof(long));

}

Trie::~Trie() {
	// TODO Auto-generated destructor stub
}

