#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "uart.h"
#include "hci.h"
#include "bt_timer.h"

uint8_t bt_tx_buffer[1024] = {0};


int32_t bt_hci_reset_timer;

void bt_hci_reset_timeout(void *para)
{
	printf("bt_hci_reset_timeout\r\n");
	bt_hci_reset();
}

void bt_hci_reset()
{
    uint8_t *tx_buffer = bt_tx_buffer;
    UINT8_TO_STREAM(tx_buffer, BT_H4_TYPE_CMD);
    UINT16_TO_STREAM(tx_buffer, HCI_RESET_OP);
    UINT8_TO_STREAM(tx_buffer,0);

    uart_bt_send(bt_tx_buffer, tx_buffer-bt_tx_buffer);

    bt_hci_reset_timer = utimer_create(10, bt_hci_reset_timeout, NULL);
}

void bt_hci_write_scan_enble()
{
    uint8_t *tx_buffer = bt_tx_buffer;
    UINT8_TO_STREAM(tx_buffer, BT_H4_TYPE_CMD);
    UINT16_TO_STREAM(tx_buffer, HCI_WRITE_SCAN_ENABLE_OP);
    UINT8_TO_STREAM(tx_buffer,1);
    UINT8_TO_STREAM(tx_buffer,3);

    uart_bt_send(bt_tx_buffer, tx_buffer-bt_tx_buffer);
}

void bt_hci_vendor(uint8_t *data,uint16_t data_len)
{
    uint8_t *tx_buffer = bt_tx_buffer;
    UINT8_TO_STREAM(tx_buffer, BT_H4_TYPE_CMD);
    UINT16_TO_STREAM(tx_buffer, HCI_VENDOR_OP);
    
    memcpy(tx_buffer,data,data_len);
	tx_buffer += data_len;

    uart_bt_send(bt_tx_buffer, tx_buffer-bt_tx_buffer);
}