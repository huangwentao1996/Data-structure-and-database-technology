#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

typedef struct _TreeNode {
	int type;/*数字是0，单目运算符是1，双目运算符是2*/
	double value;
	char operator;
	struct _TreeNode *lchild, *rchild;
} TreeNode;

//用于判断运算符的优先级
int priority(char *p){
	switch (*p) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
		case '%':
			return 2;
		case '^':
			return 3;
	}
	return 0;
}

TreeNode *create_tree(char *start, char *end){
	char *p, *q;	//分别代表头指针和尾指针
	int i = 0;
	//分别用来统计符号的数量，左右括号的数量之差，优先级为1,2,3的符号数量
	int count_ope = 0, count_para = 0, count_1 = 0, count_2 = 0, count_3 = 0;
	//分别用来指示最后一个优先级为1,2,3的符号的位置
	int p1 = 0,p2 = 0,p3 = 0;
	TreeNode *tree;
	if(*start == '(' && *end == ')'){
		return create_tree(start+1, end-1);	//脱括号
	}
	for(p = start; p != (end+1); p++){
			//判断是否是符号
	    if (!(*p <= 57 && *p >= 48 || *p == '.')) count_ope++;
	    if (*p == '(') count_para++;
      if (*p == ')') count_para--;
			//判断括号外的符号的优先级和位置
			if (count_para == 0) {
				if(priority(p) == 1){
					p1 = i;
					count_1++;
				}
				else if(priority(p) == 2){
					p2 = i;
					count_2++;
				}
				else if(priority(p) == 3){
					p3 = i;
					count_3++;
				}
			}
			i++;
	}
	if (count_para != 0) {
		puts("左右括号数量不匹配，请检查表达式");
		return 0;
	}
	//检测是否有sin,cos,cotan,cos,exp函数的存在
	else if(*start == 't' || *start == 'c' || *start == 's' || *start == 'e' && *end == ')'){
		if (*(start+1) == 'o') {
			tree = (TreeNode *)malloc(sizeof(TreeNode));
			tree->rchild = NULL;
			tree->type = 1;
			tree->operator = 'o';		//代表cotan;
			tree->lchild = create_tree(start + 6,end - 1);
			return tree;
		}
		tree = (TreeNode *)malloc(sizeof(TreeNode));
		tree->rchild = NULL;
		tree->type = 1;
		tree->operator = *start;
		tree->lchild = create_tree(start+4,end-1);
		return tree;
	}
	if(count_ope == 0){
		tree = (TreeNode *)malloc(sizeof(TreeNode));
		tree->lchild = tree->rchild = NULL;
		tree->type = 0;
		tree->value = atof(start);
		return tree;
	}
	else if(count_ope == 1 && *start == '-'){
		tree = (TreeNode *)malloc(sizeof(TreeNode));
		tree->lchild = tree->rchild = NULL;
		tree->type = 0;
		tree->value = (-1)*atof(start+1);
		return tree;
	}
	else {
		//定位分界点
  	if(count_1 != 0) q = start + p1;
  	else if(count_2 != 0) q = start + p2;
  	else if(count_3 != 0) q = start + p3;
		tree = (TreeNode *)malloc(sizeof(TreeNode));
		tree->type = 2;
		tree->operator = *q;
		tree->lchild = create_tree(start, q-1);
		tree->rchild = create_tree(q+1, end);
		return tree;
	}
}

//用于计算二叉树
double calculate(TreeNode *p){
	if(p->type == 0) {
		return p->value;
	}
	else if(p->type==1){
		switch (p->operator) {
			case 't':
				return tan(calculate(p->lchild));
			case 's':
				return sin(calculate(p->lchild));
			case 'c':
				return cos(calculate(p->lchild));
			case 'o':
				return 1 / tan(calculate(p->lchild));
			case 'e':
				return exp(calculate(p->lchild));
		}
	}
	else if(p->type==2){
		switch (p->operator) {
			case '+':
				return calculate(p->lchild) + calculate(p->rchild);
			case '-':
				return calculate(p->lchild) - calculate(p->rchild);
			case '/':
				return calculate(p->lchild) / calculate(p->rchild);
			case '*':
				return calculate(p->lchild) * calculate(p->rchild);
			case '^':
				return pow(calculate(p->lchild), calculate(p->rchild));
			case '%':
			{
				int a,b;     /* 取模时要将double类型转为int */
				a = calculate(p->lchild);
				b = calculate(p->rchild);
				return  a % b;
			}
		}
	}
}

int main(){
	TreeNode *tree;
	char *p, *q;
	char expression[50] = "3+exp(2)*2-(-4)/2+4%2+tan(0.8)";
	p = expression;
	q = &expression[strlen(expression)-1];
	printf("测试表达式为:%s\n", p);
	assert(tree = create_tree(expression, q));
	printf("表达式结果是:%.3f\n", calculate(tree));
}
