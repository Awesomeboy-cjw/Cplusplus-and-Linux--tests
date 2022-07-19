#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

//创建双端管道用于双端通信
static int piped[2];

//设置为非阻塞
void setnonblock(int fd)
{
    int old = fcntl(fd, F_GETFL);
    old |= O_NONBLOCK;
    fcntl(fd, F_SETFL, old);
}

int main()
{
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, piped);
    setnonblock(piped[0]);
    setnonblock(piped[1]);
    if(ret == -1)
    {
        printf("create normal pipe failed\n");
        exit(1);
    }
    
    int pid = fork();
    if(pid < 0)
    {
        printf("fork failed\n");
        exit(1);
    }
    else if(pid > 0)
    {
        //父进程中关闭pipe[1]，通过pipe[0]读取
        //子进程数据，或者给子进程发数据
        close(piped[1]);
        ret = write(piped[0], "i am father", 10);
        if(ret == -1) printf("father write error\n");
        
        sleep(1);
        char buf[11] = {'\0'};
        ret = read(piped[0], buf, 10);
        if(ret == -1) printf("father read error\n");
        else printf("in father %s\n", buf);
    }
    else
    {
        //子进程相反，通过piped[1]读写数据
        close(piped[0]);
        ret = write(piped[1], "i am child", 10);
        if(ret == -1) printf("child write error\n");
        sleep(1);

        char buf[11] = {'\0'};
        ret = read(piped[1], buf, 10);
        if(ret == -1) printf("child write error\n");
        else printf("in child %s\n", buf);
    }
    return 0;
}
/*
    输出如下，和pipe.cpp功能一样
    可见socketpair()函数的原理
    in father i am child
    in child i am fathe
*/
