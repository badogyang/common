## rtk_hciattach -n -s 115200 ttyS1 rtk_h5 执行log

```
root@rockchip:/# rtk_hciattach -n -s 115200 ttyS1 rtk_h5 &
[1] 1402
root@rockchip:/# Realtek Bluetooth :Realtek Bluetooth init uart with init speed:115200, type:HCI UART H5
Realtek Bluetooth :Realtek hciattach version 3.1.9495cb2.20230110-195934

Realtek Bluetooth :Use epoll
Realtek Bluetooth :[SYNC] Get SYNC Resp Pkt
Realtek Bluetooth :[CONFIG] Get SYNC pkt
Realtek Bluetooth :[CONFIG] Get CONFG pkt
Realtek Bluetooth :[CONFIG] Get CONFG resp pkt
Realtek Bluetooth :dic is 1, cfg field 0x14
Realtek Bluetooth :H5 init finished

Realtek Bluetooth :Realtek H5 IC
Realtek Bluetooth :Receive cmd complete event of command: fc61
Realtek Bluetooth :LMP Subversion 0x8822
Realtek Bluetooth :Receive cmd complete event of command: fc61
Realtek Bluetooth :HCI Revision 0x000e
Realtek Bluetooth :Receive cmd complete event of command: fc6d
Realtek Bluetooth :Read ROM version 00
Realtek Bluetooth :LMP Subversion 0x8822
Realtek Bluetooth :EVersion 0
Realtek Bluetooth :IC: RTL8822ES
Realtek Bluetooth :Firmware/config: rtl8822es_fw, rtl8822es_config
Realtek Bluetooth :Couldnt open extra config /opt/rtk_btconfig.txt, No such file or directory
Realtek Bluetooth :Couldnt open BT MAC file /opt/bdaddr, No such file or directory
Realtek Bluetooth :Origin cfg len 33
Realtek Bluetooth :55 ab 23 87 1b 00 0c 00 10 02 80 92 04 50 c5 ea
Realtek Bluetooth :19 e1 1b fd af 5f 01 a4 0b 8d 00 01 fa 8f 00 01
Realtek Bluetooth :bf
Realtek Bluetooth :Config baudrate: 04928002
Realtek Bluetooth :uart flow ctrl: 1
Realtek Bluetooth :Vendor baud from Config file: 04928002
Realtek Bluetooth :New cfg len 33
Realtek Bluetooth :55 ab 23 87 1b 00 0c 00 10 02 80 92 04 50 c5 ea
Realtek Bluetooth :19 e1 1b fd af 5f 01 a4 0b 8d 00 01 fa 8f 00 01
Realtek Bluetooth :bf
Realtek Bluetooth :Load FW /lib/firmware/rtlbt/rtl8822es_fw OK, size 46776
Realtek Bluetooth :rtb_get_fw_project_id: opcode 0, len 1, data 33
Realtek Bluetooth :Receive cmd complete event of command: fc61
Realtek Bluetooth :HCI Revision 0x000e
Realtek Bluetooth :rtb_get_final_patch: key id 0
Realtek Bluetooth :FW version 0x29,8b,34,01,37,71,02,00
Realtek Bluetooth :opcode 0x0001
Realtek Bluetooth :number 0x0001
Realtek Bluetooth :eco 0x01, Eversion:00
Realtek Bluetooth :Pri:15, Patch length 0xb685
Realtek Bluetooth :len = 0xb685
Realtek Bluetooth :len = 0xb685
Realtek Bluetooth :FW  exists, Config file  exists
Realtek Bluetooth :Total len 46758 for fwc
Realtek Bluetooth :baudrate in change speed command: 0x02 0x80 0x92 0x04
Realtek Bluetooth :Receive cmd complete event of command: fc17
Realtek Bluetooth :Received cc of vendor change baud
Realtek Bluetooth :Final speed 1500000
Realtek Bluetooth :end_idx: 185, lp_len: 138, additional pkts: 0

Realtek Bluetooth :Start downloading...
Realtek Bluetooth :Send last pkt
Realtek Bluetooth :Enable host hw flow control
Realtek Bluetooth :h5_hci_reset: Issue hci reset cmd
Realtek Bluetooth :Receive cmd complete event of command: 0c03
Realtek Bluetooth :Received cc of hci reset cmd
Realtek Bluetooth :Init Process finished
Realtek Bluetooth :Realtek Bluetooth post process
Realtek Bluetooth :Device setup complete
```



