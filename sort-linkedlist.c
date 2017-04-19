#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* 本程序用于链表的排序，将链表按照多项式的次数大小进行排序 */

typedef struct {             /* 定义一个结构体来作为链表内的数据类型 */
  int order;                 /* 多项式的次数 */
  double coefficient;           /* 多项式的系数 */
} DATA_TYPE;

typedef struct _ListNode ListNode;

struct _ListNode {              /* 定义单链表的节点的数据类型 */
  DATA_TYPE *value;            /*节点里的数据 */
  ListNode *next;            /* 指向下一个节点的指针 */
};

/* 用于创建新链表 */
ListNode *creat_list(){
  ListNode *list = (ListNode *)malloc(sizeof(ListNode));
  list->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  list->value->coefficient = 0;
  list->value->order = 0;
  list->next = NULL;
}

/* 用于向线性表插入新节点 */
ListNode *insert_node (ListNode *linked_list, DATA_TYPE *new_value) {
  if (new_value == NULL){           /* 若插入的数据为空，则不对原表做任何修改,且返回原表 */
    return linked_list;
  }
  if (linked_list->value->order == 0 && linked_list->value->coefficient == 0 && linked_list->next == NULL) {      /* 判断是否为空表 */
    linked_list->value->order = new_value->order;
    linked_list->value->coefficient = new_value->coefficient;
    return linked_list;
  }
  while (linked_list->next != NULL) {
    if (linked_list->value->order == new_value->order){             /* 判断次数是否相等，如果相等则系数相加 */
      linked_list->value->coefficient += new_value->coefficient;
      return linked_list;
    }
    linked_list = linked_list->next;
  }
  /* 原多项式中没有该次项，故创建一个新节点并插入链表 */
  ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));      /* 声明本次插入的节点，之后返回值会用到 */
  new_node->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  new_node->value->coefficient = new_value->coefficient;
  new_node->value->order = new_value->order;
  new_node->next = NULL;
  linked_list->next = new_node;
  return new_node;          /* 插入成功，返回值为本次插入节点，这有利于多项式相加效率的提高，因为在插入下一项时不用遍历上一次插入点之前的次数高的节点 */
}

/* 用于打印多项式 */
int print_list (ListNode *list){           /* 输入参数为要打印的多项式 */
  if (list == NULL) {         /* 判断是否为空表 */
    printf("List is empty!\n");
    return 0;
  }
  printf("%.1fx^%d", list->value->coefficient, list->value->order);         /* 第一个项的系数的符号只有当为负数时才打印出来 */
  list = list->next;
  while (list != NULL) {
    printf("%+.1fx^%d", list->value->coefficient, list->value->order);      /* 第一个项之后的项要将系数的符号打印出来，正数也要打出正号 */
    list = list->next;
  }
  printf("\n");
  return 1;
}

/* 采用冒泡法对链表按照多项式的次数高低进行排序 */
int sort_list(ListNode *list){
  if (list->next == NULL) {   /* 如果链表只有一个节点则不用排序 */
    return 1;
  }
  ListNode *p;
  ListNode *q;
  int i;    /* 作为一个是否发生交换的指针 */
  do {
    i = 0;
    p = list;
    q = p->next;
    while (q != NULL) {
      /* 如果p的次数小于q，则交换一下数据 */
      if (p->value->coefficient < q->value->coefficient) {
        int a;
        double b;
        a = p->value->coefficient;
        b = p->value->order;
        p->value->coefficient = q->value->coefficient;
        p->value->order = q->value->order;
        q->value->coefficient = a;
        q->value->order = b;
        i = 1;    /* 若发生交换则i取1 */
      }
      p = q;    /* 将p,q都后移一个位置 */
      q = p->next;
    }
  } while(i == 1);   /* 若上一个循环发生交换则继续对比 */
  return 1;
}

int main(){
  ListNode *testlist;
  assert(testlist = creat_list());
  int order = 100;
  DATA_TYPE testdata;
  testdata.order = 1;
  testdata.coefficient = 1;
  while (testdata.order <= 10) {
    assert(insert_node(testlist, &testdata));
    testdata.order++;
    testdata.coefficient++;
  }
  puts("原多项式为");
  print_list(testlist);
  puts("多项式排序之后为");
  assert(sort_list(testlist));
  print_list(testlist);
  return 0;
}
