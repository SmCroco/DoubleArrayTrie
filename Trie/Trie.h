/*
 * Trie.h
 *
 *  Created on: 2016年3月22日
 *      Author: smallcroco
 */

#ifndef TRIE_TRIE_H_
#define TRIE_TRIE_H_

class Trie {
public:
	Trie();
	~Trie();

private:
	long* base; // base数组
	long* check; // check数组
};

#endif /* TRIE_TRIE_H_ */
