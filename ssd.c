/*****************************************************************************************************************************
This project was supported by the National Basic Research 973 Program of China under Grant No.2011CB302301
Huazhong University of Science and Technology (HUST)   Wuhan National Laboratory for Optoelectronics

FileName： ssd.c
Author: Hu Yang		Version: 2.1	Date:2011/12/02
Description: 

History:
<contributor>     <time>        <version>       <desc>                   <e-mail>
Yang Hu	        2009/09/25	      1.0		    Creat SSDsim       yanghu@foxmail.com
                2010/05/01        2.x           Change 
Zhiming Zhu     2011/07/01        2.0           Change               812839842@qq.com
Shuangwu Zhang  2011/11/01        2.1           Change               820876427@qq.com
Chao Ren        2011/07/01        2.0           Change               529517386@qq.com
Hao Luo         2011/01/01        2.0           Change               luohao135680@gmail.com
Zhiming Zhu     2012/07/19        2.1.1         Correct erase_planes()   812839842@qq.com  
*****************************************************************************************************************************/

#define _CRTDBG_MAP_ALLOC
// DEBUG 
//#define DEBUG
 
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <time.h>
// memset 
#include <string.h>
// #include <memory.h>

#include "ssd.h"
#include "initialize.h"
#include "pagemap.h"
#include "flash.h"
#include "hash.h"



void block_erase_count_output(struct ssd_info* ssd)
{
	FILE * fp = fopen("out_whole_block_erase_count.csv", "wt");
	//int idx=0;
	struct block_erase_count *prev = (struct block_erase_count *)malloc(sizeof(struct block_erase_count));
	prev = ssd->block_head;
	while(prev)
	{
		//fprintf(fp, "channel[%d]chip[%d]die[%d]plane[%d]block[%d] erase count: %d\n", prev->channel, prev->chip, prev->die, prev->plane, prev->block, prev->erase_count);
		fprintf(fp, "%d\n", prev->erase_count);
		prev = prev->next;
	}
	fclose(fp);
}

//struct frequence_set *get_frequence_set(struct ssd_info* ssd)
//{
//	struct frequence_set *p = NULL;
//    struct frequence_set *prev = NULL;
//	struct frequence_set *frequence_prev;
//	int i, j;
//	int idx = 0;
//	unsigned int a[10];
//	unsigned int b[N];
//	char c;
//	struct hash_tp *hp;
//	hash_tp print;
//    int m,key;
//    char ch;
//    struct listnode *ret;
//	int count = 0;
//
//    memset(b, 0, sizeof(b));
//
//	//每次调用初始化频繁集合链表
//	ssd->frequence_head = NULL;
//	//struct frequence_set *head = NULL;
//	p = (struct frequence_set *)malloc(sizeof(struct frequence_set));
//		
//	while ((c = fgetc(ssd->array_fp)) != EOF)//读入一个字符，并判断是否为终止符
//	{
//		memset(a, 0, sizeof(a));//每一行读入前先将a[10]置0
//		ungetc(c, ssd->array_fp);//回退一个字符
//		i = 0;
//		while (1)//读入一行
//		{
//			c = fgetc(ssd->array_fp);//读入一个字符
//			if (c == 0x0a)
//				break;
//			else
//			{
//				ungetc(c, ssd->array_fp);//回退一个字符
//				fscanf(ssd->array_fp, "%d", &a[i]);//从文件中读入一个数值
//				// printf("a[%d]:%d\n", i, a[i]);
//				i++;
//			}
//		}
//		
//		frequence_prev = (struct frequence_set *)malloc(sizeof(struct frequence_set));
//		for (i = 0; i < 10; i++)
//		{
//			frequence_prev->frequence_array[i] = a[i];
//			//printf("a[i]:%d\n", a[i]);
//			//printf("p->frequence_array[i]:%d\n", frequence_prev->frequence_array[i]);
//		}
//		//strcpy(frequence_prev->frequence_array[10], a[10]);
//		//frequence_prev->frequence_array[10] = a[10];
//		//memcpy(frequence_prev->frequence_array[10], a[10], sizeof(frequence_prev->frequence_array[10]));
//		frequence_prev->next = ssd->frequence_head;
//		ssd->frequence_head = frequence_prev;
//
//	}
//
//	
//	prev = (struct frequence_set *)malloc(sizeof(struct frequence_set));
//	prev = ssd->frequence_head;
//	i = 0;
//	while(prev)
//	{
//		b[i] = prev->frequence_array[0];
//		i++;
//		prev = prev->next;
//	}
//	for(i=0; i<N; i++)
//	{
//		count++;
//		if(b[i] == 0) break;	
//	}
//	//根据记录个数得到hash表长度
//	m =(int) ceil((float)count/0.75);
//	//初始化哈希表空间
//	init_hash(&hp,m);
//	
//	//prev = (struct frequence_set *)malloc(sizeof(struct frequence_set));
//	//prev = ssd->frequence_head;
//	//创建hash表
//	create_hash(hp,b,ssd);
//
//
//    /*
//	p = (struct frequence_set *)malloc(sizeof(struct frequence_set));
//	p = ssd->frequence_head;
//	printf("currently the frequence array is:");
//	while (p)
//	{
//		for (i = 0; i < 10; i++)
//		{
//			printf("%d", p->frequence_array[i]);
//			//fprintf("");
//		}
//		//system("pause");
//		p = p->next;
//	}
//	printf("\n");
//	system("pause");
//	*/
//	return hp;
//}

/*
void insert_list(ssd)
{
	struct request *prev;
	prev=(struct request*)malloc(sizeof(struct request));
	
	prev->time = ssd->temp_req->time_t;
	//printf("%u ", request1->time);
	prev->lsn = ssd->temp_req->lsn;
	prev->size = ssd->temp_req->size;
	prev->operation = ssd->temp_req->ope;	
	prev->begin_time = ssd->temp_req->time_t;
	prev->response_time = 0;	
	prev->energy_consumption = 0;	
	prev->next_node = NULL;
	prev->distri_flag = 0;              // indicate whether this request has been distributed already
	prev->subs = NULL;
	prev->need_distr_flag = NULL;
	prev->complete_lsn_count=0;         //record the count of lsn served by buffer
	
	prev->next_node = ssd->insert_list_head;
	ssd->insert_list_head = prev;

}
*/

// create hash_table from pattern file
// return pattern number
int get_hash_table(struct ssd_info* ssd)
{
    unsigned int i;
	int j;
    char c;
	char* s[20];
	int ret = 0;
	
	// init hash table
	hash_table_init();

	i = 0; // row count
	j = 0; // item count -> as itemID
	
    // read file
	while ((c = fgetc(ssd->array_fp)) != EOF)//读入一个字符，并判断是否为终止符
	{
		ungetc(c, ssd->array_fp);//回退一个字符
		
		while (1)//读入一行
		{
			c = fgetc(ssd->array_fp);//读入一个字符
			if (c == 0x0a){
				i++;
				break;
			}
			else{
				ungetc(c, ssd->array_fp);//回退一个字符
				fscanf(ssd->array_fp, "%s", s);//从文件中以字符串形式读入一个数值
				// printf("a[%d]:%d\n", i, a[i]);
#ifdef DEBUG
				printf("read in :%s\n",s);
				hash_table_print();
#endif

				ret = hash_table_insert(s, j, i);
				if(ret == 0){
					j++;
				}
			}
			
		}

	}

	hash_table_print();
	// return count of patterns
    return i;
}

// Init a int MATRIX with $row rows and $col cols
int** init_requestmat(int row, int col){
	int ** arr = NULL;
	int i;
	arr = (int **)malloc(sizeof(int*)*row);		//arr在这里可以看出成数组，数组的每个成员都是指向int类型的指针，这样每个指针指向的代表一行，共row行   
	for(i=0; i<row; i++)					//为每行申请空间
	{
	    arr[i]=(int*)malloc(sizeof(int)*col);	//每一行有col列
		memset(arr[i],-1,sizeof(int)*col);
	}
	return arr;
}

// init pattern trace table, input is pattern count
/*
DATA like this->
	patternid1->reqid11->reqid12->reqid13
	patternid2->reqid21->reqid22->reqid23
	patternid3->reqid31->reqid32->reqid33
*/
pattern_list** init_pattern(int row){
	pattern_list** arr = NULL;
	pattern_list* temp = NULL;
	pattern_list* npList = NULL;
	int i;
	// int count = 0;
	// HashNode* tempnode = hashTable[0];
	arr = (pattern_list**)malloc(sizeof(pattern_list*)*row);
	memset(arr,0,sizeof(pattern_list*)*row);

	for(i = 0; i < HASH_TABLE_MAX_SIZE; ++i){
		if(hashTable[i]){
			temp = hashTable[i]->pList;
			while(temp){
				npList = (pattern_list*)malloc(sizeof(pattern_list));
				// set value
				npList->data = hashTable[i]->nValue;
				//if(arr[temp->data] && arr[temp->data]!= 0xcdcdcdcd){
				if(arr[temp->data]){
					npList->next = arr[temp->data];
				}
				else{
					npList->next = NULL;
					// count++;
				}
				arr[temp->data] = npList;
				// arr[temp->data]->data = hashTable[i]->nValue;
				temp = temp->next;
			}
		}
	}
	return arr;
}


