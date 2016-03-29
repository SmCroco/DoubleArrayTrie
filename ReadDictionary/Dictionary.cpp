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
	dictionary->encode = 0;
	dictionary->value[0] = 0;
	dictionary->value[1] = 0;
	dictionary->value[2] = 0;
	dictionary->value[3] = 0;
	dictionary->value[4] = 0;
	dictionary->value[5] = 0;
	dictionary->children = NULL;
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

	if (InitDictionary(path)) {

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
bool Dictionary::InitDictionary(string path) {
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
		char word[200] = { 0 };
		memset(word, 0, 200 * sizeof(char));
		while (!file.eof()) {
			file.getline(word, 200);

			i += 1;

			// 换行字符
			for (int i = 0; (word[i] != '\r' && word[i] != 0);) {

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
				switch (n) {
				case 1: {
					q->value[0] = value.byChar;
					q->encode = value.byChar;
				}
					break;

				case 2: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;

					// utf8的字符编码
					q->encode = ((q->value[0] & 0x1f) << 6)
							+ (q->value[1] & 0x3f);
				}
					break;

				case 3: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;
					q->value[2] = word[i];
					i += 1;
					// utf8的字符编码
					q->encode = ((q->value[0] & 0x0f) << 12)
							+ ((q->value[1] & 0x3f) << 6)
							+ (q->value[2] & 0x3f);
				}
					break;

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
				}
					break;

				case 5: {
					q->value[0] = value.byChar;
					q->value[1] = word[i];
					i += 1;
					q->value[2] = word[i];
					i += 1;
					q->value[3] = word[i];
					i += 1;
					q->value[4] = word[i];
					i += 1;
					// utf8的字符编码
					q->encode = ((q->value[0] & 0x03) << 24)
							+ ((q->value[1] & 0x3f) << 18)
							+ ((q->value[2] & 0x3f) << 12)
							+ ((q->value[3] & 0x3f) << 6)
							+ (q->value[4] & 0x3f);
				}
					break;

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
				}
					break;

				default: {
					cout << "字符不正确" << endl;
				}
				}
			}

			memset(word, 0, 200 * sizeof(char));
		}

		cout << i << endl;

		file.close();

		BuildDictionary(dictionary);

		return true;
	} else {
		cout << "词库文件打开失败" << endl;
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

void Dictionary::BuildDictionary(ST_Dictionary* dic) {

	// 遍历词库，找出所有相同字符开头的词语
	ST_Dictionary* del = dic;
	ST_Dictionary* p = dic->children;

	long encode = 0;
	while (p != NULL) {
		// 取出当前字符的编码，用于查找相同字符开头的词语
		encode = p->encode;
		if (encode == 25665) {
			int y = 1;
		}

		// 生成子词库
		ST_Dictionary* sub_dic = new ST_Dictionary;
		sub_dic->encode = 0;
		sub_dic->value[0] = 0;
		sub_dic->value[1] = 0;
		sub_dic->value[2] = 0;
		sub_dic->value[3] = 0;
		sub_dic->value[4] = 0;
		sub_dic->value[5] = 0;
		sub_dic->children = NULL;
		sub_dic->next = NULL;

		ST_Dictionary* sub_p = sub_dic;

		while (p != NULL) {
			if (encode == p->encode) {

				ST_Dictionary* character = new ST_Dictionary;
				character->encode = p->encode;
				character->value[0] = p->value[0];
				character->value[1] = p->value[1];
				character->value[2] = p->value[2];
				character->value[3] = p->value[3];
				character->value[4] = p->value[4];
				character->value[5] = p->value[5];
				character->children = NULL;
				character->next = p->next;

				sub_p->children = character;
				sub_p = character;

				del->children = p->children;
				p->children = NULL;
				p->next = NULL;
				delete p;
				p = del->children;
			} else {
				del = p;
				p = p->children;
			}
		}

		BuildTrie(sub_dic);

		del = dic;
		p = dic->children;
	}

}

//void Dictionary::BuildTrie(ST_Dictionary* dic) {
//
//	ST_Dictionary* p = dic;
//	while (dic != NULL) {
//		while(p != NULL) {
//		cout<<p->value;
//		p = p->next;
//		}
//		cout<<""<<endl;
//		dic = dic->children;
//		p = dic;
//	}
//
//	// 遍历词库，找出所有相同字符开头的词语
//	ST_Dictionary* del = dic;
//	ST_Dictionary* p = dic->children;
//
//	Character_List* root = new Character_List;
//	root->encode = p->encode;
//	root->next = NULL;
//
//	Character_List* list = root;
//
//	long encode = 0;
//	while (p != NULL) {
//		// 取出当前字符的编码，用于查找相同字符开头的词语
//		encode = p->next->encode;
//
//		// 生成子词库
//		ST_Dictionary* sub_dic = new ST_Dictionary;
//		sub_dic->encode = 0;
//		sub_dic->value[0] = 0;
//		sub_dic->value[1] = 0;
//		sub_dic->value[2] = 0;
//		sub_dic->value[3] = 0;
//		sub_dic->value[4] = 0;
//		sub_dic->value[5] = 0;
//		sub_dic->children = NULL;
//		sub_dic->next = NULL;
//
//		ST_Dictionary* sub_p = sub_dic;
//
//		// 复制所有词语头以后的 内容
//		ST_Dictionary* character = new ST_Dictionary;
//		character->children = NULL;
//		character->encode = encode;
//		character->next = p->next->next;
//		character->value[0] = p->next->value[0];
//		character->value[1] = p->next->value[1];
//		character->value[2] = p->next->value[2];
//		character->value[3] = p->next->value[3];
//		character->value[4] = p->next->value[4];
//		character->value[5] = p->next->value[5];
//
//		if (encode != p->next->encode) {
//			Character_List* node = new Character_List;
//			node->encode = p->next->encode;
//			node->next = NULL;
//
//			list->next = node;
//			list = node;
//		}
//	}
//}

void Dictionary::BuildTrie(ST_Dictionary* dic) {

	ST_Dictionary* l = dic;
	ST_Dictionary* n = dic;
	while (n != NULL) {
		while (l != NULL) {
			cout << l->value;
			l = l->next;
		}
		cout << "" << endl;
		n = n->children;
		l = n;
	}

	// 删除根节点
	ST_Dictionary* p = dic;
	dic = dic->children;
	p->children = NULL;
	delete p;
	p = dic;

	// 记录词语第一个字符的编码
	Character_List* root = new Character_List;
	root->encode = p->encode;
	root->next = NULL;
	Character_List* list_p = root;

	// 产生子一级的词库
	ST_Dictionary* sub_dic = new ST_Dictionary;
	sub_dic->encode = 0;
	sub_dic->value[0] = 0;
	sub_dic->value[1] = 0;
	sub_dic->value[2] = 0;
	sub_dic->value[3] = 0;
	sub_dic->value[4] = 0;
	sub_dic->value[5] = 0;
	sub_dic->children = NULL;
	sub_dic->next = NULL;

	ST_Dictionary* sub_p = sub_dic;

	// 遍历dictionary
	while (p != NULL) {
		ST_Dictionary* q = p->next;
		if (q != NULL) {
			if (!isExist(q->encode, root->next)) {
				Character_List* new_char = new Character_List;
				new_char->encode = q->encode;
				new_char->next = NULL;
				list_p->next = new_char;
				list_p = new_char;
			}

			// 记录当前词语
			ST_Dictionary* character = new ST_Dictionary;
			character->encode = q->encode;
			character->value[0] = q->value[0];
			character->value[1] = q->value[1];
			character->value[2] = q->value[2];
			character->value[3] = q->value[3];
			character->value[4] = q->value[4];
			character->value[5] = q->value[5];
			character->next = q->next;
			character->children = NULL;
			sub_p->children = character;
			sub_p = character;
		}
		// 删除当前词语
		dic = p->children;
		p->children = NULL;
		p->next = NULL;
		delete p;
		p = dic;
	}

	l = sub_dic;
	n = sub_dic;

	int x = 0;
	while (n != NULL) {
		while (l != NULL) {
			cout << l->value;
			l = l->next;
		}
		cout << "" << endl;
		n = n->children;
		l = n;

		x++;
	}
	cout << x << endl;

	x = 0;
	list_p = root;
	while (list_p != NULL) {
		cout << list_p->encode << endl;
		list_p = list_p->next;
		x += 1;
	}
	cout <<(x-1) << endl;



	BuildDictionary(sub_dic);
}

bool Dictionary::isExist(long encode, Character_List* p) {
	while (p != NULL) {
		if (encode == p->encode) {
			return true;
		}

		p = p->next;
	}
	return false;
}

void Dictionary::BuildTrieNode(Character_List* list) {

	int key = 1;
	long size = trie->getSize();

	Character_List* p = list->next;


	while (p != NULL) {
		if (p->encode > size) {
			// 重新分配trie的base数组
		}
	}



}
