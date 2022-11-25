
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#include "btsnoop.h"

int btsnoop_fd = -1;

void be_store_32(uint8_t* buffer, uint32_t value)
{
    uint8_t index = 0;
    buffer[index++] = (uint8_t)(value >> 24);
    buffer[index++] = (uint8_t)(value >> 16);
    buffer[index++] = (uint8_t)(value >> 8);
    buffer[index++] = (uint8_t)(value);
}

void be_store_64(uint8_t* buffer, uint64_t value)
{
    uint8_t index = 0;
    buffer[index++] = (uint8_t)(value >> 56);
    buffer[index++] = (uint8_t)(value >> 48);
    buffer[index++] = (uint8_t)(value >> 40);
    buffer[index++] = (uint8_t)(value >> 32);
    buffer[index++] = (uint8_t)(value >> 24);
    buffer[index++] = (uint8_t)(value >> 16);
    buffer[index++] = (uint8_t)(value >> 8);
    buffer[index++] = (uint8_t)(value);
}

uint8_t btsnoop_open(uint8_t* file_name)
{
    if( !file_name ) 
    {
        printf("!file name \n");
        return 1;
    }

    btsnoop_fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY);

    if( btsnoop_fd == -1 )
    {
        printf("btsnoop_fd == -1\n");
        return 1;
    }

    write(btsnoop_fd, "btsnoop\0\0\0\0\1\0\0\x3\xea", 16);
}

uint8_t btsnoop_close(void)
{
    if( btsnoop_fd != -1 )
    {
        close(btsnoop_fd);
        btsnoop_fd = -1;
    }
}

uint8_t btsnoop_write(uint8_t type, uint8_t in, uint8_t *data, uint16_t data_len)
{
    uint32_t original_length;
    uint32_t inclueded_length;
    uint32_t packet_flags;
    uint32_t cumulative_drops;
    uint64_t timestamp;

    if( type == TRANSPORT_TYPE_CMD )
    {
        packet_flags = 2;
    } 
    else if( type == TRANSPORT_TYPE_ACL )
    {
        packet_flags = in;
    }
    else if( type == TRANSPORT_TYPE_EVT)
    {
        packet_flags = 3;
    }

    struct timeval curr_time;

    gettimeofday(&curr_time, NULL);

    be_store_32((uint8_t *)&original_length, data_len);
    be_store_32((uint8_t *)&inclueded_length, data_len);
    be_store_32((uint8_t *)&packet_flags, packet_flags);
    be_store_32((uint8_t *)&cumulative_drops, 0);
    be_store_64((uint8_t *)&timestamp, curr_time.tv_usec);

    write(btsnoop_fd, &original_length, sizeof(original_length));
    write(btsnoop_fd, &inclueded_length, sizeof(inclueded_length));
    write(btsnoop_fd, &packet_flags, sizeof(packet_flags));
    write(btsnoop_fd, &cumulative_drops, sizeof(cumulative_drops));
    write(btsnoop_fd, &timestamp, sizeof(timestamp));

    write(btsnoop_fd, &data, data_len);
}