/********************************************************************************************************************************
1，main函数中initiatio()函数用来初始化ssd,；2，make_aged()函数使SSD成为aged，aged的ssd相当于使用过一段时间的ssd，里面有失效页，
non_aged的ssd是新的ssd，无失效页，失效页的比例可以在初始化参数中设置；3，pre_process_page()函数提前扫一遍读请求，把读请求
的lpn<--->ppn映射关系事先建立好，写请求的lpn<--->ppn映射关系在写的时候再建立，预处理trace防止读请求是读不到数据；4，simulate()是
核心处理函数，trace文件从读进来到处理完成都由这个函数来完成；5，statistic_output()函数将ssd结构中的信息输出到输出文件，输出的是
统计数据和平均数据，输出文件较小，trace_output文件则很大很详细；6，free_all_node()函数释放整个main函数中申请的节点
*********************************************************************************************************************************/
void main()
{
	unsigned  int i,j,k;
	struct ssd_info *ssd;

	#ifdef DEBUG
	printf("enter main\n"); 
	#endif

	ssd=(struct ssd_info*)malloc(sizeof(struct ssd_info));
	alloc_assert(ssd,"ssd");
	memset(ssd,0, sizeof(struct ssd_info));

	ssd=initiation(ssd);
	make_aged(ssd);
	pre_process_page(ssd);

	for (i=0;i<ssd->parameter->channel_number;i++)
	{
		for (j=0;j<ssd->parameter->die_chip;j++)
		{
			for (k=0;k<ssd->parameter->plane_die;k++)
			{
				printf("%d,0,%d,%d:%5d\n",i,j,k,ssd->channel_head[i].chip_head[0].die_head[j].plane_head[k].free_page);
			}
		}
	}
	fprintf(ssd->outputfile,"\t\t\t\t\t\t\t\t\tOUTPUT\n");
	fprintf(ssd->outputfile,"****************** TRACE INFO ******************\n");


    

	ssd=simulate(ssd);
	block_erase_count_output(ssd);
	statistic_output(ssd);  
/*	free_all_node(ssd);*/

	printf("\n");
	printf("the simulation is completed!\n");
	// system("pause");
/* 	_CrtDumpMemoryLeaks(); */
}



/******************simulate() *********************************************************************
*simulate()是核心处理函数，主要实现的功能包括
*1,从trace文件中获取一条请求，挂到ssd->request
*2，根据ssd是否有dram分别处理读出来的请求，把这些请求处理成为读写子请求，挂到ssd->channel或者ssd上
*3，按照事件的先后来处理这些读写子请求。
*4，输出每条请求的子请求都处理完后的相关信息到outputfile文件中
**************************************************************************************************/
struct ssd_info *simulate(struct ssd_info *ssd)
{
	struct request *prev = NULL;
	struct request *elem = NULL;
	struct request *elem_temp = NULL;
	struct request *search = NULL;
	// void search;
	struct request *del = NULL;
	struct request *del_temp = NULL;
    struct request *old = NULL;//用于释放结点
	//stack
	struct request *next_head = NULL;
	struct request *top = NULL;
	//struct frequence_set *elem = NULL;
	// struct frequence_set *temp = NULL;
    // struct frequence_set *temp_prev = NULL;
	// frequence_pset ret = NULL;
	
	// hash_tp hp;


	int flag_elem;
	int flag_req = 0;
	double output_step=0;
	unsigned int a=0,b=0;
	errno_t err;
	unsigned long int count = 0;
	//int idx = 0;//debug
	//int idx2 = 0;
    //int idx3 = 0;
	
	
	// pattern matrix row and col
	int matrow = 0;
	int matcol = 0;
	// request mat
	int** requestmat;
	// whole request list
	struct request *insertListHead = NULL;
	// pattern HIT FINISH request list
	struct request *hitListHead = NULL;
	struct request *hitListTemp = NULL;
	// pattern HIT !NOT! FINISH request list
	struct request *nothitListHead = NULL;
	struct request *nothitListTemp = NULL;
	struct request *nothitListTempPre = NULL;
	// previous request link
	struct request *previousList = NULL;
	struct request* request1 = NULL;
	struct request* temp1 = NULL;
	struct request* temp2 = NULL;
	// temp vars
	char temp[20];
	HashNode* tempHashNode = NULL;
	pattern_list* temppList = NULL;
	pattern_list* temppList2 = NULL;
	pattern_list* checkpList = NULL;
	pattern_list* checkpListHead = NULL;
	// pattern to reqid list
	pattern_list** pattern_info = NULL;
	// for vars
	int i = 0;

	int flag=1;
	int	flag1=0;
	int flag2=0;
	int check_flag = 0;
	int group_flag = 0;
	
	int temp_count = 0;
	int insertcount = 0;
	int nothitcount = 0;
	int hitcount = 0;
	int selflinkcount = 0;

	int ii = 0;
	int jj = 0;


	//new
	ssd->insert_list_head = NULL;
	// ssd->insert_list_head = (struct request*)malloc(sizeof(struct request));
	ssd->temp_req = NULL;
	ssd->get_requests_count = 0;



	printf("\n");
	printf("begin simulating.......................\n");
	printf("\n");
	printf("\n");
	printf("   ^o^    OK, please wait a moment, and enjoy music and coffee   ^o^    \n");


	ssd->array_fp = fopen("output.ssv", "rt");

	if((err=fopen_s(&(ssd->tracefile),ssd->tracefilename,"r"))!=0)
	{  
		printf("the trace file can't open\n");
		return NULL;
	}

	fprintf(ssd->outputfile,"      arrive           lsn     size ope     begin time    response time    process time\n");	
	fflush(ssd->outputfile);

