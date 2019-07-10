/**
 * 学习系统的信号量
 */

#include <signal.h>
#include <stdio.h>

int level = 1;
/**
 * @param signum [系统传递的信号量]
 */
void SignaleHandler(int signum) {
    printf("signum = %d\n", signum);
    switch(signum) {
        case SIGINT:
            //不加exit(0)会阻塞掉,程序不会结束
            exit(0);
            break;
        case SIGKILL:
            printf("signal kill\n");
            break;
        case SIGUSR1:
        {
            ++level;
            printf("level = %d\n", level);
            break;
        }
        case SIGSEGV:
        {
            printf("segment fault!\n");
        }
            break;
    }
}

void SignalTest() {
    //监听ctrl+C信号
    signal(SIGINT, SignaleHandler);
    //监听killall -USR1信号
    signal(SIGUSR1, SignaleHandler);
    //SIGKILL信号不能阻塞,系统会直接杀掉
    signal(SIGKILL, SignaleHandler);
    while(1) {
    }
}