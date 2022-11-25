
#include <stdio.h>
int main()
{
    int i = 1;
    char *a = (char *)&i;
    if(*a == 1)
        printf("小端\n");
    else
        printf("大端\n");
    return 0;
}

