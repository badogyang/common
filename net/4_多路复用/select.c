#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int iofd = 0;
    char s[] = "D.T.software\n";
    int len = 0;

    fd_set reads = {0};
    fd_set temps = {0};
    struct timeval timeout = {0};

    FD_ZERO(&reads);
    FD_SET(iofd, &reads);

    while (1)
    {
        int r = 1; 

        temps = reads;

        timeout.tv_sec = 0;
        timeout.tv_usec = 50000;

        r = select(1, &temps, 0, 0, &timeout);

        if(r > 0)
        {
            len = read(iofd, s, sizeof(s)-1);
            s[len] = 0;

            printf("Input: %s\n", s);
        } 
        else if (r == 0)
        {
            static int count = 0;
            usleep(10000);
            count++;

            if(count > 100)
            {
                printf("do something else\n");

                count = 0;
            }
        }
        else 
        {
            break;
        }
    }
    

    write(0, s, sizeof(s));    //向标准输入输出写入

    len = read(0, s, 5);       //向标准输入输出写入

    s[len] = 0;

    printf("%s\n", s);
}