	while(flag!=100)    
	{
		if(ssd->get_requests_count % 1024 == 0)//ssd->get_requests_count !=0 &&
	    {
			// pattern number
			matrow = get_hash_table(ssd);
			// size of hash table
			matcol = hash_table_size;

			//printf("%d", hp->length);
			//system("pause");
	    }
		flag = get_requests(ssd);
		// printf("%d\n",flag);
		count++;
		// !!!for count for 1024 requests!!!
		ssd->get_requests_count++;
		//printf("count:%d\n", count);

        //create 1024 req list
		prev=(struct request*)malloc(sizeof(struct request));
		//printf("ssd->temp_req->time:%u\n", ssd->temp_req->time);
		//printf("prev->time:%u\n", prev->time);
		prev->time = ssd->temp_req->time;
	    //printf("%u ", request1->time);
	    prev->lsn = ssd->temp_req->lsn;
	    prev->size = ssd->temp_req->size;
	    prev->operation = ssd->temp_req->operation;	
	    prev->begin_time = ssd->temp_req->time;
	    prev->response_time = 0;	
	    prev->energy_consumption = 0;	
	    prev->next_node = NULL;
	    prev->distri_flag = 0;              // indicate whether this request has been distributed already
	    prev->subs = NULL;
	    prev->need_distr_flag = NULL;
	    prev->complete_lsn_count=0;         //record the count of lsn served by buffer
	
	    prev->next_node = ssd->insert_list_head;
	    ssd->insert_list_head = prev;
		        
        if(count == 1024)
        {
			
			// create requestmat as requestmat[matrow][matcol]
			requestmat = init_requestmat(matrow,matcol);
			pattern_info = init_pattern(matrow);

			// set avaliable position in REQUEST MATRIX to 0
			for(i = 0; i < matrow; i++){
				checkpList = pattern_info[i];
				while(checkpList){
					requestmat[i][checkpList->data] = 0;
					checkpList = checkpList->next;
				}
			}


#ifdef DEBUG
			// output pattern_info
			for(i = 0; i < matrow ; i++){
				checkpList = pattern_info[i];
				while(checkpList){
					printf("lsn:%d ",checkpList->data);
					checkpList = checkpList->next;
				}
				printf("\n");
				
			}
#endif

			flag_elem = 0;

			insertListHead = ssd->insert_list_head;

			//while(insertListHead){
			//	if(insertListHead->lsn == 3612720){
			//			printf("found \n");
			//	}
			//	insertListHead = insertListHead->next_node;
			//}
			
            while(insertListHead)
            {
#ifdef DEBUG
				printf("Going into request %d\n",insertListHead->lsn);
#endif
				temp_count++;

				// if(insertListHead->lsn == 3612720){
				// 	printf("found \n");
				// }

				// temp = NULL;
				itoa(insertListHead->lsn,temp,10);
				// reset value
				tempHashNode = NULL;
				temppList = NULL;
				// search data
				tempHashNode = hash_table_lookup(temp);
				
				// if flag1=1 found; else not found
				flag1 = 0;
				// found lsn in pattern
				if(tempHashNode){
#ifdef DEBUG
					printf("found pattern %s\n",temp);
#endif
					flag1 = 1;
					// move this request to not hit list
					if(nothitListHead){
						// insert from head
						temp1 = nothitListHead;
						// nothitListHead->next_node = nothitListHead;
						nothitListHead = insertListHead;
						
						// jump link to next node
						previousList->next_node = insertListHead->next_node;
						
						// set nohitListHead link
						nothitListHead->next_node = temp1;
						
						// add pattern info
						// nothitListHead->pattern = tempHashNode->
					}
					else{
						// insert from head and set next to NULL
						nothitListHead = insertListHead;

						// jump link to next node
						previousList->next_node = insertListHead->next_node;

						nothitListHead->next_node = NULL;
						
					}

					// PROBLEM
					temppList = tempHashNode->pList;

					// Add data to request MATRIX FIRST
					while(temppList){
						requestmat[temppList->data][tempHashNode->nValue] += 1;
#ifdef DEBUG
						printf("MAT\n");
						for (ii = 0 ; ii < matrow ; ii++){
							for (jj = 0 ; jj < matcol ; jj++){
								printf("%d ",requestmat[ii][jj]);
							}
							printf("\n");
						}
#endif
						temppList = temppList->next;
					}


					// START CHECK PATTERN
					// reset pList
					temppList = tempHashNode->pList;
					while(temppList){
						// check if current pattern is satisfied
						checkpList = pattern_info[temppList->data];
						// checkpListHead = pattern_info[temppList->data];
						
						check_flag = 0;
						while(checkpList){
							// this pattern have not safisfied col
							if(requestmat[temppList->data][checkpList->data] < 1){
								// check failed
								check_flag = 1;
							}
							checkpList = checkpList->next;
							
						}
						//reset pattern_info
						// pattern_info[temppList->data] = checkpListHead;
						
						// check satisfied, remove pattern instence from matrix and nothitlist
						if(check_flag == 0){
							
							checkpList = pattern_info[temppList->data];
							// checkpListHead = pattern_info[temppList->data];
							while(checkpList){
								// remove pattern instance from matrix
//								itoa(insertListHead->lsn,temp,10);
//								// search data
//								tempHashNode = hash_table_lookup(temp);
//								temppList2 = tempHashNode->pList;
//								while(temppList2){
//#ifdef DEBUG
//									printf("deleting data on %d %d\n",temppList2->data,checkpList->data);
//#endif
//									requestmat[temppList2->data][checkpList->data] -= 1;
//									temppList2 = temppList2->next;
//								}

								for(i=0;i<matrow;i++){
									if(requestmat[i][checkpList->data] != -1){
										requestmat[i][checkpList->data] -= 1;
									}
								}


								// remove pattern instance from not hit list
								nothitListTemp = nothitListHead;
								nothitListTempPre = NULL;
								while(nothitListTemp){
									// rewrite using memcpy
									// reset pre link
									
									flag2 = 0;
									itoa(nothitListTemp->lsn,temp,10);
									// search data
									tempHashNode = hash_table_lookup(temp);
									if(tempHashNode->nValue == checkpList->data){
										flag2 = 1;
										hitListTemp = (struct request*)malloc(sizeof(struct request));
										memcpy(hitListTemp,nothitListTemp,sizeof(struct request));
										
										if(hitListHead){
											temp2 = hitListHead;
											hitListHead = hitListTemp;
											hitListHead->next_node = temp2;
										}
										else{
											hitListHead = hitListTemp;
											hitListHead->next_node = NULL;
										}
										if(nothitListTempPre){
											nothitListTempPre->next_node = nothitListTemp->next_node;
										}
										else{
											nothitListHead = nothitListHead->next_node;
										}
										break;

										// nothitListTemp = nothitListTemp->next_node;
									}
									nothitListTempPre = nothitListTemp;
									nothitListTemp = nothitListTemp->next_node;
									// end rewrite using memcpy

									
									// remove instance from list old
									//flag2 = 0;
									//itoa(nothitListTemp->lsn,temp,10);
									//// search data
									//tempHashNode = hash_table_lookup(temp);
									//if(tempHashNode->nValue == checkpList->data){
									//	// check if ListHead is been moved to hitList
									//	if(nothitListTemp == nothitListHead){
									//		nothitListHead = nothitListHead->next_node;
									//	}

									//	// if head = tail set next to NULL
									//	if(nothitListHead == nothitListHead->next_node){
									//		selflinkcount++;
									//		printf("self link!\n");
									//		nothitListHead->next_node = NULL;
									//	}

									//	flag2 = 1;
									//	if(hitListHead){
									//		// insert from head
									//		temp2 = hitListHead;
									//		// hitListHead->next_node = hitListHead;
									//		hitListHead = nothitListTemp;
									//		
									//		// jump link to next one
									//		hitListTemp->next_node = nothitListTemp->next_node;


									//		hitListHead->next_node = temp2;
									//		
									//	}
									//	else{
									//		// insert from head
									//		hitListHead = nothitListTemp;

									//		// jump link to next one 
									//		hitListTemp->next_node = nothitListTemp->next_node;

									//		hitListHead->next_node = NULL;
									//		
									//	}
									//	nothitListTemp = hitListTemp->next_node;
									//	break;
									//}
									//// if not found, update hitListTemp as previous node
									//if(flag2==0){
									//	hitListTemp = nothitListTemp;
									//	nothitListTemp = nothitListTemp->next_node;
									//}
									//else{
									//	nothitListTemp = hitListTemp->next_node;
									//}
									// end remove instance from list old
									
								}
								
								// end remove pattern 


								checkpList = checkpList->next;
							}
							//reset pattern_info
							// pattern_info[temppList->data] = checkpListHead;

							// TODO 
							// IN THIS PLACE, hitListHead may have a pattern request, need to map to flash
							
							
						}
						// GOTO next node
						temppList = temppList->next;
					}
				}

				// next node
				// if found pattern, NOT UPDATE PreviousList
				// if not found pattern, UPDATE PreviousList
				if (flag1 == 0){
					// set previous
					previousList = insertListHead;
					insertListHead = insertListHead->next_node;
				}
				else{
					insertListHead = previousList->next_node;
				}
				// insertListHead = insertListHead->next_node;

            }//while(insertListHead)
			printf("find self link %d \n",selflinkcount);
			printf("TEMP_COUNT: %d\n",temp_count);
			temp_count = 0;
            count = 0;//每次链表里处理完将count置0
			// clear HASH table
			hash_table_release();




			// *****INSERT LIST*****
			// clear ssd list and feed it from insertListHead
			insertListHead = ssd->insert_list_head;
			insertcount = 0;
			while(insertListHead && insertListHead != 0xfeeefeee){
				
				// ssd->request_tail = insertListHead;
				// ssd->request_queue = insertListHead;

				//ssd->current_time = insertListHead->time;
				//// insertListHead->response_time=ssd->current_time+1000;

				//if(insertListHead->next_node){
				//	ssd->next_request_time = insertListHead->next_node->time;
				//}

				// create request1
				request1 = (struct request*)malloc(sizeof(struct request));
				memcpy(request1,insertListHead,sizeof(struct request));
				request1->next_node = NULL;

				// process request1
				if(ssd->request_queue == NULL)          //The queue is empty
				{
					ssd->request_queue = request1;
					ssd->request_tail = request1;
					ssd->request_queue_length++;
				}
				else
				{			
					(ssd->request_tail)->next_node = request1;	
					ssd->request_tail = request1;			
					ssd->request_queue_length++;
				}

				if (request1->operation==1)             //计算平均请求大小 1为读 0为写
				{
					ssd->ave_read_size=(ssd->ave_read_size*ssd->read_request_count+request1->size)/(ssd->read_request_count+1);
				} 
				else
				{
					ssd->ave_write_size=(ssd->ave_write_size*ssd->write_request_count+request1->size)/(ssd->write_request_count+1);
				}

				flag = 1;
				if(flag == 1){   
					//printf("once\n");
					if (ssd->parameter->dram_capacity!=0){
						buffer_management(ssd);  
						distribute(ssd); 
					} 
					else{
						no_buffer_distribute(ssd);
					}		
				}

				process(ssd);
		#ifdef DEBUG
				printf("%I64u %I64u %d %d %d\n",insertListHead->begin_time, insertListHead->response_time,insertListHead->lsn,insertListHead->size,insertListHead->operation);
		#endif
				trace_output(ssd);
				insertListHead = insertListHead->next_node;
				insertcount++;
			}
			printf("INSERT COUNT %d\n",insertcount);

			// *****NOTHIT LIST*****
			// clear ssd list and feed it from insertListHead
			insertListHead = nothitListHead;
			nothitcount = 0;
			while(insertListHead && insertListHead != 0xfeeefeee){
				
				// ssd->request_tail = insertListHead;
				// ssd->request_queue = insertListHead;

				//ssd->current_time = insertListHead->time;
				//// insertListHead->response_time=ssd->current_time+1000;

				//if(insertListHead->next_node){
				//	ssd->next_request_time = insertListHead->next_node->time;
				//}

				// create request1
				request1 = (struct request*)malloc(sizeof(struct request));
				memcpy(request1,insertListHead,sizeof(struct request));
				request1->next_node = NULL;

				// process request1
				if(ssd->request_queue == NULL)          //The queue is empty
				{
					ssd->request_queue = request1;
					ssd->request_tail = request1;
					ssd->request_queue_length++;
				}
				else
				{			
					(ssd->request_tail)->next_node = request1;	
					ssd->request_tail = request1;			
					ssd->request_queue_length++;
				}

				if (request1->operation==1)             //计算平均请求大小 1为读 0为写
				{
					ssd->ave_read_size=(ssd->ave_read_size*ssd->read_request_count+request1->size)/(ssd->read_request_count+1);
				} 
				else
				{
					ssd->ave_write_size=(ssd->ave_write_size*ssd->write_request_count+request1->size)/(ssd->write_request_count+1);
				}

				flag = 1;
				if(flag == 1){   
					//printf("once\n");
					if (ssd->parameter->dram_capacity!=0){
						buffer_management(ssd);  
						distribute(ssd); 
					} 
					else{
						no_buffer_distribute(ssd);
					}		
				}

				process(ssd);
		#ifdef DEBUG
				printf("%I64u %I64u %d %d %d\n",insertListHead->begin_time, insertListHead->response_time,insertListHead->lsn,insertListHead->size,insertListHead->operation);
		#endif
				printf("%I64u %I64u %d %d %d\n",insertListHead->begin_time, insertListHead->response_time,insertListHead->lsn,insertListHead->size,insertListHead->operation);
				trace_output(ssd);
				insertListHead = insertListHead->next_node;
				nothitcount++;
			}
			printf("NOTHIT COUNT %d\n",nothitcount);
			nothitcount = 0;


			// *****HIT LIST*****
			// clear ssd list and feed it from insertListHead
			insertListHead = hitListHead;
			hitcount = 0;
			while(insertListHead && insertListHead != 0xfeeefeee){
				
				// ssd->request_tail = insertListHead;
				// ssd->request_queue = insertListHead;

				//ssd->current_time = insertListHead->time;
				//// insertListHead->response_time=ssd->current_time+1000;

				//if(insertListHead->next_node){
				//	ssd->next_request_time = insertListHead->next_node->time;
				//}

				// create request1
				request1 = (struct request*)malloc(sizeof(struct request));
				memcpy(request1,insertListHead,sizeof(struct request));
				request1->next_node = NULL;

				// process request1
				if(ssd->request_queue == NULL)          //The queue is empty
				{
					ssd->request_queue = request1;
					ssd->request_tail = request1;
					ssd->request_queue_length++;
				}
				else
				{			
					(ssd->request_tail)->next_node = request1;	
					ssd->request_tail = request1;			
					ssd->request_queue_length++;
				}

				if (request1->operation==1)             //计算平均请求大小 1为读 0为写
				{
					ssd->ave_read_size=(ssd->ave_read_size*ssd->read_request_count+request1->size)/(ssd->read_request_count+1);
				} 
				else
				{
					ssd->ave_write_size=(ssd->ave_write_size*ssd->write_request_count+request1->size)/(ssd->write_request_count+1);
				}

				flag = 1;
				if(flag == 1){   
					//printf("once\n");
					if (ssd->parameter->dram_capacity!=0){
						buffer_management(ssd);  
						distribute(ssd); 
					} 
					else{
						no_buffer_distribute(ssd);
					}		
				}

				process(ssd);
		#ifdef DEBUG
				printf("%I64u %I64u %d %d %d\n",insertListHead->begin_time, insertListHead->response_time,insertListHead->lsn,insertListHead->size,insertListHead->operation);
		#endif
				printf("%I64u %I64u %d %d %d\n",insertListHead->begin_time, insertListHead->response_time,insertListHead->lsn,insertListHead->size,insertListHead->operation);
				trace_output(ssd);
				insertListHead = insertListHead->next_node;
				hitcount++;
			}
			printf("HIT COUNT %d\n",hitcount);
			hitcount = 0;



		// TODO :
		//   free insertListHead
		//   free nothitListHead
		//   free hitListHead


		// free ssd->insert_list_head
			ssd->insert_list_head = NULL;

        }//if(count == 1024)

		if(flag == 0){
        	flag = 100;
		}
	}//while(flag != 100)

	
	fclose(ssd->tracefile);
	fclose(ssd->array_fp);
	//ssd->request_queue = insertListHead;
	//ssd->request_queue_length = 100;
	//ssd->request_tail = ssd->insert_list_head;
	//while(ssd->insert_list_head){
	//	printf("%d\n",ssd->insert_list_head->lsn);
	//	ssd->insert_list_head = ssd->insert_list_head->next_node;
	//}



