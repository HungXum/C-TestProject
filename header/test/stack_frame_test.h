/**
 * 函数中栈,参数的入栈出栈顺序
 */

#include <stdio.h>

int PrintfOrder(int &x);
void PrintfOrder(int x, int y);

//测试输出的入栈过程
//根据网上的评论得出:编译器不同,无法确定函数参数栈的执行顺序,可能是从左到右,也可能是从右到左,故没有研究意义
//但是可以确定的是形参入栈的顺序是从右到左,并且stack的存储地址从低到高入栈,即最右边的参数位于stack的最低位置
void StackLayoutTest() {
    int i = 1;

    /**
     * 输出结果:2, 3, 3
     */
    // printf("%d, %d, %d\n", i++, ++i, i); 
    
    /**
     * 输出结果:1th calls printOrder!
     * 2th calls printOrder!
     * 3   2
     */
    // printf("%d\t%d\n", PrintfOrder(i), PrintfOrder(i));

    /**
     * 输出结果:
     * &x = 0x7ffc19d5d11c
     * &y = 0x7ffc19d5d118
     */
    int x = 4, y = 5;
    PrintfOrder(x, y);
}

int PrintfOrder(int &x) {
    printf("%dth calls printOrder!\n", x);
    return ++x;
}

void PrintfOrder(int x, int y) {
    printf("&x = %p\n", &x);
    printf("&y = %p\n", &y);

    int i = 23;
    char c = 'a';
    float f = 48.0;
    printf("&i = %p\n", &i);
    printf("&c = %p\n", &c);
    printf("&f = %p\n", &f);
}