从log打印开始分析

realtek_init()

  --> rtb_init()

​    --> rtb_init_h5()

```c
static int realtek_init(int fd, struct uart_t *u, struct termios *ti)
{
	RS_INFO("Realtek Bluetooth init uart with init speed:%d, type:HCI UART %s",
		u->init_speed,
		(u->proto == HCI_UART_H4) ? "H4" : "H5");
	return rtb_init(fd, u->proto, u->speed, ti);
}

//hciattach_rtk.c
int rtb_init(int fd, int proto, int speed, struct termios *ti)
{
  struct epoll_event ev;
  int result;

  RS_INFO("Realtek hciattach version %s \n", RTK_VERSION);

  memset(&rtb_cfg, 0, sizeof(rtb_cfg));
  rtb_cfg.serial_fd = fd;
  rtb_cfg.dl_fw_flag = 1;

  rtb_cfg.epollfd = epoll_create(64);
  if (rtb_cfg.epollfd == -1) {
    RS_ERR("epoll_create1, %s (%d)", strerror(errno), errno);
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
  ev.data.fd = fd;
  if (epoll_ctl(rtb_cfg.epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    RS_ERR("epoll_ctl: epoll ctl add, %s (%d)", strerror(errno),
           errno);
    exit(EXIT_FAILURE);
  }

  rtb_cfg.timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (rtb_cfg.timerfd == -1) {
    RS_ERR("timerfd_create error, %s (%d)", strerror(errno), errno);
    return -1;
  }

  if (rtb_cfg.timerfd > 0) {
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    ev.data.fd = rtb_cfg.timerfd;
    if (epoll_ctl(rtb_cfg.epollfd, EPOLL_CTL_ADD,
            rtb_cfg.timerfd, &ev) == -1) {
      RS_ERR("epoll_ctl: epoll ctl add, %s (%d)",
             strerror(errno), errno);
      exit(EXIT_FAILURE);
    }
  }

  RS_INFO("Use epoll");

  if (proto == HCI_UART_3WIRE) {
    if (rtb_init_h5(fd, ti) < 0)
      return -1;;
  }

  result = rtb_config(fd, proto, speed, ti);

  epoll_ctl(rtb_cfg.epollfd, EPOLL_CTL_DEL, fd, NULL);
  epoll_ctl(rtb_cfg.epollfd, EPOLL_CTL_DEL, rtb_cfg.timerfd, NULL);
  close(rtb_cfg.timerfd);
  rtb_cfg.timerfd = -1;

  return result;
}

/*
 * Init realtek Bluetooth h5 proto.
 * There are two steps: h5 sync and h5 config.
 */
int rtb_init_h5(int fd, struct termios *ti)
{
  struct sk_buff *nskb;
  unsigned char h5sync[2] = { 0x01, 0x7E };
  /* 16-bit CCITT CRC may be used and the sliding win size is 4 */
  unsigned char h5conf[3] = { 0x03, 0xFC, 0x14 };
  int result;

  /* Disable CRTSCTS by default */
  ti->c_cflag &= ~CRTSCTS;

  /* set even parity */
  ti->c_cflag |= PARENB;
  ti->c_cflag &= ~(PARODD);
  if (tcsetattr(fd, TCSANOW, ti) < 0) {
    RS_ERR("Can't set port settings");
    return -1;
  }

  /* h5 sync */
  rtb_cfg.link_estab_state = H5_SYNC;
  nskb = h5_prepare_pkt(&rtb_cfg, h5sync, sizeof(h5sync),
            H5_LINK_CTL_PKT);
  result = start_transmit_wait(fd, nskb, OP_H5_SYNC, 500, 10);
  skb_free(nskb);
  if (result < 0) {
    RS_ERR("OP_H5_SYNC Transmission error");
    return -1;
  }

  /* h5 config */
  nskb = h5_prepare_pkt(&rtb_cfg, h5conf, sizeof(h5conf), H5_LINK_CTL_PKT);
  result = start_transmit_wait(fd, nskb, OP_H5_CONFIG, 500, 10);
  skb_free(nskb);
  if (result < 0) {
    RS_ERR("OP_H5_CONFIG Transmission error");
    return -1;
  }

  rtb_send_ack(fd);
  RS_DBG("H5 init finished\n");

  rtb_cfg.cmd_state.state = CMD_STATE_UNKNOWN;

  return 0;
}
```

