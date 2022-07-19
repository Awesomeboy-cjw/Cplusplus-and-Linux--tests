#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

//创建两个管道用于双端通信
static int piped[2];
static int pipef[2];

//设置为非阻塞
void setnonblock(int fd)
{
    int old = fcntl(fd, F_GETFL);
    old |= O_NONBLOCK;
    fcntl(fd, F_SETFL, old);
}

int main()
{
    int ret = pipe(piped);
    setnonblock(piped[0]);
    setnonblock(piped[1]);
    if(ret == -1)
    {
        printf("create normal pipe failed\n");
        exit(1);
    }
    ret = pipe(pipef);
    setnonblock(pipef[0]);
    setnonblock(pipef[1]);
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
        //父进程中关闭piped中的写，通过pipef写
        //关闭pipef中的读，通过piped读，
        close(pipef[0]);
        close(piped[1]);
        ret = write(pipef[1], "i am father", 10);
        if(ret == -1) printf("father write error\n");
        
        sleep(1);
        char buf[11] = {'\0'};
        ret = read(piped[0], buf, 10);
        if(ret == -1) printf("father read error\n");
        else printf("in father %s\n", buf);
    }
    else
    {
        //子进程相反，通过piped写，通过pipef读
        close(pipef[1]);
        close(piped[0]);
        ret = write(piped[1], "i am child", 10);
        if(ret == -1) printf("child write error\n");
        sleep(1);

        char buf[11] = {'\0'};
        ret = read(pipef[0], buf, 10);
        if(ret == -1) printf("child write error\n");
        else printf("in child %s\n", buf);
    }
    return 0;
}
/*
  输出是下面两行，符合预期
  in father i am child
  in child i am fathe
*/
