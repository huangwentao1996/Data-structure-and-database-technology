#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

int check_expression_syntax(char *expression) {     /* 检查输入表达式是否合法 */
  int left_para = 0;    /* 用来计算左括号数量 */
  int right_para = 0;   /* 用来计算右括号数量 */
  int i = 0;
  while (expression[i] != '\0') {
    int a = expression[i];      /* 将字符转换为ASSIC II码 */
    if ( (a >= 40 && a <= 57 && a!= 44) || a == 37 || a == 94){      /* 判断输入的元素是否为规定的数字或操作符 */
      if ( a == 40 ){    /* 判断是否为左括号 */
        left_para++;
      }
      if ( a == 41 && right_para < left_para ){ /* 判断是否为右括号且之前是否出现过左括号 */
        right_para++;
      }
    }
    else {
      puts("输入的表达式不合法，只允许输入数字（0-9），小数点（.），以及运算符（+-*/^%），请不要输入空格以及其他字符！\n");
      return 0;
    }
    i++;
  };
  if (left_para == right_para) {
    return 1;
  }
  else {
    return 0;
  }
}

int priority(char operator) {   /* 返回操作符的优先级 */
  switch(operator) {
    case '^':
      return 4;
    case '*':
    case '/':
    case '%':
      return 3;
    case '+':
    case '-':
      return 2;
    case '#':
      return 1;
    }
    return -1;
}

double calculate(double val1, double val2, char op) {     /* 计算 */
  switch(op) {
    case '+':
      return(val1+val2);
    case '-':
      return(val1-val2);
    case '*':
      return(val1*val2);
    case '/':
      return(val1/val2);
    case '%':
      {
        int a,b;     /* 取模时要将数据类型转为int */
        a = val1;
        b = val2;
        return(a%b);
      }
    case '^':
      return(pow(val1,val2));
    }
    return -1;
}

char *location;         /* 用于记录括号递归时的指针位置 */

double operate(char *expression){
  double *num_stack = (double *)malloc(1000 * sizeof(double));   /* 算子栈 */
  char *ope_stack = (char *)malloc(1000 * sizeof(char));     /* 算符栈 */
  double num1, num2;      /* 预留两个变量用来储存出栈数 */
  double temp;    /* 用于将字符串转换为小数 */
  *ope_stack = '#';     /* #作为算符栈底元素 */
  while (*expression != '\0') {     /* 判断是否读取到了字符串末尾 */
      if (*expression == '(') {     /* 判断是否为左括号 */
        expression++;
        double a;
        a = operate(expression);    /* 计算括号内的结果,并将其压入栈中 */
        num_stack++;
        *num_stack = a;
        expression = location;
      }
      else if (*expression == ')') {     /* 判断是否为右括号 */
        expression++;
        location = expression;
        break;      /* 当为右括号时，跳出循环，开始计算 */
      }
      else {
        if (('0' <= *expression) && (*expression <= '9')) {      /* 判断是否为数字 */
          temp = ((double)(*expression - '0'));
          expression++;
          while ( (('0' <= *expression)&&(*expression <= '9')) || (*expression == '.')) {
            if ( *expression == '.') {    /* 判断是否为小数点 */
              int i = -1;   /* 记录小数点的位数 */
              expression++;
              while (('0' <= *expression) && (*expression <= '9')) {
                temp = temp + ((double)(*expression-'0')) * pow(10, i);
                expression++;
                i--;
              }
            }
            if (('0' <= *expression) && (*expression <= '9')) {  /* 判断是否为数字 */
              temp = temp * 10 + ((double)(*expression-'0'));
              expression++;
            }
          }
          num_stack++;
          *num_stack = temp;   /* 将数字压入算子栈 */
        }
        else {    /* 当字符为操作符时 */
          if ( priority(*expression) > priority(*ope_stack)) {      /* 比较当前操作符与栈内操作符的优先级 */
            ope_stack++;
            *ope_stack = *expression;     /* 将操作符压入算符栈顶 */
            expression++;
          }
          else {    /* 栈顶操作符优先级更高 */
            num2 = *num_stack;
            num_stack--;
            num1 = *num_stack;
            double result;
            result = calculate(num1, num2, *ope_stack);
            *num_stack = result;
            ope_stack--;
          }
        }
      }
  }
  while (*ope_stack != '#') {       /* 弹出算子和算符中的元素进行计算 */
    num2 = *num_stack;
    num_stack--;
    num1 = *num_stack;
    double result;
    result = calculate(num1, num2, *ope_stack);
    *num_stack = result;
    ope_stack--;
  }
  return *num_stack;
}

int debug(){
  char string[1000] = "3+4+(4*3-5/6)-2";
  char *test_string;
  test_string = string;
  assert(check_expression_syntax(test_string));
  assert(priority('+'));
  assert(calculate(3,4,'+'));
  assert(operate(test_string));
  return 1;
}

int main() {
  int a;
  a = debug();
  if ( a = 1) {
    puts("函数自检通过");
  }
  char *expression;
  expression = (char *)malloc(1000 * sizeof(char));
  puts("\n欢迎使用科学计算器，本计算器支持的运算有：加减乘除，取余（%），乘方（^），支持小数运算，不支持负数运算！\n输入示例: 3.1*4+(8/5)\n");
  puts("请输入你要计算的表达式:");
  fgets(expression, 1000, stdin);
  expression[strlen(expression)-1] = '\0';
  if (check_expression_syntax(expression)) {
    printf("%s\n%s = %.3f\n", "计算完成", expression, operate(expression));
  }
  return 0;
}
