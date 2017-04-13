#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct _Matrix_Node Matrix_Node;
typedef double DATA_TYPE;

struct _Matrix_Node {        /* 定义节点的数据类型 */
  int row;
  int col;
  DATA_TYPE value;
  Matrix_Node *right;
  Matrix_Node *down;
};

/* 用于创建矩阵，返回值为总头结点的指针 */
Matrix_Node *creat_matrix(int row, int col){
  if ( row <= 0 || col <= 0) {
    puts("创建矩阵的行数或列数不合法！");
    return 0;
  }
  Matrix_Node *head;        /* 创建头节点 */
  int node_num;     /* 需要创建的节点数量 */
  node_num = row > col ? row : col;
  head = (Matrix_Node *)malloc((node_num+1) * sizeof (Matrix_Node));
  head->row = row;
  head->col = col;
  head->right = NULL;
  head->down = NULL;
  int i = 1;
  while (i <= node_num) {
    head[i].row = 0;
    head[i].col = 0;
    head[i].down = NULL;
    head[i].right = NULL;
    i++;
  }
  return head;
}

/* 用于向矩阵中插入或修改数据 */
int insert_data(Matrix_Node *head, int row, int col, double val){
  if (row <= 0 || col <= 0 || row > head->row || col > head->col) {
    puts("插入的行数或列数不合法！");
    return 0;
  }
  Matrix_Node *node_row;
  Matrix_Node *node_col;
  node_row = &head[row];
  node_col = &head[col];
  while (node_row->right != NULL) {     /* 找到要行要插入的位置 */
    if (node_row->right->col >= col) break;
    node_row = node_row->right;
  }
  if (node_row->right != NULL) {     /* 判断矩阵中要插入的位置原来是否存在一个值，如果存在则用新值覆盖掉旧值 */
    if (node_row->right->col == col) {
      node_row->right->value = val;
      puts("警告！插入位置的原数据已被覆盖！");
      return 1;
    }
  }
  Matrix_Node *new_node;
  new_node = (Matrix_Node *)malloc(sizeof(Matrix_Node));
  new_node->row = row;
  new_node->col = col;
  new_node->value = val;
  new_node->right = node_row->right;
  node_row->right = new_node;
  while (node_col->down != NULL) {       /* 找到列要插入的位置 */
    if (node_col->down->row >= row) break;
    node_col = node_col->down;
  }
  new_node->down = node_col->down;
  node_col->down = new_node;
  return 1;
}

/* 用于矩阵转置，返回值为转置之后的矩阵的总头节点的指针 */
Matrix_Node *matrix_transpose(Matrix_Node *old_matrix){
  int new_row = old_matrix->col;
  int new_col = old_matrix->row;
  Matrix_Node *new_matrix;
  new_matrix = creat_matrix(new_row, new_col);
  int row_i = 1;
  Matrix_Node *p;     /* 用于指向老矩阵 */
  Matrix_Node *q;     /* 用于指向新矩阵 */
  while (row_i <= old_matrix->row) {
    p = &old_matrix[row_i];
    p = p->right;
    while (p != NULL) {
      /* 将旧矩阵中的数据的行列位置换一下，插入新矩阵 */
      insert_data(new_matrix, p->col, p->row, p->value);
      p = p->right;
    }
    row_i++;
  }
  return new_matrix;
}

/* 用于矩阵相乘，返回值为相乘之后的矩阵的总头节点的指针 */
Matrix_Node *matrix_multiply(Matrix_Node *matrix1, Matrix_Node *matrix2){
  if ((matrix1->row != matrix2->col) || (matrix1->col != matrix2->row)) {       /* 判断两矩阵的行列数是否相等 */
    puts("两矩阵行列数不匹配，无法相乘！\n");
    return 0;
  }
  Matrix_Node *multi_result;
  multi_result = creat_matrix(matrix1->row, matrix2->col);
  Matrix_Node *p;   /* 代表矩阵1的节点 */
  Matrix_Node *q;   /* 代表矩阵2的节点 */
  int row_i = 1;    /* 用来指示矩阵1的行 */
  int col_i = 1;    /* 用来指示矩阵1的列 */
  double temp = 0;     /* 用来临时存储相乘的值 */
  while (row_i <= matrix1->row) {         /* 遍历第一个矩阵的每一行 */
    p = &matrix1[row_i];
    q = &matrix2[col_i];
    p = p->right;
    q = q->down;
    while (col_i <= matrix2->col) {           /* 遍历第二个矩阵的每一列 */
        while (p != NULL && q != NULL) {
          if (p->col == q->row) {
            temp = p->value * q->value + temp;      /* 遇到行列对应的则相乘再相加 */
            p = p->right;
            q = q->down;
          }
          else if (p->col < q->row) {
            p = p->right;
          }
          else {
            q = q->down;
          }
        }
        if (temp != 0) {
          insert_data(multi_result, row_i, col_i, temp);      /* 将结果插入到新矩阵 */
        }
        col_i++;
        p = &matrix1[row_i];
        q = &matrix2[col_i];
        temp = 0;
    }
    row_i++;
    col_i = 1;
  }
  return multi_result;
}