	return ssd;
}




/********    get_request    ******************************************************
*	1.get requests that arrived already
*	2.add those request node to ssd->reuqest_queue
*	return	0: reach the end of the trace
*			-1: no request has been added
*			1: add one request to list
*SSD模拟器有三种驱动方式:时钟驱动(精确，太慢) 事件驱动(本程序采用) trace驱动()，
*两种方式推进事件：channel/chip状态改变、trace文件请求达到。
*channel/chip状态改变和trace文件请求到达是散布在时间轴上的点，每次从当前状态到达
*下一个状态都要到达最近的一个状态，每到达一个点执行一次process
********************************************************************************/
int get_requests(struct ssd_info *ssd)  
{  
	char buffer[200];
	unsigned int lsn=0;
	int device,  size, ope,large_lsn, i=0;
	struct request *request1 = NULL;
	// struct frequence_set *frequence_prev;//new
	//struct frequence_set *frequence_follow;//new
    //struct request *p;//new
	//struct request *pt;//new
	//struct request *prev;//new
	//struct request *follow;//new
	//struct frequence_set *lsn_record;//new
	//int lsn_record_count = 0;
	//int idx, idx2;//debug
	int flag = 1;
	//int flag_
	long filepoint; 
	__int64 time_t = 0;
	__int64 nearest_event_time;
	//unsigned int lsn_record;

	//p = (struct request *)malloc(sizeof(struct request));
	//pt = (struct request *)malloc(sizeof(struct request));

	#ifdef DEBUG
	printf("enter get_requests,  current time:%I64u, ",ssd->current_time);
	#endif
    /*
    if(ssd->doing_front->next_node == NULL && ssd->standby_front->next_node == NULL)
	{
		request1 = NULL;
		return 100; 
	}
	*/
	
	if(feof(ssd->tracefile)){
		return 100;
	}
	

	
	filepoint = ftell(ssd->tracefile);	
	fgets(buffer, 200, ssd->tracefile); 
	sscanf(buffer,"%I64u %d %d %d %d",&time_t,&device,&lsn,&size,&ope);
	
	// ADD DEBUG check 
#ifdef DEBUG
	printf("%d %d %d %d \n",device,lsn,size,ope);
#endif
	//if(ope == 0) ssd->get_requests_count++;

	//printf("%I64u", time_t);
	//printf("%d\n", idx);
	//判断写操作的lsn是否在集合中
	//if(ope==0)
	//{
	/*
		frequence_prev = (struct frequence_set *)malloc(sizeof(struct frequence_set));
		frequence_prev = ssd->frequence_head;
		while(frequence_prev)
		{
			
			for(i=0; i<10 ; i++)
			{
				if(ope == 0 && lsn == frequence_prev->frequence_array[i])
				{
					//struct request *prev;
					prev = (struct request *)malloc(sizeof(struct request));
					prev->time = time_t;
					prev->lsn = lsn;
					prev->size = size;
					prev->operation = ope;
					prev->next_node = NULL;
					//入队
					ssd->standby_rear->next_node = prev;
					ssd->standby_rear = prev;
					//printf("%d\n", prev->lsn);
					//system("pause");
					flag = 0;//该请求已经放入等待队列
					break;
				}//if
			}//for
			frequence_prev = frequence_prev->next;
        }//while

		//判断等待队列是否已存在集合中的组合
        frequence_prev = (struct frequence_set *)malloc(sizeof(struct frequence_set));
		frequence_prev = ssd->frequence_head;
		lsn_record = (struct frequence_set *)malloc(sizeof(struct frequence_set));
		prev = (struct request *)malloc(sizeof(struct request));
		follow = (struct request *)malloc(sizeof(struct request));
		prev = ssd->standby_front;
		follow = ssd->standby_front;

		//new
		while(prev)
		{
			while(frequence_prev)
			{
				for(i=0; i<10; i++)
			    {
					if(prev->lsn == frequence_prev->frequence_array[i])
					{//record 当前匹配的 array and lsn
						
						for(i=0; i<10; i++)
						{
							current_array[i] = frequence_prev->frequence_array[i]；
						}
						lsn_record->frequence_array[lsn_record_count] = prev->lsn;
						lsn_record_count++;

						break;
					}
			    }


			}
		}//while(prev)

		while(prev)
		{
			while(follow)
			{
				while(frequence_prev)
		        {
					if((prev->lsn == frequence_prev->frequence_array[0] && follow->lsn == frequence_prev->frequence_array[1]) || (prev->lsn == frequence_prev->frequence_array[1] && follow->lsn == frequence_prev->frequence_array[0]))
				        {//一个一个出队再入队
							p = prev;
							prev = prev->next_node;
							//入队
							pt = p;
							free(p);
							p = NULL;
							ssd->standby_rear->next_node = pt;
					        ssd->standby_rear = pt;
							break;
				        }//if
			        
			        frequence_prev = frequence_prev->next;
                }//while(frequence_prev)
			follow = follow->next_node;
		    }//while(follow)
		prev = prev->next_node;
		}//while(prev)
		
	
		//system("pause");
		//不在频繁集合里
		if(flag = 1)
		{
			follow = (struct request *)malloc(sizeof(struct request));
		    follow->time = time_t;
		    //printf("%I64u", time_t);
		    follow->lsn = lsn;
		    follow->size = size;
		    follow->operation = ope;
		    follow->next_node = NULL;
		    ssd->doing_rear->next_node = follow;
		    ssd->doing_rear = follow;
		}
		*/
        
		//print going queue输出队列应该从头指针的next开始
		/*
	    prev = ssd->doing_front->next_node;
		while(prev)
		{
			printf("%d,", prev->lsn);
			prev = prev->next_node;
		}
		system("pause");
		*/
	    //idx2++;
		//printf("%d\n", idx2);
		
	//}if
	//print 滞后queue
	/*
	p = ssd->standby_front->next_node;
    printf("the laid queue is:");
    while(p)
    {
         printf("%d\n", p->lsn);
         p = p->next_node;
	}
	*/
	//print 进行queue
	/*
	p = ssd->doing_front->next_node;
    printf("the on doing queue is:");
    while(p)
    {
         printf("%d\n", p->lsn);
         p = p->next_node;
	}
	*/
	
	//time_t = 0;
	//如果进行队列不为空，将进行中的最上面出队，进行下面的操作
    /*
	if (ssd->doing_front->next_node != NULL)
	{
		//出队
		p = ssd->doing_front;
        ssd->doing_front = ssd->doing_front->next_node;
        time_t = p->time;
		lsn = p->lsn ;
		size = p->size ;
		ope = p->operation;
		//printf("%I64u", p->time);
        free(p);
		p = NULL;
		//printf("%I64u", time_t);
		//system("pause");
		//printf("\nThe queue is not empty!\n");
		//system("pause");
	}
	else if(ssd->doing_front->next_node == NULL && ssd->standby_front->next_node != NULL)
	{
		return 5;
		printf("\nThe queue is empty!\n");
		while(1){}
		//system("pause");
	}
	else if(ssd->doing_front->next_node == NULL && ssd->standby_front->next_node == NULL)
	{
		request1 = NULL;
		return 100; 
	}
    */    

	if ((device<0)&&(lsn<0)&&(size<0)&&(ope<0))
	{
		return 100;
	}
	if (lsn<ssd->min_lsn) 
		ssd->min_lsn=lsn;
	if (lsn>ssd->max_lsn)
		ssd->max_lsn=lsn;
	/******************************************************************************************************
	*上层文件系统发送给SSD的任何读写命令包括两个部分（LSN，size） LSN是逻辑扇区号，对于文件系统而言，它所看到的存
	*储空间是一个线性的连续空间。例如，读请求（260，6）表示的是需要读取从扇区号为260的逻辑扇区开始，总共6个扇区。
	*large_lsn: channel下面有多少个subpage，即多少个sector。overprovide系数：SSD中并不是所有的空间都可以给用户使用，
	*比如32G的SSD可能有10%的空间保留下来留作他用，所以乘以1-provide
	***********************************************************************************************************/
	large_lsn=(int)((ssd->parameter->subpage_page*ssd->parameter->page_block*ssd->parameter->block_plane*ssd->parameter->plane_die*ssd->parameter->die_chip*ssd->parameter->chip_num)*(1-ssd->parameter->overprovide));
	// printf("lsn %d\n",lsn);
	// printf("large_lsn %d\n",large_lsn);
	// XXF-> file save non paged LSN info?
	lsn = lsn%large_lsn;
	// printf("new lsn %d\n",lsn);

	nearest_event_time=find_nearest_event(ssd);
	if (nearest_event_time==0x7fffffffffffffff)
	{
		ssd->current_time=time_t;           
		                                                  
		//if (ssd->request_queue_length>ssd->parameter->queue_length)    //如果请求队列的长度超过了配置文件中所设置的长度                     
		//{
			//printf("error in get request , the queue length is too long\n");
		//}
	}
	else
	{   
		if(nearest_event_time<time_t)
		{
			/*******************************************************************************
			*回滚，即如果没有把time_t赋给ssd->current_time，则trace文件已读的一条记录回滚
			*filepoint记录了执行fgets之前的文件指针位置，回滚到文件头+filepoint处
			*int fseek(FILE *stream, long offset, int fromwhere);函数设置文件指针stream的位置。
			*如果执行成功，stream将指向以fromwhere（偏移起始位置：文件头0，当前位置1，文件尾2）为基准，
			*偏移offset（指针偏移量）个字节的位置。如果执行失败(比如offset超过文件自身大小)，则不改变stream指向的位置。
			*文本文件只能采用文件头0的定位方式，本程序中打开文件方式是"r":以只读方式打开文本文件	
			**********************************************************************************/
			fseek(ssd->tracefile,filepoint,0); 
			if(ssd->current_time<=nearest_event_time)
				ssd->current_time=nearest_event_time;
			return -1;
		}
		else
		{
			if (ssd->request_queue_length>=ssd->parameter->queue_length)
			{
				fseek(ssd->tracefile,filepoint,0);
				ssd->current_time=nearest_event_time;
				return -1;
			} 
			else
			{
				ssd->current_time=time_t;
			}
		}
	}

    
    //printf("%I64u", time_t);
	//time_t = (unsigned long int)time_t;
	if(time_t < 0)
	{
		printf("t<0 error!\n");
		while(1){}
	}
	//应该改成going_queue为空
	/*
	if(ssd->doing_front->next_node == NULL){
		request1 = NULL;
		return 100; 
	}
	*/
	
	if(feof(ssd->tracefile)){
		request1 = NULL;
		return 100; 
	}
	

	request1 = (struct request*)malloc(sizeof(struct request));
	alloc_assert(request1,"request");
	memset(request1,0, sizeof(struct request));

	request1->time = time_t;
	//printf("%u\n ", time_t);
	request1->lsn = lsn;
	request1->size = size;
	request1->operation = ope;	
	request1->begin_time = time_t;
	request1->response_time = 0;	
	request1->energy_consumption = 0;	
	request1->next_node = NULL;
	request1->distri_flag = 0;              // indicate whether this request has been distributed already
	request1->subs = NULL;
	request1->need_distr_flag = NULL;
	request1->complete_lsn_count=0;         //record the count of lsn served by buffer
	filepoint = ftell(ssd->tracefile);		// set the file point

	ssd->temp_req = request1;
	//printf("%u\n", ssd->temp_req->response_time);
    /*
	if(ssd->request_queue == NULL)          //The queue is empty
	{
		ssd->request_queue = request1;
		ssd->request_tail = request1;
		ssd->request_queue_length++;
	}
	else
	{			
		(ssd->request_tail)->next_node = request1;	
		ssd->request_tail = request1;			
		ssd->request_queue_length++;
	}
	*/

	//if (request1->operation==1)             //计算平均请求大小 1为读 0为写
	//{
	//	ssd->ave_read_size=(ssd->ave_read_size*ssd->read_request_count+request1->size)/(ssd->read_request_count+1);
	//} 
	//else
	//{
	//	ssd->ave_write_size=(ssd->ave_write_size*ssd->write_request_count+request1->size)/(ssd->write_request_count+1);
	//}

	
	filepoint = ftell(ssd->tracefile);	
	fgets(buffer, 200, ssd->tracefile);    //寻找下一条请求的到达时间
	sscanf(buffer,"%I64u %d %d %d %d",&time_t,&device,&lsn,&size,&ope);
	ssd->next_request_time=time_t;
	fseek(ssd->tracefile,filepoint,0);

	return 1;
}

