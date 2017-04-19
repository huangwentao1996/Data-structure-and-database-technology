#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define INITIAL_LENGTH 10000   /* ArrayList初始长度为10000 */

typedef int DATA_TYPE;        /* 规定数据类型为void */

typedef struct {                      /* 定义ArrayList的结构*/
  DATA_TYPE *data;
  unsigned int length;
  unsigned int used_length;
} Array_List;

Array_List *Creat_List(){            /* 创建ArrayList的函数，若创建成功则返回所创建的ArrayList的指针 */
  Array_List *sample_list;
  sample_list = (Array_List *)malloc(sizeof(Array_List));
  sample_list->data = (DATA_TYPE *)malloc(INITIAL_LENGTH * sizeof(DATA_TYPE));
  sample_list->used_length = 0;
  sample_list->length = INITIAL_LENGTH;
  return sample_list;
}

int Enlarge_List(Array_List *list){         /* 对ArrayList进行扩容 */
  unsigned int newlength = list->length * 2;                /* 将表长扩大一倍 */
  DATA_TYPE *newdata;
  newdata = (DATA_TYPE *)realloc(list->data, sizeof(DATA_TYPE) * newlength);
  if(newdata == NULL){
    puts("内存扩大失败");
    return 0;
  }
  else{
    list->data = newdata;
    list->length = newlength;
    return 1;
  }
}

int ArrayList_Insert(Array_List *list, int insert_position, DATA_TYPE *insert_data){     /* insert_position代表插入的位置，等于1时即代表插在表头 */
  if (insert_position <= 0 || insert_position > list->length){     /* 判断插入位置是否合法，插入位置应在1到length之间 */
    puts("插入位置应在1到length之间");
    return 0;
  }
  if (list->used_length == list->length)  Enlarge_List(list);     /* 对ArrayList的使用情况进行判断，若容量已使用完则进行扩容 */
  memmove(&list->data[insert_position], &list->data[insert_position-1], (list->used_length - insert_position + 1) * sizeof(DATA_TYPE) );
  list->data[insert_position-1] = *insert_data;
  list->used_length++;
  return 1;
}

/* 采用冒泡法对ArrayList按照多项式的次数高低进行排序 */
int sort_array_list(Array_List *list){
  if (list->used_length == 0) {   /* 如果链表只有一个节点则不用排序 */
    return 1;
  }
  DATA_TYPE *p;
  DATA_TYPE *q;
  int i;    /* 作为一个是否发生交换的指针 */
  int j;
  do {
    i = 0;
    p = list->data;
    q = &p[1];
    j = 1;
    while (j < list->used_length) {
      /* 如果p大于q，则交换一下数据 */
      if (*p > *q) {
        DATA_TYPE a;
        a = *p;
        *p = *q;
        *q = a;
        i = 1;    /* 若发生交换则i取1 */
      }
      p++;    /* 将p,q都后移一个位置 */
      q++;
      j++;
    }
  } while(i == 1);   /* 若上一个循环发生交换则继续对比 */
  return 1;
}

int print_array_list(Array_List *list){
  int i = 0;
  int a;
  while (i < (list->used_length-1)) {
    a = list->data[i];
    printf("%d,", a);
    i++;
  }
  a = list->data[i];
  printf("%d\n", a);
  return 1;
}

int main() {            /* 将在main函数里用assert函数对各个函数逐个进行测试 */
  assert(Creat_List());       /* 对Creat_List函数进行测试 */
  Array_List *testlist = Creat_List();    /* 建立一个测试list，用于测试其他函数 */
  DATA_TYPE a = 10;
  int b= 1;
  DATA_TYPE *p = &a;
  while (a >= 1) {
    assert(ArrayList_Insert(testlist, b, p));
    b++;
    a--;
  }
  assert(Enlarge_List(testlist));
  puts("未排序之前的arraylist为");
  assert(print_array_list(testlist));
  assert(sort_array_list(testlist));
  puts("排序之后的arraylist为");
  assert(print_array_list(testlist));
  return 0;
}
