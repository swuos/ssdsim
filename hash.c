#include"hash.h"
#include"initialize.h"

 
//initialize hash table
void hash_table_init()
{
    hash_table_size = 0;
    memset(hashTable, 0, sizeof(HashNode*) * HASH_TABLE_MAX_SIZE);
}
 
 
//string hash function
unsigned int hash_table_hash_str(const char* skey)
{
    const signed char *p = (const signed char*)skey;
    unsigned int h = *p;
    if(h)
    {
        for(p += 1; *p != '\0'; ++p)
			// simplified shift hash func
            h = (h << 5) - h + *p;
			// backup shift hash func
		    // h = (h << 4)^(h >> 28)^*p;
    }
    return h;
}
 
//insert key-value into hash table
int hash_table_insert(const char* skey, int nvalue, unsigned int pattern)
{
	unsigned int pos;
	HashNode* pHead;
	pattern_list* pNewpList;
	HashNode* pNewNode;
	// reach hash table max size
    if(hash_table_size >= HASH_TABLE_MAX_SIZE)
    {
        printf("out of hash table size!\n");
        return -1;
    }
 
	// get hash for input string
    pos = hash_table_hash_str(skey) % HASH_TABLE_MAX_SIZE;
 
	// set hashtable position
    pHead =  hashTable[pos];
    while(pHead)
    {
        if(strcmp(pHead->sKey, skey) == 0)
        {
			// found current key -> add current pattern to pList
#ifdef DEBUG
            printf("%s already exists! Add to exist cols\n", skey);
#endif
			// search pattern id -> no need for each run flush hashtable
			//pattern_list* cur_pList = pHead->pList;
			//while(cur_pList){
			//	if(cur_pList->data == pattern){
			//		return ;
			//	}
			//}

			// if not found, add current pattern to pList
			// create pattern_list
			pNewpList= (pattern_list*)malloc(sizeof(pattern_list));
			memset(pNewpList, 0, sizeof(pattern_list));
			pNewpList->data = pattern;
			// head insert 
			pNewpList->next = pHead->pList;
			pHead->pList = pNewpList;
            
			return 1;
        }
        pHead = pHead->pNext;
    }

	// not found current key, create HashNode for this key
    pNewNode = (HashNode*)malloc(sizeof(HashNode));
    memset(pNewNode, 0, sizeof(HashNode));
	// set skey
    pNewNode->sKey = (char*)malloc(sizeof(char) * (strlen(skey) + 1));
    strcpy(pNewNode->sKey, skey);
	// set nvalue
    pNewNode->nValue = nvalue;
	// create pList
	pNewpList= (pattern_list*)malloc(sizeof(pattern_list));
	memset(pNewpList, 0, sizeof(pattern_list));
	pNewpList->data = pattern;
	// set pList
	pNewNode->pList = pNewpList;

	// set next HashNode -> insert into hashtable
    pNewNode->pNext = hashTable[pos];
    hashTable[pos] = pNewNode;
 
	// plus hash_table_size
    hash_table_size++;
	return 0;
}
//remove key-value frome the hash table
void hash_table_remove(const char* skey)
{
    unsigned int pos = hash_table_hash_str(skey) % HASH_TABLE_MAX_SIZE;
	pattern_list* pListHead;
    if(hashTable[pos])
    {
        HashNode* pHead = hashTable[pos];
        HashNode* pLast = NULL;
        HashNode* pRemove = NULL;
        while(pHead)
        {
            if(strcmp(skey, pHead->sKey) == 0)
            {
                pRemove = pHead;
                break;
            }
            pLast = pHead;
            pHead = pHead->pNext;
        }
        if(pRemove)
        {
            if(pLast)
                pLast->pNext = pRemove->pNext;
            else
                // hashTable[pos] = NULL;
				hashTable[pos] = pRemove->pNext;
			
			// free sKey
            free(pRemove->sKey);
			// free pattern_list
			pListHead = pRemove->pList;
			while(pListHead){
				pattern_list* pListTemp = pListHead;
				pListHead = pListTemp->next;

				free(pListTemp);
			}

			// free other non pointer vars
            free(pRemove);
        }
    }
}
 
//lookup a key in the hash table
HashNode* hash_table_lookup(const char* skey)
{
    unsigned int pos = hash_table_hash_str(skey) % HASH_TABLE_MAX_SIZE;
	HashNode* pHead;
    if(hashTable[pos])
    {
        pHead = hashTable[pos];
        while(pHead)
        {
            if(strcmp(skey, pHead->sKey) == 0)
                return pHead;
            pHead = pHead->pNext;
        }
    }
    return NULL;
}
 
//print the content in the hash table
void hash_table_print()
{
	int i;
	HashNode* pHead;
	pattern_list* temppList;

    printf("===========content of hash table=================\n");
    for(i = 0; i < HASH_TABLE_MAX_SIZE; ++i)
        if(hashTable[i])
        {
            pHead = hashTable[i];
            printf("%d=>", i);
            while(pHead)
            {
                printf("%s:%d  ", pHead->sKey, pHead->nValue);
				temppList = pHead->pList;
				while(temppList){
					printf("%d ", temppList->data);
					temppList = temppList->next;
				}
                pHead = pHead->pNext;
            }
            printf("\n");
        }
}
 
//free the memory of the hash table
void hash_table_release()
{
    int i;
	HashNode* pHead;
	HashNode* pTemp;
	pattern_list* pListHead;
	pattern_list* pListTemp;

    for(i = 0; i < HASH_TABLE_MAX_SIZE; ++i)
    {
        if(hashTable[i])
        {
            pHead = hashTable[i];
            while(pHead)
            {
                pTemp = pHead;
                pHead = pHead->pNext;
                if(pTemp)
                {
					// free sKey
					free(pTemp->sKey);

					// free pattern_list
					pListHead = pTemp->pList;
					while(pListHead){
						pListTemp = pListHead;
						pListHead = pListTemp->next;

						free(pListTemp);
					}
                    
					// free other non pointer vars
                    free(pTemp);
                }
 
            }
        }
    }
}
