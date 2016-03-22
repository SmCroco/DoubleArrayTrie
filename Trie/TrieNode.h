/*
 * TrieNode.h
 *
 *  Created on: 2016年3月22日
 *      Author: smallcroco
 */

#ifndef TRIE_TRIENODE_H_
#define TRIE_TRIENODE_H_

union character {
	struct Bit {
		unsigned char byte7:1;
		unsigned char byte6:1;
		unsigned char byte5:1;
		unsigned char byte4:1;
		unsigned char byte3:1;
		unsigned char byte2:1;
		unsigned char byte1:1;
		unsigned char byte0:1;
	}byBit;
	char byChar;
};

struct ST_trieNode {
	long key; // 当前节点对应的值
	long parent; // 父节点对应的下标
	long encode; // 汉字的编码
	char value[6]; // 汉字的值
    bool isEnd; // 关键词的结尾标记
    long count; // 以该字结尾的词的个数
    long faile; // 指向匹配失败后的失败结点
};

struct ST_Dictionary {
	long encode;
	char value[6];
	ST_Dictionary* children;
	ST_Dictionary* next;

};
#endif /* TRIE_TRIENODE_H_ */
