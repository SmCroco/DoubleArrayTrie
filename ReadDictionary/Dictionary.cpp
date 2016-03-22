/*
 * Dictionary.cpp
 *
 *  Created on: 2016年3月22日
 *      Author: smallcroco
 */

#include <fstream>
#include <iostream>
#include <string.h>
#include "Dictionary.h"

/*
 * @Function Name: Dictionary
 * @Description: 构造函数, 初始化trie树
 */
Dictionary::Dictionary() {

	this->trie = new Trie();
	this->dictionary = new ST_Dictionary;
	dictionary->children = NULL;
	dictionary->encode = 0;
	dictionary->next = NULL;
}

/*
 * @Function Name: ~Dictionary
 * @Description: 析构函数, 释放trie树
 */
Dictionary::~Dictionary() {
	if (this->trie != NULL) {
		delete this->trie;
		this->trie = NULL;
	}
}

/*
 * @Function Name: InitDictionary
 * @Parameter [in] string path: 词库文件路径
 * @Description:
 * @Return Value: 返回词库文件初始状态
 */
bool Dictionary::InitDictionary(string path) {


	if (ReadDictionary(path)) {

		return true;
	} else {
		return false;
	}
}

/*
 * @Function Name: ReadDictionary
 * @Parameter [in] string path: 词库文件路径
 * @Description: 将词库文件读取到内存
 * @Return Value: 返回读取成功与否
 */
bool Dictionary::ReadDictionary(string path) {
	ifstream file(path.c_str());
	if (file.is_open()) {
		file.seekg(0, ios::beg);

		// 用来标记每一个UTF8字符占几个字节
		int n = 0;

		// 词库的根结点
		ST_Dictionary* p = NULL;
		ST_Dictionary* q = NULL;
		p = q = dictionary;

		// 循环读取文件中的字符
		character value;

		int i = 0;

		// 读取词库文件的每一行
		char word[200] = {0};
		memset(word, 0, 200*sizeof(char));
		while (!file.eof()) {
			file.getline(word, 200);

			i += 1;

			// 换行字符
			for (int i = 0; (word[i] != '\r' && word[i] != 0); ) {

				ST_Dictionary* character = new ST_Dictionary;
				character->children = NULL;
				character->next = NULL;
				if (i == 0) {
					p->children = character;
					p = q = character;
				} else {
					q->next = character;
					q = character;
				}

				// UTF-8的第一个字节
				value.byChar = word[i];
				i += 1;

				// 获取utf8单个字符的长度
				n = getCharacterLen(value);
				switch(n) {
				case 1: {
					q->value[0] = value.byChar;
					q->encode = value.byChar;
				}break;

				case 2: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;

					// utf8的字符编码
					q->encode = ((q->value[0] & 0x1f) << 6)
							+ (q->value[1] & 0x3f);
				}break;

				case 3: {
					q->value[0] = value.byChar;
					q->value[1]= word[i];
					i += 1;
					q->value[2] = word[i];
					i += 1;
					// utf8的字符编码
					q->encode = ((q->value[0] & 0x0f) << 12)
							+ ((q->value[1] & 0x3f) << 6)
							+ (q->value[2] & 0x3f);
				}break;

				case 4: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;
					q->value[2] = word[i];
					i += 1;
					q->value[3] = word[i];
					i += 1;
					// utf8的字符编码
					q->encode = ((q->value[0] & 0x07) << 18)
							+ ((q->value[1] & 0x3f) << 12)
							+ ((q->value[2] & 0x3f) << 6)
							+ (q->value[3] & 0x3f);
				}break;

				case 5: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;
					q->value[2]= word[i];
					i += 1;
					q->value[3]= word[i];
					i += 1;
					q->value[4] = word[i];
					i += 1;
					// utf8的字符编码
					q->encode = ((q->value[0] & 0x03) << 24)
							+ ((q->value[1] & 0x3f) << 18)
							+ ((q->value[2] & 0x3f) << 12)
							+ ((q->value[3] & 0x3f) << 6)
							+ (q->value[4] & 0x3f);
				}break;

				case 6: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;
					q->value[2] = word[i];
					i += 1;
					q->value[3] = word[i];
					i += 1;
					q->value[4] = word[i];
					i += 1;
					q->value[5] = word[i];
					i += 1;
					// utf8的字符编码
					q->encode = ((q->value[0] & 0x1) << 30)
							+ ((q->value[1] & 0x3f) << 24)
							+ ((q->value[2] & 0x3f) << 18)
							+ ((q->value[3] & 0x3f) << 12)
							+ ((q->value[4] & 0x3f) << 6)
							+ (q->value[5] & 0x3f);
				}break;

				default: {
					cout << "字符不正确" << endl;
				}
				}
			}

			memset(word, 0, 200*sizeof(char));
		}

		cout<<i<<endl;

		file.close();
		return true;
	} else {
		cout<<"词库文件打开失败"<<endl;
		return false;
	}
}
/*
 * @Funcition Name: getCharacterLen
 * @Parameter [in] character c: 单个字节
 * @Description: 根据UTF-8字符的第一个字节，判断UTF-8字符占几个字节
 * @Return Value: 返回占几个字节
 */
int Dictionary::getCharacterLen(character c) {

	if (c.byBit.byte0 == 0) {
		return 1;
	}

	if (c.byBit.byte0 == 1 && c.byBit.byte1 == 1 && c.byBit.byte2 == 0) {
		return 2;
	}

	if (c.byBit.byte0 == 1 && c.byBit.byte1 == 1 && c.byBit.byte2 == 1
			&& c.byBit.byte3 == 0) {
		return 3;
	}

	if (c.byBit.byte0 == 1 && c.byBit.byte1 == 1 && c.byBit.byte2 == 1
			&& c.byBit.byte3 == 1 && c.byBit.byte4 == 0) {
		return 4;
	}

	if (c.byBit.byte0 == 1 && c.byBit.byte1 == 1 && c.byBit.byte2 == 1
			&& c.byBit.byte3 == 1 && c.byBit.byte4 == 1 && c.byBit.byte5 == 0) {
		return 5;
	}

	if (c.byBit.byte0 == 1 && c.byBit.byte1 == 1 && c.byBit.byte2 == 1
			&& c.byBit.byte3 == 1 && c.byBit.byte4 == 1 && c.byBit.byte5 == 1
			&& c.byBit.byte6 == 0) {
		return 6;
	}

	return 0;
}