/* 用于矩阵相加，返回值为相加之后的矩阵的总头节点的指针 */
Matrix_Node *matrix_add(Matrix_Node *matrix1, Matrix_Node *matrix2){
  if ((matrix1->col != matrix2->col) || (matrix1->row != matrix2->row) ) {
    puts("两矩阵行列数不匹配，不能相加！\n");
    return 0;
  }
  Matrix_Node *add_result;    /* 相加之后的矩阵 */
  add_result = creat_matrix(matrix1->row, matrix1->col);
  Matrix_Node *p;       /* 用来遍历矩阵１ */
  Matrix_Node *q;       /* 用来遍历矩阵２ */
  int row_i = 1;
  while (row_i <= matrix1->row) {
    p = &matrix1[row_i];
    q = &matrix2[row_i];
    p = p->right;
    q = q->right;
    while (p != NULL && q != NULL) {      /* 遍历两表的该行 */
      if (p->col == q->col) {       /* 如果同一个位置两个矩阵都有值，则相加 */
        if (p->value + q->value != 0) {       /* 判断相加结果是或否等于0,不等于0则插入 */
          insert_data(add_result, p->row, p->col, p->value + q->value);
        }
        p = p->right;
        q = q->right;
      }
      else if (p->col < q->col) {
        insert_data(add_result, p->row, p->col, p->value);
        p = p->right;
      }
      else {
        insert_data(add_result, q->row, q->col, q->value);
        q = q->right;
      }
    }
    while (p != NULL) {         /* 矩阵2的行已遍历完，但矩阵1还没遍历完 */
      insert_data(add_result, p->row, p->col, p->value);
      p = p->right;
    }
    while (q != NULL) {         /* 矩阵1已便利完，但矩阵1还没遍历完 */
      insert_data(add_result, q->row, q->col, q->value);
      q = q->right;
    }
    row_i++;
  }
  return add_result;
}

/* 用于打印矩阵 */
int print_matrix(Matrix_Node *matrix){
  if (matrix->row <= 0 || matrix->col <= 0) {
    puts("矩阵输入错误！\n");
    return 0;
  }
  int row_i = 1;
  int col_i = 1;
  Matrix_Node *p;
  while (row_i <= matrix->row) {
    p = &matrix[row_i];
    p = p->right;
    while (col_i <= matrix->col) {
      if (p != NULL) {
        if (col_i == p->col) {
          printf("%.3f\t", p->value);
          p = p->right;
        }
        else {
          printf("%d\t", 0);
        }
      }
      else {
        printf("%d\t", 0);
      }
      col_i++;
    }
    printf("\n");
    row_i++;
    col_i = 1;
  }
  return 1;
}

int main() {
  Matrix_Node *test;
  assert(creat_matrix(4,4));    /* 测试创建函数的矩阵 */
  puts("创建矩阵函数测试通过");
  test = creat_matrix(4,4);
  assert(insert_data(test, 1, 2, 5.1));
  assert(insert_data(test, 3, 2, 8.9));
  assert(insert_data(test, 2, 3, 9));
  puts("插入元素函数测试通过");
  puts("测试打印test1矩阵");
  assert(print_matrix(test));
  puts("打印矩阵函数测试通过，上述矩阵为test1");
  Matrix_Node *test2;
  assert(matrix_transpose(test));
  puts("矩阵转置函数测试通过");
  test2 = matrix_transpose(test);
  puts("转置后矩阵test2为：");
  print_matrix(test2);
  Matrix_Node *multi_matrix;
  assert(matrix_multiply(test, test2));
  puts("矩阵相乘函数测试通过");
  multi_matrix = matrix_multiply(test, test2);
  puts("test1*test2的结果为");
  print_matrix(multi_matrix);
  Matrix_Node *add_matrix;
  assert(matrix_add(test, test2));
  puts("矩阵相加函数测试通过");
  add_matrix = matrix_add(test, test2);
  puts("test1+test2的结果为");
  print_matrix(add_matrix);
  return 0;
}
