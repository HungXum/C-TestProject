/**
 * atexit函数测试:main结束以后才被调用的
 */
#include <stdlib.h>
#include <vector>

void AtexitPrintf() {
    printf("process terminate\n");
    return;
}

void AtexitTest() {
    atexit(AtexitPrintf);
    //段错误并不会执行atexit函数,只有在正常terminate才会执行if
    std::vector<int> v;
    printf("v = %d\n", v[100]);
}