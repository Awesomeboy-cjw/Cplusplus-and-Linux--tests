#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void handle(int sig)
{
    printf("sig handle for %d\n", sig);
    int n = 5;
    while(n--)
    {
        printf("sleep one second\n");
        sleep(1);
    }
    //exit(0);
}


int main()
{
    struct sigaction newact;
    newact.sa_handler = handle;
    sigemptyset(&newact.sa_mask);
    sigaddset(&newact.sa_mask, SIGINT); //屏蔽了SIGINT信号
    if(sigaction(SIGHUP, &newact, NULL) < 0)
        perror("");
    printf("pid is %d\n", getpid());
    sleep(10);
    sleep(10);
}

//运行这个程序，同时通过help.cpp编译的程序给此程序传递SIGHUP信号，进入
//信号处理函数后，此时我们通过终端输入SIGINT信号(ctrl+c)发现对SINGINT
//的处理暂时搁置，直到处理完SIGHUP信号