/**********************************************************************************************************************************************
*首先buffer是个写buffer，就是为写请求服务的，因为读flash的时间tR为20us，写flash的时间tprog为200us，所以为写服务更能节省时间
*  读操作：如果命中了buffer，从buffer读，不占用channel的I/O总线，没有命中buffer，从flash读，占用channel的I/O总线，但是不进buffer了
*  写操作：首先request分成sub_request子请求，如果是动态分配，sub_request挂到ssd->sub_request上，因为不知道要先挂到哪个channel的sub_request上
*          如果是静态分配则sub_request挂到channel的sub_request链上,同时不管动态分配还是静态分配sub_request都要挂到request的sub_request链上
*		   因为每处理完一个request，都要在traceoutput文件中输出关于这个request的信息。处理完一个sub_request,就将其从channel的sub_request链
*		   或ssd的sub_request链上摘除，但是在traceoutput文件输出一条后再清空request的sub_request链。
*		   sub_request命中buffer则在buffer里面写就行了，并且将该sub_page提到buffer链头(LRU)，若没有命中且buffer满，则先将buffer链尾的sub_request
*		   写入flash(这会产生一个sub_request写请求，挂到这个请求request的sub_request链上，同时视动态分配还是静态分配挂到channel或ssd的
*		   sub_request链上),在将要写的sub_page写入buffer链头
***********************************************************************************************************************************************/
struct ssd_info *buffer_management(struct ssd_info *ssd)
{   
	unsigned int j,lsn,lpn,last_lpn,first_lpn,index,complete_flag=0, state,full_page;
	unsigned int flag=0,need_distb_flag,lsn_flag,flag1=1,active_region_flag=0;           
	struct request *new_request;
	struct buffer_group *buffer_node,key;
	unsigned int mask=0,offset1=0,offset2=0;

	#ifdef DEBUG
	printf("enter buffer_management,  current time:%I64u\n",ssd->current_time);
	#endif
	ssd->dram->current_time=ssd->current_time;
	full_page=~(0xffffffff<<ssd->parameter->subpage_page);
	
	new_request=ssd->request_tail;
	lsn=new_request->lsn;
	lpn=new_request->lsn/ssd->parameter->subpage_page;
	last_lpn=(new_request->lsn+new_request->size-1)/ssd->parameter->subpage_page;
	first_lpn=new_request->lsn/ssd->parameter->subpage_page;

	new_request->need_distr_flag=(unsigned int*)malloc(sizeof(unsigned int)*((last_lpn-first_lpn+1)*ssd->parameter->subpage_page/32+1));
	alloc_assert(new_request->need_distr_flag,"new_request->need_distr_flag");
	memset(new_request->need_distr_flag, 0, sizeof(unsigned int)*((last_lpn-first_lpn+1)*ssd->parameter->subpage_page/32+1));
	
	if(new_request->operation==READ) 
	{		
		while(lpn<=last_lpn)      		
		{
			/************************************************************************************************
			 *need_distb_flag表示是否需要执行distribution函数，1表示需要执行，buffer中没有，0表示不需要执行
             *即1表示需要分发，0表示不需要分发，对应点初始全部赋为1
			*************************************************************************************************/
			need_distb_flag=full_page;   
			key.group=lpn;
			buffer_node= (struct buffer_group*)avlTreeFind(ssd->dram->buffer, (TREE_NODE *)&key);		// buffer node 

			while((buffer_node!=NULL)&&(lsn<(lpn+1)*ssd->parameter->subpage_page)&&(lsn<=(new_request->lsn+new_request->size-1)))             			
			{             	
				lsn_flag=full_page;
				mask=1 << (lsn%ssd->parameter->subpage_page);
				if(mask>31)
				{
					printf("the subpage number is larger than 32!add some cases");
					getchar(); 		   
				}
				else if((buffer_node->stored & mask)==mask)
				{
					flag=1;
					lsn_flag=lsn_flag&(~mask);
				}

				if(flag==1)				
				{	//如果该buffer节点不在buffer的队首，需要将这个节点提到队首，实现了LRU算法，这个是一个双向队列。		       		
					if(ssd->dram->buffer->buffer_head!=buffer_node)     
					{		
						if(ssd->dram->buffer->buffer_tail==buffer_node)								
						{			
							buffer_node->LRU_link_pre->LRU_link_next=NULL;					
							ssd->dram->buffer->buffer_tail=buffer_node->LRU_link_pre;							
						}				
						else								
						{				
							buffer_node->LRU_link_pre->LRU_link_next=buffer_node->LRU_link_next;				
							buffer_node->LRU_link_next->LRU_link_pre=buffer_node->LRU_link_pre;								
						}								
						buffer_node->LRU_link_next=ssd->dram->buffer->buffer_head;
						ssd->dram->buffer->buffer_head->LRU_link_pre=buffer_node;
						buffer_node->LRU_link_pre=NULL;			
						ssd->dram->buffer->buffer_head=buffer_node;													
					}						
					ssd->dram->buffer->read_hit++;					
					new_request->complete_lsn_count++;											
				}		
				else if(flag==0)
					{
						ssd->dram->buffer->read_miss_hit++;
					}

				need_distb_flag=need_distb_flag&lsn_flag;
				
				flag=0;		
				lsn++;						
			}	
				
			index=(lpn-first_lpn)/(32/ssd->parameter->subpage_page); 			
			new_request->need_distr_flag[index]=new_request->need_distr_flag[index]|(need_distb_flag<<(((lpn-first_lpn)%(32/ssd->parameter->subpage_page))*ssd->parameter->subpage_page));	
			lpn++;
			
		}
	}  
	else if(new_request->operation==WRITE)
	{
		while(lpn<=last_lpn)           	
		{	
			need_distb_flag=full_page;
			mask=~(0xffffffff<<(ssd->parameter->subpage_page));
			state=mask;

			if(lpn==first_lpn)
			{
				offset1=ssd->parameter->subpage_page-((lpn+1)*ssd->parameter->subpage_page-new_request->lsn);
				state=state&(0xffffffff<<offset1);
			}
			if(lpn==last_lpn)
			{
				offset2=ssd->parameter->subpage_page-((lpn+1)*ssd->parameter->subpage_page-(new_request->lsn+new_request->size));
				state=state&(~(0xffffffff<<offset2));
			}
			//printf("lsn:%d\n", lsn);
			ssd=insert2buffer(ssd, lpn, state,NULL,new_request);
			lpn++;
		}
	}
	complete_flag = 1;
	for(j=0;j<=(last_lpn-first_lpn+1)*ssd->parameter->subpage_page/32;j++)
	{
		if(new_request->need_distr_flag[j] != 0)
		{
			complete_flag = 0;
		}
	}

