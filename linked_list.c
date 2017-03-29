#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct {             /* 定义一个结构体来作为链表内的数据类型 */
  int order;                 /* 多项式的次数 */
  int coefficient;           /* 多项式的系数 */
} DATA_TYPE;

typedef struct _ListNode ListNode;

struct _ListNode {              /* 定义单链表的节点的数据类型 */
  DATA_TYPE *value;            /*节点里的数据 */
  ListNode *next;            /* 指向下一个节点的指针 */
};

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
  ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));      /* 声明本次插入的节点，之后返回值会用到 */
  new_node->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  new_node->next = (ListNode *)malloc(sizeof(ListNode));
  if (linked_list->value->order < new_value->order) {                        /* 判断新加入的次数是否比最高次数大，是则插入表头，否则开始遍历链表 */
    ListNode *temp_node = (ListNode *)malloc(sizeof(ListNode));
    temp_node->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
    temp_node->next = (ListNode *)malloc(sizeof(ListNode));
    memcpy(temp_node->value, linked_list->value, sizeof(DATA_TYPE *));
    temp_node->next = linked_list->next;
    memcpy(linked_list->value, new_value, sizeof(DATA_TYPE *));
    linked_list->next = temp_node;
    new_node = linked_list;
  }
  else{       /* 开始遍历链表，找到要插入的位置 */
    for(; linked_list->value->order > new_value->order && linked_list->next != NULL; linked_list = linked_list->next);
    if (linked_list->value->order == new_value->order){                       /* 判断次数是否相等，如果相等则系数相加 */
      linked_list->value->coefficient += new_value->coefficient;
      new_node = linked_list;
    }
    else{          /* 原多项式中没有该次项，故创建一个新节点并插入链表 */
      new_node->value = new_value;
      new_node->next = linked_list->next;
      linked_list->next = new_node;
    }
  }
  return new_node;          /* 插入成功，返回值为本次插入节点，这有利于多项式相加效率的提高，因为在插入下一项时不用遍历上一次插入点之前的次数高的节点 */
}

/* 用于拷贝线性表 */
ListNode *copy_list(ListNode *list){            /* 输入参数为需要拷贝的线性表，返回一个与输入表内容一样的新表 */
  if (list == NULL) {
    printf("The list is empty!\n");
    return 0;
  }
  ListNode *new_list = (ListNode *)malloc(sizeof(ListNode));
  ListNode *new_list_copy = new_list;
  for(; list != NULL; list = list->next){
    new_list->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
    memcpy(new_list->value, list->value, sizeof(DATA_TYPE));
    if (list->next != NULL) {
      new_list->next = (ListNode *)malloc(sizeof(ListNode));
      new_list = new_list->next;
    }
  }
  new_list->next = NULL;
  new_list = new_list_copy;
  return new_list;
}

/* 用于多项式相加 */
/* 两个多项式的相加可以看做是将其中一个链表逐项插入另一个链表，由于两个链表都是排好续的，因此可以在插入第二项时，可以从第一项插入的位置开始遍历，提高速度 */
ListNode *list_add (ListNode *linked_list1, ListNode *linked_list2){             /* 输入的两个参数为两个多项式 */
  if (linked_list1 == NULL && linked_list2 == NULL) {         /* 判断是否为空表 */
    printf("Please input non-empty polynomial!\n");
    return 0;
  }
  if (linked_list1 == NULL && linked_list2 != NULL) {
    return linked_list2;
  }
  if (linked_list1 != NULL && linked_list2 == NULL) {
    return linked_list1;
  }
  ListNode *final_list = (ListNode *)malloc(sizeof(ListNode));      /* 先申明相加后的链表，并分配内存 */
  final_list = copy_list(linked_list1);       /* 将表１拷贝到最终链表 */
  ListNode *final_list_copy = final_list;
  for (; linked_list2 != NULL; linked_list2 = linked_list2->next){
    final_list = insert_node(final_list, linked_list2->value);
  }
  final_list = final_list_copy;
  return final_list;
}

