#include "btsnoop.h"

int main()
{
    printf("%d\n", btsnoop_open("./hci_btsnoop.log"));
    //btsnoop_open("./hci_btsnoop.log");
    
    uint8_t hci_reset[] = {0x01, 0x03, 0x0C, 0x00};
    //uint8_t hci_reset_evt[] = {0x04 ,0x0E ,0x04 ,0x01 ,0x03 ,0x0C ,0x00};
    
    btsnoop_write(TRANSPORT_TYPE_CMD, 0, hci_reset, sizeof(hci_reset));
    //btsnoop_write(TRANSPORT_TYPE_EVT, 1, hci_reset_evt, sizeof(hci_reset_evt));

    sleep(1);

    btsnoop_close();
    return 0;
}
