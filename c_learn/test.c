#include <stdio.h>

int main()
{
    char str[] = "hello";
    char* p = str;

    printf("str size = %d\n", sizeof(str));
    printf("p size = %d\n", sizeof(*p));

    return 0;
}