	/*************************************************************
	*如果请求已经被全部由buffer服务，该请求可以被直接响应，输出结果
	*这里假设dram的服务时间为1000ns
	**************************************************************/
	if((complete_flag == 1)&&(new_request->subs==NULL))               
	{
		new_request->begin_time=ssd->current_time;
		new_request->response_time=ssd->current_time+1000;            
	}

    //printf("channel0的chip0的die0的plane0的block1731的擦出次数是: %d\n", ssd->channel_head[0].chip_head[0].die_head[0].plane_head[0].blk_head[1731].erase_count);
    //system("pause");

	return ssd;
}

/*****************************
*lpn向ppn的转换
******************************/
unsigned int lpn2ppn(struct ssd_info *ssd,unsigned int lsn)
{
	int lpn, ppn;	
	struct entry *p_map = ssd->dram->map->map_entry;
#ifdef DEBUG
	printf("enter lpn2ppn,  current time:%I64u\n",ssd->current_time);
#endif
	lpn = lsn/ssd->parameter->subpage_page;			//lpn
	ppn = (p_map[lpn]).pn;
	return ppn;
}

/**********************************************************************************
*读请求分配子请求函数，这里只处理读请求，写请求已经在buffer_management()函数中处理了
*根据请求队列和buffer命中的检查，将每个请求分解成子请求，将子请求队列挂在channel上，
*不同的channel有自己的子请求队列
**********************************************************************************/

struct ssd_info *distribute(struct ssd_info *ssd) 
{
	unsigned int start, end, first_lsn,last_lsn,lpn,flag=0,flag_attached=0,full_page;
	unsigned int j, k, sub_size;
	int i=0;
	struct request *req;
	struct sub_request *sub;
	int* complt;

	#ifdef DEBUG
	printf("enter distribute,  current time:%I64u\n",ssd->current_time);
	#endif
	full_page=~(0xffffffff<<ssd->parameter->subpage_page);

	req = ssd->request_tail;
	if(req->response_time != 0){
		return ssd;
	}
	if (req->operation==WRITE)
	{
		return ssd;
	}

	if(req != NULL)
	{
		if(req->distri_flag == 0)
		{
			//如果还有一些读请求需要处理
			if(req->complete_lsn_count != ssd->request_tail->size)
			{		
				first_lsn = req->lsn;				
				last_lsn = first_lsn + req->size;
				complt = req->need_distr_flag;
				start = first_lsn - first_lsn % ssd->parameter->subpage_page;
				end = (last_lsn/ssd->parameter->subpage_page + 1) * ssd->parameter->subpage_page;
				i = (end - start)/32;	

				while(i >= 0)
				{	
					/*************************************************************************************
					*一个32位的整型数据的每一位代表一个子页，32/ssd->parameter->subpage_page就表示有多少页，
					*这里的每一页的状态都存放在了 req->need_distr_flag中，也就是complt中，通过比较complt的
					*每一项与full_page，就可以知道，这一页是否处理完成。如果没处理完成则通过creat_sub_request
					函数创建子请求。
					*************************************************************************************/
					for(j=0; j<32/ssd->parameter->subpage_page; j++)
					{	
						k = (complt[((end-start)/32-i)] >>(ssd->parameter->subpage_page*j)) & full_page;	
						if (k !=0)
						{
							lpn = start/ssd->parameter->subpage_page+ ((end-start)/32-i)*32/ssd->parameter->subpage_page + j;
							sub_size=transfer_size(ssd,k,lpn,req);    
							if (sub_size==0) 
							{
								continue;
							}
							else
							{
								sub=creat_sub_request(ssd,lpn,sub_size,0,req,req->operation);
							}	
						}
					}
					i = i-1;
				}

			}
			else
			{
				req->begin_time=ssd->current_time;
				req->response_time=ssd->current_time+1000;   
			}

		}
	}
	return ssd;
}


/**********************************************************************
*trace_output()函数是在每一条请求的所有子请求经过process()函数处理完后，
*打印输出相关的运行结果到outputfile文件中，这里的结果主要是运行的时间
**********************************************************************/
void trace_output(struct ssd_info* ssd){
	int flag = 1;	
	__int64 start_time, end_time;
	struct request *req, *pre_node;
	struct sub_request *sub, *tmp;

#ifdef DEBUG
	printf("enter trace_output,  current time:%I64u\n",ssd->current_time);
#endif

	pre_node=NULL;
	req = ssd->request_queue;
	start_time = 0;
	end_time = 0;

	if(req == NULL)
		return;

	while(req != NULL)	
	{
		sub = req->subs;
		flag = 1;
		start_time = 0;
		end_time = 0;
		//printf("%u\n", req->response_time);
		if(req->response_time != 0)
		{
			fprintf(ssd->outputfile,"%16I64u %10u %6u %2u %16I64u %16I64u %10I64u\n",req->time,req->lsn, req->size, req->operation, req->begin_time, req->response_time, req->response_time-req->time);
			fflush(ssd->outputfile);

			if(req->response_time-req->begin_time==0)
			{
				//printf("response_time:%d, begin_time:%d\n", req->response_time, req->begin_time);
				printf("the response time is 0?? \n");
				getchar();
			}

			if (req->operation==READ)
			{
				ssd->read_request_count++;
				ssd->read_avg=ssd->read_avg+(req->response_time-req->time);
			} 
			else
			{
				ssd->write_request_count++;
				ssd->write_avg=ssd->write_avg+(req->response_time-req->time);
			}

			if(pre_node == NULL)
			{
				if(req->next_node == NULL)
				{
					free(req->need_distr_flag);
					req->need_distr_flag=NULL;
					free(req);
					req = NULL;
					ssd->request_queue = NULL;
					ssd->request_tail = NULL;
					ssd->request_queue_length--;
				}
				else
				{
					ssd->request_queue = req->next_node;
					pre_node = req;
					req = req->next_node;
					free(pre_node->need_distr_flag);
					pre_node->need_distr_flag=NULL;
					free((void *)pre_node);
					pre_node = NULL;
					ssd->request_queue_length--;
				}
			}
			else
			{
				if(req->next_node == NULL)
				{
					pre_node->next_node = NULL;
					free(req->need_distr_flag);
					req->need_distr_flag=NULL;
					free(req);
					req = NULL;
					ssd->request_tail = pre_node;
					ssd->request_queue_length--;
				}
				else
				{
					pre_node->next_node = req->next_node;
					free(req->need_distr_flag);
					req->need_distr_flag=NULL;
					free((void *)req);
					req = pre_node->next_node;
					ssd->request_queue_length--;
				}
			}
		}
		else
		{
			flag=1;
			while(sub != NULL)
			{
				if(start_time == 0)
					start_time = sub->begin_time;
				if(start_time > sub->begin_time)
					start_time = sub->begin_time;
				if(end_time < sub->complete_time)
					end_time = sub->complete_time;
				
				if((sub->current_state == SR_COMPLETE)||((sub->next_state==SR_COMPLETE)&&(sub->next_state_predict_time<=ssd->current_time)))	// if any sub-request is not completed, the request is not completed
				{
					sub = sub->next_subs;
				}
				else
				{
					flag=0;
					break;
				}
				
			}
              //printf("sub->complete_time:%d\n", sub->complete_time);
				//printf("end_time:%d\n", end_time);
			if (flag == 1)
			{		
				fprintf(ssd->outputfile,"%16I64u %10u %6u %2u %16I64u %16I64u %10I64u\n",req->time,req->lsn, req->size, req->operation, start_time, end_time, end_time-req->time);
				fflush(ssd->outputfile);
				
				if(end_time-start_time==0)
				{
					printf("end_time:%d, start_time:%d\n", end_time, start_time);
					printf("the response time is 0?? \n");
					// getchar();
				}
				
				if (req->operation==READ)
				{
					ssd->read_request_count++;
					ssd->read_avg=ssd->read_avg+(end_time-req->time);
				} 
				else
				{
					ssd->write_request_count++;
					ssd->write_avg=ssd->write_avg+(end_time-req->time);
				}

				while(req->subs!=NULL)
				{
					tmp = req->subs;
					req->subs = tmp->next_subs;
					if (tmp->update!=NULL)
					{
						free(tmp->update->location);
						tmp->update->location=NULL;
						free(tmp->update);
						tmp->update=NULL;
					}
					free(tmp->location);
					tmp->location=NULL;
					free(tmp);
					tmp=NULL;
					
				}
				
				if(pre_node == NULL)
				{
					if(req->next_node == NULL)
					{
						free(req->need_distr_flag);
						req->need_distr_flag=NULL;
						free(req);
						req = NULL;
						ssd->request_queue = NULL;
						ssd->request_tail = NULL;
						ssd->request_queue_length--;
					}
					else
					{
						ssd->request_queue = req->next_node;
						pre_node = req;
						req = req->next_node;
						free(pre_node->need_distr_flag);
						pre_node->need_distr_flag=NULL;
						free(pre_node);
						pre_node = NULL;
						ssd->request_queue_length--;
					}
				}
				else
				{
					if(req->next_node == NULL)
					{
						pre_node->next_node = NULL;
						free(req->need_distr_flag);
						req->need_distr_flag=NULL;
						free(req);
						req = NULL;
						ssd->request_tail = pre_node;	
						ssd->request_queue_length--;
					}
					else
					{
						pre_node->next_node = req->next_node;
						free(req->need_distr_flag);
						req->need_distr_flag=NULL;
						free(req);
						req = pre_node->next_node;
						ssd->request_queue_length--;
					}

				}
			}
			else
			{	
				pre_node = req;
				req = req->next_node;
			}
		}		
	}
}