/* 用于多项式与多项式的相乘 */
/* 多项式相乘可以分解为两步，先让一个多项式乘以另一个多项式的各项，然后合并多项式 */
ListNode *list_multiply_list (ListNode *linked_list1, ListNode *linked_list2){      /* 输入的两个参数为两个多项式 */
  if (linked_list1 == NULL || linked_list2 == NULL) {         /* 判断是否为空表 */
    return 0;
  }
  ListNode *final_list = (ListNode *)malloc(sizeof(ListNode));            /* 初始化代表相乘结果的多项式的指针，并赋0值 */
  final_list->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  final_list->value->order = 0;
  final_list->value->coefficient = 0;
  final_list->next = NULL;
  ListNode *final_list_copy = final_list;
  ListNode *linked_list1_copy = linked_list1;         /* 复制第一个多项式的首项指针，用来遍历第一个多项式 */
  for(; linked_list2 != NULL; linked_list2 = linked_list2->next){       /* 将第二个多项式的每一项与第一个多项式相乘，然后相加 */
    for (linked_list1; linked_list1 != NULL; linked_list1 = linked_list1->next) {
      DATA_TYPE *temp_value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
      temp_value->coefficient = linked_list1->value->coefficient * linked_list2->value->coefficient;
      temp_value->order = linked_list1->value->order + linked_list2->value->order;
      final_list = insert_node(final_list, temp_value);
    }
    linked_list1 = linked_list1_copy;     /* 将linked_list1重新指向第一个节点 */
    final_list = final_list_copy;
  }
  return final_list;
}

/* 用于打印多项式 */
int print_list (ListNode *list){           /* 输入参数为要打印的多项式 */
  if (list == NULL) {         /* 判断是否为空表 */
    printf("List is empty!\n");
    return 0;
  }
  printf("%dx^%d", list->value->coefficient, list->value->order);           /* 第一个项的系数的符号只有当为负数时才打印出来 */
  list = list->next;
  for (; list != NULL; list = list->next) {
    printf("%+dx^%d", list->value->coefficient, list->value->order);      /* 第一个项之后的项要将系数的符号打印出来，正数也要打出正号 */
  }
  printf("\n");
  return 1;
}

int main() {
  /* 建一个测试表 */
  ListNode *test_list1 = (ListNode *)malloc(sizeof(ListNode));     /* 建一个测试表 */
  test_list1->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  test_list1->value->coefficient = 2;
  test_list1->value->order = 5;
  test_list1->next = NULL;

  /* 接下来建立三个测试数据，次数分别是比5大，等于5，比5小，用于测试insert_node能否在三种情况下正常工作 */
  DATA_TYPE *test_value1 = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));         /* 建一个次数大于5的测试数据 */
  test_value1->coefficient = 3;
  test_value1->order = 6;
  DATA_TYPE *test_value2 = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));         /* 建一个次数等于5的测试数据 */
  test_value2->coefficient = 7;
  test_value2->order = 5;
  DATA_TYPE *test_value3 = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));         /* 建一个次数小于5的测试数据 */
  test_value3->coefficient = 4;
  test_value3->order = 3;

  /* 开始测试 */

  /* 先测试打印功能 */
  printf("\n");
  printf("/* Now test the print_list function */\n");
  printf("The test list 1 is ");
  assert(print_list(test_list1));
  printf("\n");

  /* 测试插入功能，分别插入上述三个数据 */
  printf("/* Now test the insert_node function */\n");
  printf("Result after inserting 3x^6 is ");
  assert(insert_node(test_list1, test_value1));            /* 测试插入第一个数据 */
  assert(print_list(test_list1));
  printf("Result after inserting 7x^5 is ");
  assert(insert_node(test_list1, test_value2));            /* 测试插入第二个数据 */
  assert(print_list(test_list1));
  printf("Result after inserting 4x^3 is ");
  assert(insert_node(test_list1, test_value3));            /* 测试插入第三个数据 */
  assert(print_list(test_list1));
  printf("\n");

  /* 测试链表拷贝功能 */
  printf("/* Now test the copy_list function */\n");
  assert(copy_list(test_list1));
  printf("Copy test list 1 to a new list, the new list is ");
  print_list(copy_list(test_list1));
  printf("\n");

  /* 建立第二个测试表 */
  ListNode *test_list2 = (ListNode *)malloc(sizeof(ListNode));
  test_list2->value = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  test_list2->value->coefficient = 2;
  test_list2->value->order = 8;
  test_list2->next = NULL;
  DATA_TYPE *node2 = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
  node2->coefficient = 9;
  node2->order = 7;
  insert_node(test_list2, node2);

  /* 测试两个多项式相加和相乘 */
  printf("/* Now test the list_add and list_multiply_list function */\n");
  printf("List1 is ");
  print_list(test_list1);
  printf("List2 is ");
  print_list(test_list2);
  printf("List1 + List2 = ");
  assert(list_add(test_list1, test_list2));
  print_list(list_add(test_list1, test_list2));
  printf("List1 * List2 = ");
  print_list(list_multiply_list(test_list1, test_list2));
  return 0;
}
