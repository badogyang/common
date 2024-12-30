#include <stdio.h>
#include <unistd.h>

int main()
{
    int iofd = 0;
    char str[] = "hello DT\n";
    int len = 0;
    
    write(iofd, str, sizeof(str));

    len = read(iofd, str, 5);

    str[len] = 0;

    printf("%s\n", str);

    return 0;
}