/*******************************************************************************
*statistic_output()函数主要是输出处理完一条请求后的相关处理信息。
*1，计算出每个plane的擦除次数即plane_erase和总的擦除次数即erase
*2，打印min_lsn，max_lsn，read_count，program_count等统计信息到文件outputfile中。
*3，打印相同的信息到文件statisticfile中
*******************************************************************************/
void statistic_output(struct ssd_info *ssd)
{
	unsigned int lpn_count=0,i,j,k,m,erase=0,plane_erase=0;
	double gc_energy=0.0;
#ifdef DEBUG
	printf("enter statistic_output,  current time:%I64u\n",ssd->current_time);
#endif

	for(i=0;i<ssd->parameter->channel_number;i++)
	{
		for(j=0;j<ssd->parameter->die_chip;j++)
		{
			for(k=0;k<ssd->parameter->plane_die;k++)
			{
				plane_erase=0;
				for(m=0;m<ssd->parameter->block_plane;m++)
				{
					if(ssd->channel_head[i].chip_head[0].die_head[j].plane_head[k].blk_head[m].erase_count>0)
					{
						erase=erase+ssd->channel_head[i].chip_head[0].die_head[j].plane_head[k].blk_head[m].erase_count;
						plane_erase+=ssd->channel_head[i].chip_head[0].die_head[j].plane_head[k].blk_head[m].erase_count;
					}
				}
				fprintf(ssd->outputfile,"the %d channel, %d chip, %d die, %d plane has : %13d erase operations\n",i,j,k,m,plane_erase);
				fprintf(ssd->statisticfile,"the %d channel, %d chip, %d die, %d plane has : %13d erase operations\n",i,j,k,m,plane_erase);
			}
		}
	}

	fprintf(ssd->outputfile,"\n");
	fprintf(ssd->outputfile,"\n");
	fprintf(ssd->outputfile,"---------------------------statistic data---------------------------\n");	 
	fprintf(ssd->outputfile,"min lsn: %13d\n",ssd->min_lsn);	
	fprintf(ssd->outputfile,"max lsn: %13d\n",ssd->max_lsn);
	fprintf(ssd->outputfile,"read count: %13d\n",ssd->read_count);	  
	fprintf(ssd->outputfile,"program count: %13d",ssd->program_count);	
	fprintf(ssd->outputfile,"                        include the flash write count leaded by read requests\n");
	fprintf(ssd->outputfile,"the read operation leaded by un-covered update count: %13d\n",ssd->update_read_count);
	fprintf(ssd->outputfile,"erase count: %13d\n",ssd->erase_count);
	fprintf(ssd->outputfile,"direct erase count: %13d\n",ssd->direct_erase_count);
	fprintf(ssd->outputfile,"copy back count: %13d\n",ssd->copy_back_count);
	fprintf(ssd->outputfile,"multi-plane program count: %13d\n",ssd->m_plane_prog_count);
	fprintf(ssd->outputfile,"multi-plane read count: %13d\n",ssd->m_plane_read_count);
	fprintf(ssd->outputfile,"interleave write count: %13d\n",ssd->interleave_count);
	fprintf(ssd->outputfile,"interleave read count: %13d\n",ssd->interleave_read_count);
	fprintf(ssd->outputfile,"interleave two plane and one program count: %13d\n",ssd->inter_mplane_prog_count);
	fprintf(ssd->outputfile,"interleave two plane count: %13d\n",ssd->inter_mplane_count);
	fprintf(ssd->outputfile,"gc copy back count: %13d\n",ssd->gc_copy_back);
	fprintf(ssd->outputfile,"write flash count: %13d\n",ssd->write_flash_count);
	fprintf(ssd->outputfile,"interleave erase count: %13d\n",ssd->interleave_erase_count);
	fprintf(ssd->outputfile,"multiple plane erase count: %13d\n",ssd->mplane_erase_conut);
	fprintf(ssd->outputfile,"interleave multiple plane erase count: %13d\n",ssd->interleave_mplane_erase_count);
	fprintf(ssd->outputfile,"read request count: %13d\n",ssd->read_request_count);
	fprintf(ssd->outputfile,"write request count: %13d\n",ssd->write_request_count);
	fprintf(ssd->outputfile,"read request average size: %13f\n",ssd->ave_read_size);
	fprintf(ssd->outputfile,"write request average size: %13f\n",ssd->ave_write_size);
	// Modified by XXF avoid divide by zero
	if (ssd->read_request_count != 0)
	fprintf(ssd->outputfile,"read request average response time: %16I64u\n",ssd->read_avg/ssd->read_request_count);
	// Modified by XXF avoid divide by zero
	if (ssd->write_request_count != 0)
	fprintf(ssd->outputfile,"write request average response time: %16I64u\n",ssd->write_avg/ssd->write_request_count);
	fprintf(ssd->outputfile,"buffer read hits: %13d\n",ssd->dram->buffer->read_hit);
	fprintf(ssd->outputfile,"buffer read miss: %13d\n",ssd->dram->buffer->read_miss_hit);
	fprintf(ssd->outputfile,"buffer write hits: %13d\n",ssd->dram->buffer->write_hit);
	fprintf(ssd->outputfile,"buffer write miss: %13d\n",ssd->dram->buffer->write_miss_hit);
	fprintf(ssd->outputfile,"erase: %13d\n",erase);
	fflush(ssd->outputfile);

	fclose(ssd->outputfile);


	fprintf(ssd->statisticfile,"\n");
	fprintf(ssd->statisticfile,"\n");
	fprintf(ssd->statisticfile,"---------------------------statistic data---------------------------\n");	
	fprintf(ssd->statisticfile,"min lsn: %13d\n",ssd->min_lsn);	
	fprintf(ssd->statisticfile,"max lsn: %13d\n",ssd->max_lsn);
	fprintf(ssd->statisticfile,"read count: %13d\n",ssd->read_count);	  
	fprintf(ssd->statisticfile,"program count: %13d",ssd->program_count);	  
	fprintf(ssd->statisticfile,"                        include the flash write count leaded by read requests\n");
	fprintf(ssd->statisticfile,"the read operation leaded by un-covered update count: %13d\n",ssd->update_read_count);
	fprintf(ssd->statisticfile,"erase count: %13d\n",ssd->erase_count);	  
	fprintf(ssd->statisticfile,"direct erase count: %13d\n",ssd->direct_erase_count);
	fprintf(ssd->statisticfile,"copy back count: %13d\n",ssd->copy_back_count);
	fprintf(ssd->statisticfile,"multi-plane program count: %13d\n",ssd->m_plane_prog_count);
	fprintf(ssd->statisticfile,"multi-plane read count: %13d\n",ssd->m_plane_read_count);
	fprintf(ssd->statisticfile,"interleave count: %13d\n",ssd->interleave_count);
	fprintf(ssd->statisticfile,"interleave read count: %13d\n",ssd->interleave_read_count);
	fprintf(ssd->statisticfile,"interleave two plane and one program count: %13d\n",ssd->inter_mplane_prog_count);
	fprintf(ssd->statisticfile,"interleave two plane count: %13d\n",ssd->inter_mplane_count);
	fprintf(ssd->statisticfile,"gc copy back count: %13d\n",ssd->gc_copy_back);
	fprintf(ssd->statisticfile,"write flash count: %13d\n",ssd->write_flash_count);
	fprintf(ssd->statisticfile,"waste page count: %13d\n",ssd->waste_page_count);
	fprintf(ssd->statisticfile,"interleave erase count: %13d\n",ssd->interleave_erase_count);
	fprintf(ssd->statisticfile,"multiple plane erase count: %13d\n",ssd->mplane_erase_conut);
	fprintf(ssd->statisticfile,"interleave multiple plane erase count: %13d\n",ssd->interleave_mplane_erase_count);
	fprintf(ssd->statisticfile,"read request count: %13d\n",ssd->read_request_count);
	fprintf(ssd->statisticfile,"write request count: %13d\n",ssd->write_request_count);
	fprintf(ssd->statisticfile,"read request average size: %13f\n",ssd->ave_read_size);
	fprintf(ssd->statisticfile,"write request average size: %13f\n",ssd->ave_write_size);
	// Modified by XXF avoid divide by zero
	if (ssd->read_request_count != 0)
	fprintf(ssd->statisticfile,"read request average response time: %16I64u\n",ssd->read_avg/ssd->read_request_count);
	// Modified by XXF avoid divide by zero
	if (ssd->write_request_count != 0)
	fprintf(ssd->statisticfile,"write request average response time: %16I64u\n",ssd->write_avg/ssd->write_request_count);
	fprintf(ssd->statisticfile,"buffer read hits: %13d\n",ssd->dram->buffer->read_hit);
	fprintf(ssd->statisticfile,"buffer read miss: %13d\n",ssd->dram->buffer->read_miss_hit);
	fprintf(ssd->statisticfile,"buffer write hits: %13d\n",ssd->dram->buffer->write_hit);
	fprintf(ssd->statisticfile,"buffer write miss: %13d\n",ssd->dram->buffer->write_miss_hit);
	fprintf(ssd->statisticfile,"erase: %13d\n",erase);
	fflush(ssd->statisticfile);

	fclose(ssd->statisticfile);
}


/***********************************************************************************
*根据每一页的状态计算出每一需要处理的子页的数目，也就是一个子请求需要处理的子页的页数
************************************************************************************/
unsigned int size(unsigned int stored)
{
	unsigned int i,total=0,mask=0x80000000;

	#ifdef DEBUG
	printf("enter size\n");
	#endif
	for(i=1;i<=32;i++)
	{
		if(stored & mask) total++;
		stored<<=1;
	}
	#ifdef DEBUG
	    printf("leave size\n");
    #endif
    return total;
}


