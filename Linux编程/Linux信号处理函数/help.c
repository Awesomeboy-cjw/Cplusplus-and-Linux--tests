#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main()
{
    int n;
    scanf("%d",&n);
    //给sighandle.cpp运行起来的进程发送SIGHUP信号
    kill(n, SIGHUP);
    return 0;
}
