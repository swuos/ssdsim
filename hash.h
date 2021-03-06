#ifndef __HASH_H__
#define __HASH_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <math.h>
// #include "unistd.h"
#define N 100
typedef unsigned int datatype_el;
struct listnode
{
	datatype_el data;
	struct listnode *next;
}; // list_node, *list_pnode;
// CNAME for listnode 
typedef struct listnode pattern_list;

//typedef struct frequence_set{
//	unsigned int frequence_array[10];
//	//int array_count;
//	struct frequence_set *next;
//}frequence, *frequence_pset;
//typedef frequence_pset datatype;
//typedef struct hastbl
//{
//	datatype *h;
//	int length;
//}hash_tbl, *hash_tp;
//extern void init_hash(hash_tp *hp, int m);
//extern void create_hash(hash_tp hp, datatype_el *a);
//extern frequence_pset hash_search(hash_tp hp, int key);
//extern void hash_show(hash_tp hp, int hash_val, list_pnode new);

/*
hash table refs
[1]https://blog.csdn.net/qinpanke/article/details/9171541
[2]https://blog.csdn.net/fengjiexyb/article/details/77927725
*/

#define HASH_TABLE_MAX_SIZE 10000
typedef struct HashNode_Struct HashNode;
struct HashNode_Struct
{
	char* sKey;				//键
	int nValue;				//request ID -> remastered request id like int in 0-8191
	pattern_list* pList;	//存放resuestID中出现pattern的ID
	HashNode* pNext;		//当Hash值冲突时，将数据追加到plist中。
};
HashNode* hashTable[HASH_TABLE_MAX_SIZE];	//Hash表的数组
int hash_table_size;						//Hash表中元素的个数

extern void hash_table_init();
extern int hash_table_insert(const char* skey, int nvalue, unsigned int pattern);	// 向hash表中插入键位skey,值为nvalue的键值对。如果hash表中存在skey，则插入nvalue到nvalue链表中
extern void hash_table_remove(const char* skey);				// 删除hash表中skey键值对
extern HashNode* hash_table_lookup(const char* skey);			// 查找键值为skey的节点。当找到时，返回对应的HashNode指针
extern void hash_table_release();								// 释放hash表的内存空间

#endif