/*********************************************************
*transfer_size()函数的作用就是计算出子请求的需要处理的size
*函数中单独处理了first_lpn，last_lpn这两个特别情况，因为这
*两种情况下很有可能不是处理一整页而是处理一页的一部分，因
*为lsn有可能不是一页的第一个子页。
*********************************************************/
unsigned int transfer_size(struct ssd_info *ssd,int need_distribute,unsigned int lpn,struct request *req)
{
	unsigned int first_lpn,last_lpn,state,trans_size;
	unsigned int mask=0,offset1=0,offset2=0;

	first_lpn=req->lsn/ssd->parameter->subpage_page;
	last_lpn=(req->lsn+req->size-1)/ssd->parameter->subpage_page;

	mask=~(0xffffffff<<(ssd->parameter->subpage_page));
	state=mask;
	if(lpn==first_lpn)
	{
		offset1=ssd->parameter->subpage_page-((lpn+1)*ssd->parameter->subpage_page-req->lsn);
		state=state&(0xffffffff<<offset1);
	}
	if(lpn==last_lpn)
	{
		offset2=ssd->parameter->subpage_page-((lpn+1)*ssd->parameter->subpage_page-(req->lsn+req->size));
		state=state&(~(0xffffffff<<offset2));
	}

	trans_size=size(state&need_distribute);

	return trans_size;
}


/**********************************************************************************************************  
*__int64 find_nearest_event(struct ssd_info *ssd)       
*寻找所有子请求的最早到达的下个状态时间,首先看请求的下一个状态时间，如果请求的下个状态时间小于等于当前时间，
*说明请求被阻塞，需要查看channel或者对应die的下一状态时间。Int64是有符号 64 位整数数据类型，值类型表示值介于
*-2^63 ( -9,223,372,036,854,775,808)到2^63-1(+9,223,372,036,854,775,807 )之间的整数。存储空间占 8 字节。
*channel,die是事件向前推进的关键因素，三种情况可以使事件继续向前推进，channel，die分别回到idle状态，die中的
*读数据准备好了
***********************************************************************************************************/
__int64 find_nearest_event(struct ssd_info *ssd) 
{
	unsigned int i,j;
	__int64 time=0x7fffffffffffffff;
	__int64 time1=0x7fffffffffffffff;
	__int64 time2=0x7fffffffffffffff;

	for (i=0;i<ssd->parameter->channel_number;i++)
	{
		if (ssd->channel_head[i].next_state==CHANNEL_IDLE)
			if(time1>ssd->channel_head[i].next_state_predict_time)
				if (ssd->channel_head[i].next_state_predict_time>ssd->current_time)    
					time1=ssd->channel_head[i].next_state_predict_time;
		for (j=0;j<ssd->parameter->chip_channel[i];j++)
		{
			if ((ssd->channel_head[i].chip_head[j].next_state==CHIP_IDLE)||(ssd->channel_head[i].chip_head[j].next_state==CHIP_DATA_TRANSFER))
				if(time2>ssd->channel_head[i].chip_head[j].next_state_predict_time)
					if (ssd->channel_head[i].chip_head[j].next_state_predict_time>ssd->current_time)    
						time2=ssd->channel_head[i].chip_head[j].next_state_predict_time;	
		}   
	} 

	/*****************************************************************************************************
	 *time为所有 A.下一状态为CHANNEL_IDLE且下一状态预计时间大于ssd当前时间的CHANNEL的下一状态预计时间
	 *           B.下一状态为CHIP_IDLE且下一状态预计时间大于ssd当前时间的DIE的下一状态预计时间
	 *		     C.下一状态为CHIP_DATA_TRANSFER且下一状态预计时间大于ssd当前时间的DIE的下一状态预计时间
	 *CHIP_DATA_TRANSFER读准备好状态，数据已从介质传到了register，下一状态是从register传往buffer中的最小值 
	 *注意可能都没有满足要求的time，这时time返回0x7fffffffffffffff 。
	*****************************************************************************************************/
	time=(time1>time2)?time2:time1;
	return time;
}

/***********************************************
*free_all_node()函数的作用就是释放所有申请的节点
************************************************/
void free_all_node(struct ssd_info *ssd)
{
	unsigned int i,j,k,l,n;
	struct buffer_group *pt=NULL;
	struct direct_erase * erase_node=NULL;
	for (i=0;i<ssd->parameter->channel_number;i++)
	{
		for (j=0;j<ssd->parameter->chip_channel[0];j++)
		{
			for (k=0;k<ssd->parameter->die_chip;k++)
			{
				for (l=0;l<ssd->parameter->plane_die;l++)
				{
					for (n=0;n<ssd->parameter->block_plane;n++)
					{
						free(ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[n].page_head);
						ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[n].page_head=NULL;
					}
					free(ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head);
					ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head=NULL;
					while(ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].erase_node!=NULL)
					{
						erase_node=ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].erase_node;
						ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].erase_node=erase_node->next_node;
						free(erase_node);
						erase_node=NULL;
					}
				}
				
				free(ssd->channel_head[i].chip_head[j].die_head[k].plane_head);
				ssd->channel_head[i].chip_head[j].die_head[k].plane_head=NULL;
			}
			free(ssd->channel_head[i].chip_head[j].die_head);
			ssd->channel_head[i].chip_head[j].die_head=NULL;
		}
		free(ssd->channel_head[i].chip_head);
		ssd->channel_head[i].chip_head=NULL;
	}
	free(ssd->channel_head);
	ssd->channel_head=NULL;

	avlTreeDestroy( ssd->dram->buffer);
	ssd->dram->buffer=NULL;
	
	free(ssd->dram->map->map_entry);
	ssd->dram->map->map_entry=NULL;
	free(ssd->dram->map);
	ssd->dram->map=NULL;
	free(ssd->dram);
	ssd->dram=NULL;
	free(ssd->parameter);
	ssd->parameter=NULL;

	free(ssd);
	ssd=NULL;
}


/*****************************************************************************
*make_aged()函数的作用就死模拟真实的用过一段时间的ssd，
*那么这个ssd的相应的参数就要改变，所以这个函数实质上就是对ssd中各个参数的赋值。
******************************************************************************/
struct ssd_info *make_aged(struct ssd_info *ssd)
{
	unsigned int i,j,k,l,m,n,ppn;
	int threshould,flag=0;
    
	if (ssd->parameter->aged==1)
	{
		//threshold表示一个plane中有多少页需要提前置为失效
		threshould=(int)(ssd->parameter->block_plane*ssd->parameter->page_block*ssd->parameter->aged_ratio);  
		for (i=0;i<ssd->parameter->channel_number;i++)
			for (j=0;j<ssd->parameter->chip_channel[i];j++)
				for (k=0;k<ssd->parameter->die_chip;k++)
					for (l=0;l<ssd->parameter->plane_die;l++)
					{  
						flag=0;
						for (m=0;m<ssd->parameter->block_plane;m++)
						{  
							if (flag>=threshould)
							{
								break;
							}
							for (n=0;n<(ssd->parameter->page_block*ssd->parameter->aged_ratio+1);n++)
							{  
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].page_head[n].valid_state=0;        //表示某一页失效，同时标记valid和free状态都为0
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].page_head[n].free_state=0;         //表示某一页失效，同时标记valid和free状态都为0
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].page_head[n].lpn=0;  //把valid_state free_state lpn都置为0表示页失效，检测的时候三项都检测，单独lpn=0可以是有效页
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].free_page_num--;
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].invalid_page_num++;
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].blk_head[m].last_write_page++;
								ssd->channel_head[i].chip_head[j].die_head[k].plane_head[l].free_page--;
								flag++;

								ppn=find_ppn(ssd,i,j,k,l,m,n);
							
							}
						} 
					}	 
	}  
	else
	{
		return ssd;
	}

	return ssd;
}


/*********************************************************************************************
*no_buffer_distribute()函数是处理当ssd没有dram的时候，
*这是读写请求就不必再需要在buffer里面寻找，直接利用creat_sub_request()函数创建子请求，再处理。
*********************************************************************************************/
struct ssd_info *no_buffer_distribute(struct ssd_info *ssd)
{
	unsigned int lsn,lpn,last_lpn,first_lpn,complete_flag=0, state;
	unsigned int flag=0,flag1=1,active_region_flag=0;           //to indicate the lsn is hitted or not
	struct request *req=NULL;
	struct sub_request *sub=NULL,*sub_r=NULL,*update=NULL;
	struct local *loc=NULL;
	struct channel_info *p_ch=NULL;

	
	unsigned int mask=0; 
	unsigned int offset1=0, offset2=0;
	unsigned int sub_size=0;
	unsigned int sub_state=0;

	ssd->dram->current_time=ssd->current_time;
	req=ssd->request_tail;       
	lsn=req->lsn;
	lpn=req->lsn/ssd->parameter->subpage_page;
	last_lpn=(req->lsn+req->size-1)/ssd->parameter->subpage_page;
	first_lpn=req->lsn/ssd->parameter->subpage_page;

	if(req->operation==READ)        
	{		
		while(lpn<=last_lpn) 		
		{
			sub_state=(ssd->dram->map->map_entry[lpn].state&0x7fffffff);
			sub_size=size(sub_state);
			sub=creat_sub_request(ssd,lpn,sub_size,sub_state,req,req->operation);
			lpn++;
		}
	}
	else if(req->operation==WRITE)
	{
		while(lpn<=last_lpn)     	
		{	
			mask=~(0xffffffff<<(ssd->parameter->subpage_page));
			state=mask;
			if(lpn==first_lpn)
			{
				offset1=ssd->parameter->subpage_page-((lpn+1)*ssd->parameter->subpage_page-req->lsn);
				state=state&(0xffffffff<<offset1);
			}
			if(lpn==last_lpn)
			{
				offset2=ssd->parameter->subpage_page-((lpn+1)*ssd->parameter->subpage_page-(req->lsn+req->size));
				state=state&(~(0xffffffff<<offset2));
			}
			sub_size=size(state);

			sub=creat_sub_request(ssd,lpn,sub_size,state,req,req->operation);
			lpn++;
		}
	}

	return ssd;
}
