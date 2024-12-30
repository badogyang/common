#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "uart.h"
#include "btsnoop.h"
#include "bt_timer.h"
#include "hci.h"

uint16_t read_pos = 0;
bt_h4_read_status_t h4_read_status = BT_H4_W4_TRANSPORT_TYPE;
uint8_t bt_rx_buffer[1024] = {0};

uint16_t event_param_len;
uint16_t acl_param_len;


uint16_t csr8x11_initscript_wp = 0;
uint8_t csr8x11_initscript[] =
{
    //  Set PSKEY_DEEP_SLEEP_STATE never deep sleep
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x02, 0x00, 0x03, 0x70, 0x00, 0x00, 0x29, 0x02, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00,

    //  Set ANA_Freq to 26MHz
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x03, 0x00, 0x03, 0x70, 0x00, 0x00, 0xfe, 0x01, 0x01, 0x00, 0x08, 0x00, 0x90, 0x65,

    //  Set CSR_PSKEY_ANA_FTRIM 0x24 for csr8811
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x04, 0x00, 0x03, 0x70, 0x00, 0x00, 0xf6, 0x01, 0x01, 0x00, 0x08, 0x00, 0x24, 0x00,

    // Set CSR_PSKEY_DEFAULT_TRANSMIT_POWER 0x4
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x05, 0x00, 0x03, 0x70, 0x00, 0x00, 0x21, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00,

    // Set CSR_PSKEY_MAXIMUM_TRANSMIT_POWER 0x4
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x06, 0x00, 0x03, 0x70, 0x00, 0x00, 0x17, 0x00, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00,

    // Set CSR_PSKEY_BLE_DEFAULT_TRANSMIT_POWER 0x4
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x07, 0x00, 0x03, 0x70, 0x00, 0x00, 0xc8, 0x22, 0x01, 0x00, 0x08, 0x00, 0x04, 0x00,

    // Set CSR_PSKEY_BDADDR
    0x19, 0xc2, 0x02, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x03, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x08, 0x00, 0x20, 0x00, 0x98, 0x1a, 0x86, 0x00, 0x1d, 0x00,

    // Set CSR_PSKEY_PCM_CONFIG32
    0x15, 0xc2, 0x02, 0x00, 0x0a, 0x00, 0x09, 0x00, 0x03, 0x70, 0x00, 0x00, 0xb3, 0x01, 0x02, 0x00, 0x08, 0x00, 0x80, 0x08, 0x80, 0x18,

    // Set CSR_PSKEY_PCM_FORMAT 0x60
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x03, 0x70, 0x00, 0x00, 0xb6, 0x01, 0x01, 0x00, 0x08, 0x00, 0x60, 0x00,

    // Set CSR_PSKEY_USER_LOW_JITTER_MODE
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0b, 0x00, 0x03, 0x70, 0x00, 0x00, 0xc9, 0x23, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00,

    //  Set HCI_NOP_DISABLE
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0c, 0x00, 0x03, 0x70, 0x00, 0x00, 0xf2, 0x00, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00,

    // Set UART baudrate to 921600
    0x15, 0xc2, 0x02, 0x00, 0x0a, 0x00, 0x0d, 0x00, 0x03, 0x70, 0x00, 0x00, 0xea, 0x01, 0x02, 0x00, 0x08, 0x00,0x0e,0x00,0x00,0x10,/*0x1b, 0x00, 0x40, 0x77,*/

    //  WarmReset
    0x13, 0xc2, 0x02, 0x00, 0x09, 0x00, 0x0e, 0x00, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


void bt_evt_data_process(uint8_t *data,uint16_t data_len)
{
	printf("bt_evt_data_process\r\n");
	printf("-------------------------------------\r\n");
	for(int index = 0; index < data_len; index++)
		printf("0x%02x ",data[index]);
	printf("\r\n-------------------------------------\r\n");

	uint8_t *evt_buffer = data;

	uint8_t type;
	uint8_t evt_code;
	uint8_t param_len;

	STREAM_TO_UINT8(type,evt_buffer);
	STREAM_TO_UINT8(evt_code,evt_buffer);
	STREAM_TO_UINT8(param_len,evt_buffer);
	printf("type:%d evt_code:%d param_len:%d\r\n",type,evt_code,param_len);

	switch(evt_code)
	{
		case HCI_EVT_CMD_CMPL:
		{
			uint8_t num_packet;
			uint16_t opcode;
			uint8_t status;
			STREAM_TO_UINT8(num_packet,evt_buffer);
			STREAM_TO_UINT16(opcode,evt_buffer);
			STREAM_TO_UINT8(status,evt_buffer);

			switch(opcode)
			{
				case HCI_RESET_OP:
					//utimer_cancel(bt_hci_reset_timer);

					bt_hci_vendor(csr8x11_initscript,csr8x11_initscript[0]+1);
					csr8x11_initscript_wp += csr8x11_initscript[0]+1;
					break;
			}
			break;
		}
		case HCI_EVT_VS_EVT:
		{
			uint16_t write_len = *(csr8x11_initscript+csr8x11_initscript_wp)+1;
			bt_hci_vendor(csr8x11_initscript+csr8x11_initscript_wp,write_len);
			csr8x11_initscript_wp += write_len;

			if(csr8x11_initscript_wp == sizeof(csr8x11_initscript))
				printf("download bccmd done\r\n");
			break;
		}
		default:
			break;
	}
	

	btsnoop_write(TRANSPORT_TYPE_EVT,1,data,data_len);
}

#define TIMER_TICK_PER_SECOND 100
void *timer_source_thread(void * param)
{
	printf("timer_source_thread\r\n");
	while(1)
	{
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000/TIMER_TICK_PER_SECOND*1000;
		select(0, NULL, NULL, NULL, &timeout);

		utimer_polling();
	}
}

int main()
{
	pthread_t thread_timer_id; 
	utimer_init();
	pthread_create(&thread_timer_id,NULL,timer_source_thread,NULL);
	btsnoop_open("./btsnoop.log");
	uart_bt_open();

	bt_hci_reset();

	while(1)
	{
		switch(h4_read_status)
		{
			case BT_H4_W4_TRANSPORT_TYPE:
				uart_bt_read(bt_rx_buffer,1);
				if(bt_rx_buffer[0] == BT_H4_TYPE_EVT)
					h4_read_status = BT_H4_W4_EVT_HDR;
				else if(bt_rx_buffer[0] == BT_H4_TYPE_ACL)
					h4_read_status = BT_H4_W4_ACL_HDR;

				read_pos = 1;
				break;
			case BT_H4_W4_EVT_HDR:
				uart_bt_read(bt_rx_buffer+read_pos,2);
				printf("event code:0x%x len:%d\r\n",bt_rx_buffer[read_pos],bt_rx_buffer[read_pos+1]);
				event_param_len = bt_rx_buffer[read_pos+1];
				h4_read_status = BT_H4_W4_EVT_PARAM;
				read_pos += 2;
	
				break;
			case BT_H4_W4_ACL_HDR:
				uart_bt_read(bt_rx_buffer+read_pos,4);

				h4_read_status = BT_H4_W4_ACL_PARAM;
				read_pos += 4;
				break;
			case BT_H4_W4_EVT_PARAM:
				uart_bt_read(bt_rx_buffer+read_pos,event_param_len);
				/* Event process */
				bt_evt_data_process(bt_rx_buffer,read_pos+event_param_len);
				
				read_pos = 0;
				h4_read_status = BT_H4_W4_TRANSPORT_TYPE;
				break;
			case BT_H4_W4_ACL_PARAM:
				break;
			default:
				break;
		}
	}

	uart_bt_close();
	btsnoop_close();

    return 0;
}
