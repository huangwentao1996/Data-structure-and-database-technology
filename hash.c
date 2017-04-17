#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define NAME_SIZE 20

typedef long int key;
typedef struct _ListNode ListNode;
typedef struct _HashTable HashTable;

/* 本散列表以储存学生的姓名和学号为例 */
/* 本散列表采用链地址法，用一个表来储存链表头的位置，用链表储存具体数据 */

/* 每个节点的数据结构体 */
struct _ListNode {
  key student_id;        /* 储存学生ID */
  char *student_name;     /* 储存学生姓名 */
  ListNode *next;   /* 储存下一个节点的位置 */
  int label;    /* 储存0和1分别代表fasle或true，指示该节点是否被占 */
};

/* 储存链表头的表 */
struct _HashTable {
  unsigned int hash_value;
  unsigned int used_length;
  unsigned int total_length;
  ListNode *list_entry;
};

/* 用于建立一个新的哈希表 */
HashTable *hash_creat_table(){
  HashTable *new_table;
  new_table = (HashTable *)malloc(sizeof(HashTable) * 100);
  int i = 0;
  int j = 0;
  ListNode *p;
  while (i <= 99) {
    new_table[i].hash_value = i;      /* 学号除以100的余数作为key_value */
    new_table[i].total_length = 100;
    new_table[i].used_length = 0;
    new_table[i].list_entry = (ListNode *)malloc(sizeof(ListNode) * new_table[i].total_length);
    p = new_table[i].list_entry;
    while (j <= 99) {     /* 初始化链表里的每一项的label为false代表未占用 */
      p[j].label = 0;
      p[j].next = NULL;
      p[j].student_name = (char *)malloc(sizeof(char) * NAME_SIZE);
      j++;
    }
    j = 0;
    i++;
  }
  return new_table;
}

/* 用于计算给定数据的特征值，特征值为学号除以100的余数 */
int hash_calculate_value(key student_id) {
  int key_value;
  key_value = student_id % 100;
  return key_value;
}

/* 用于判断链表是否满了 */
int hash_if_full(HashTable *table) {
  double used_fraction;
  double used_length = table->used_length;
  double total_length = table->total_length;
  used_fraction = (used_length / total_length);
  if (used_fraction >= 0.33) {
    return 1;       /* 如果使用大于0.33，则返回1代表满 */
  }
  else{
    return 0;       /* 如果使用小于0.33，则返回0代表未满 */
  }
} /* 传入某个单链表 */

/* 用于free节点 */
int hash_free_node(HashTable *table, key id){
  ListNode *p;
  ListNode *q;
  p = table[hash_calculate_value(id)].list_entry;
  q = NULL;
  if (p->student_id == id) {      /* 判断是否要free的是第一个节点 */
    if (p->next != NULL) {
      memmove(p, p->next, sizeof(ListNode));  /* 将第二个非空节点移动到第一个节点 */
      p->next->label = 0;
    }
    else{
      p->label == 0;
    }
    table[hash_calculate_value(id)].used_length--;
    return 1;
  }
  while (p->next != NULL) {
    p = p->next;
    if (p->student_id == id) {
      p->label = 0;
      q->next = p->next;
      table[hash_calculate_value(id)].used_length--;
      return 1;
    }
    q = p;
  }
  puts("未找到要删除的节点");
  return 0;
}

/* 用来提取根据ID找学生姓名 */
char *hash_get_name(HashTable *table, key id){
  int hash_value;
  hash_value = hash_calculate_value(id);
  ListNode *p;
  p = table[hash_value].list_entry;
  while (p != NULL) {
    if (p->student_id == id) {
      return p->student_name;
    }
    p = p->next;
  }
  return NULL;
}

/* 用于扩大链表 */
int hash_enlarge_list(HashTable *table){
  ListNode *newlist;
  /* 将表长扩大一倍 */
  newlist = (ListNode *)realloc(table->list_entry, sizeof(ListNode) * table->total_length * 2);
  if(newlist){      /* 判断内存是否分配成功 */
    ListNode *p;
    ListNode *q;
    table->list_entry = newlist;
    int i = 1;
    p = table->list_entry;      /* 用来记录前一个数据的物理位置 */
    q = table->list_entry;      /* 用来记录后一个数据的物理位置 */
    /* 由于新链表的内存地址可能不同，所以要将每一个节点上的next指向的地址改到新节点 */
    while ( i < table->total_length) {
      q++;
      if ( q->label == 1) {
        p->next = q;
        p = q;
      }
      i++;
    }
    table->total_length = (2 * table->total_length);
    while (i < table->total_length) {
      q++;
      q->label = 0;
      q->next = NULL;
      q->student_name = (char *)malloc(sizeof(char) * NAME_SIZE);
      i++;
    }

    return 1;
  }
  else{
    puts("内存扩大失败");
    return 0;
  }
}

/* 用于插入数据 */
int hash_insert_data(HashTable *table, key id, char *name){
  unsigned int hash_value;
  int i;
  ListNode *p;
  ListNode *q;
  i = 0;
  hash_value = hash_calculate_value(id);
  if (hash_if_full(&table[hash_value])) {  /* 判断表是否满 */
    hash_enlarge_list(&table[hash_value]);
  }
  p = table[hash_value].list_entry;
  q = table[hash_value].list_entry;
  if (hash_get_name(table, id) != NULL) {     /* 判断表中是否已经存在该key */
    while (p != NULL) {
      if (p->student_id == id) {
        strcpy(p->student_name, name);
        return 1;
      }
      p = p->next;
    }
  }
  if (q->label == 0) {   /* 判断表头节点是否为空 */
    q->student_id = id;
    strcpy(q->student_name, name);
    q->label = 1;
    table[hash_value].used_length = table[hash_value].used_length + 1;
    return 1;
  }
  while (i <= table[hash_value].total_length) {  /* 找到一个表头之后可以插入的空位置 */
    if (p->label == 0) {
      break;
    }
    else {
      p++;
    }
    i++;
  }
  while (q->next != NULL) {
    q = q->next;
  }
  p->student_id = id;
  strcpy(p->student_name, name);
  p->label = 1;
  q->next = p;
  table[hash_value].used_length = table[hash_value].used_length + 1;
  return 1;
}

int main() {
  HashTable *test_table;
  char name[20] = "黄文韬";
  char *p = name;
  char *q;
  key a = 3140105751;
  /* 测试创建散列表函数 */
  assert(test_table = hash_creat_table());
  puts("散列表创建函数测试成功");
  assert(hash_insert_data(test_table, 3140105751, p));
  puts("散列表插入函数测试成功");
  assert(q = hash_get_name(test_table, 3140105751));
  puts("散列表查找函数测试成功");
  printf("学号为%ld的学生的姓名为%s\n", a, q);
  /* 下面开始连续插入120个数据，用来测试再散列功能 */
  for (int i = 0; i < 120; i++) {
    a = a+100;
    assert(hash_insert_data(test_table, a, p));
  }
  puts("再散列函数测试通过");
  /* 测试释放散列表节点 */
  assert(hash_free_node(test_table, a));
  puts("释放散列表节点函数测试通过");
  puts("所有函数测试通过！");
  return 0;
}
