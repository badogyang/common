#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


int main()
{
    unsigned int addr = inet_addr("10.23.44.127");  //填写IP地址
    struct in_addr addr1 = {0x09080706};
    struct in_addr addr2 = {0x05040302};

    char* s1 = inet_ntoa(addr1);
    char* s2 = inet_ntoa(addr2);

    printf("addr = %x\n", addr);

    return 0;

}

