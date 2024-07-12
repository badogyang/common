
#ifndef HCI_H_H_H
#define HCI_H_H_H

#define UINT8_TO_STREAM(p,u8) {*(p)++ = (uint8_t)(u8);}
#define UINT16_TO_STREAM(p,u16) {*(p)++ = (uint8_t)(u16);*(p)++ = (uint8_t)((u16) >> 8);}

#define STREAM_TO_UINT8(u8,p) {u8 = (uint8_t )(*(p));(p)+=1;}
#define STREAM_TO_UINT16(u16,p) {u16 = ((uint16_t)(*(p))) + ((uint16_t)(*((p)+1))<<8);(p) += 2;}

#define HCI_OGF_CONTROLLER_BB (0x03 << 10)
#define HCI_OGF_VENDOR_SPEC (0x3f << 10)

#define HCI_RESET_OP (0x0003 | HCI_OGF_CONTROLLER_BB)
#define HCI_WRITE_SCAN_ENABLE_OP (0x001A | HCI_OGF_CONTROLLER_BB)
#define HCI_VENDOR_OP (0x0000 | HCI_OGF_VENDOR_SPEC)

#define HCI_EVT_CMD_CMPL 0x0e
#define HCI_EVT_VS_EVT 0xff


typedef enum
{
	BT_H4_TYPE_CMD = 0x01,
	BT_H4_TYPE_ACL = 0x02,
	BT_H4_TYPE_SCO = 0x03,
	BT_H4_TYPE_EVT = 0x04,
	BT_H4_TYPE_ISO = 0x04,
}bt_h4_type_t;

typedef enum
{
	BT_H4_W4_TRANSPORT_TYPE,
	BT_H4_W4_EVT_HDR,
	BT_H4_W4_ACL_HDR,
	BT_H4_W4_EVT_PARAM,
	BT_H4_W4_ACL_PARAM,
}bt_h4_read_status_t;

void bt_hci_reset();
void bt_hci_write_scan_enble();
void bt_hci_vendor(uint8_t *data,uint16_t data_len);

#endif