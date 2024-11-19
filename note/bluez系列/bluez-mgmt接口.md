# mgmt接口调用

在bluetoothd启动是，会进行初始化，在这里会获取mgmt句柄来进行和内核之间的通信

```c
8677 int adapter_init(void)
8678 {
8679     dbus_conn = btd_get_dbus_connection();/*获取dbus连接后的句柄*/
8680     /*获取mgmt句柄，用于和内核层mgmt进行通信*/
8681     mgmt_master = mgmt_new_default();
8682     if (!mgmt_master) {
8683         error("Failed to access management interface");
8684         return -EIO;
8685     }
8686 
8687     if (getenv("MGMT_DEBUG"))
8688         mgmt_set_debug(mgmt_master, mgmt_debug, "mgmt: ", NULL);
8689 
8690     DBG("sending read version command");
8691     /*向内核发送读取mgmt版本号,具体定义在net/bluetooth/mgmt.c中MGMT_VERSION与MGMT_REVISION*/
8692     if (mgmt_send(mgmt_master, MGMT_OP_READ_VERSION,
8693                 MGMT_INDEX_NONE, 0, NULL,
8694                 read_version_complete, NULL, NULL) > 0)
8695         return 0;
8696 
8697     error("Failed to read management version information");
8698 
8699     return -EIO;
8700 }
```

```C
struct mgmt *mgmt_new_default(void)
{
	struct mgmt *mgmt;
	union {
		struct sockaddr common;
		struct sockaddr_hci hci;
	} addr;
	int fd;

	fd = socket(PF_BLUETOOTH, SOCK_RAW | SOCK_CLOEXEC | SOCK_NONBLOCK,
								BTPROTO_HCI);    //获取内核蓝牙协议栈句柄
	if (fd < 0)
		return NULL;

	memset(&addr, 0, sizeof(addr));
	addr.hci.hci_family = AF_BLUETOOTH;
	addr.hci.hci_dev = HCI_DEV_NONE;
	addr.hci.hci_channel = HCI_CHANNEL_CONTROL;

	if (bind(fd, &addr.common, sizeof(addr.hci)) < 0) {   //绑定control通道
		close(fd);
		return NULL;
	}

	mgmt = mgmt_new(fd);
	if (!mgmt) {
		close(fd);
		return NULL;
	}

	mgmt->close_on_unref = true;

	return mgmt;
}
```





```C
unsigned int mgmt_send(struct mgmt *mgmt, uint16_t opcode, uint16_t index,
				uint16_t length, const void *param,
				mgmt_request_func_t callback,
				void *user_data, mgmt_destroy_func_t destroy)
{
	return mgmt_send_timeout(mgmt, opcode, index, length, param, callback,
					user_data, destroy, 0);
}

-->

unsigned int mgmt_send_timeout(struct mgmt *mgmt, uint16_t opcode,
				uint16_t index, uint16_t length,
				const void *param, mgmt_request_func_t callback,
				void *user_data, mgmt_destroy_func_t destroy,
				int timeout)
{
	struct mgmt_request *request;

	if (!mgmt)
		return 0;

	request = create_request(mgmt, opcode, index, length, param,
					callback, user_data, destroy, timeout);   //创建请求
	if (!request)
		return 0;

	if (mgmt->next_request_id < 1)
		mgmt->next_request_id = 1;

	request->id = mgmt->next_request_id++;

	if (!queue_push_tail(mgmt->request_queue, request)) {   //入mgmt请求队列
		free(request->buf);
		free(request);
		return 0;
	}

	wakeup_writer(mgmt);       //

	return request->id;
}

-->

static void wakeup_writer(struct mgmt *mgmt)
{
	if (!queue_isempty(mgmt->pending_list)) {
		/* only queued reply commands trigger wakeup */
		if (queue_isempty(mgmt->reply_queue))
			return;
	}

	if (mgmt->writer_active)
		return;

	mgmt->writer_active = true;

	io_set_write_handler(mgmt->io, can_write_data, mgmt,
						write_watch_destroy);        //设置写入句柄
}

-->

static bool can_write_data(struct io *io, void *user_data)
{
	struct mgmt *mgmt = user_data;
	struct mgmt_request *request;
	bool can_write;

	request = queue_pop_head(mgmt->reply_queue);
	if (!request) {
		/* only reply commands can jump the queue */
		if (!queue_isempty(mgmt->pending_list))
			return false;

		request = queue_pop_head(mgmt->request_queue);
		if (!request)
			return false;

		can_write = false;
	} else {
		/* allow multiple replies to jump the queue */
		can_write = !queue_isempty(mgmt->reply_queue);
	}

	if (!send_request(mgmt, request))
		return true;

	return can_write;
}

--->

static bool send_request(struct mgmt *mgmt, struct mgmt_request *request)
{
	struct iovec iov;
	ssize_t ret;

	iov.iov_base = request->buf;
	iov.iov_len = request->len;

	ret = io_send(mgmt->io, &iov, 1);     //这里到内核层
	if (ret < 0) {
		DBG(mgmt, "write failed: %s", strerror(-ret));

		if (request->callback)
			request->callback(MGMT_STATUS_FAILED, 0, NULL,
							request->user_data);
		destroy_request(request);
		return false;
	}

	if (request->timeout)
		request->timeout_id = timeout_add_seconds(request->timeout,
							request_timeout,
							request,
							NULL);

	DBG(mgmt, "[0x%04x] command 0x%04x", request->index, request->opcode);

	queue_push_tail(mgmt->pending_list, request);

	return true;
}

```



# 内核层处理

初始化mgmt

```C
static struct hci_mgmt_chan chan = {
	.channel	= HCI_CHANNEL_CONTROL,
	.handler_count	= ARRAY_SIZE(mgmt_handlers),
	.handlers	= mgmt_handlers,
	.hdev_init	= mgmt_init_hdev,
};     //mgmt_handlers这个对应的就是对于上层传递的处理

int mgmt_init(void)
{mgmt_chan
	return hci_mgmt_chan_register(&chan);   //注册mgmt_chan
}

void mgmt_exit(void)
{
	hci_mgmt_chan_unregister(&chan);
}

//mgmt.h   
//MGMT_OP_READ_VERSION MGMT_READ_VERSION_SIZE对应
#define MGMT_OP_READ_VERSION		0x0001
#define MGMT_READ_VERSION_SIZE		0
struct mgmt_rp_read_version {
	__u8	version;
	__le16	revision;
} __packed;

//hci_sock.c
int hci_mgmt_chan_register(struct hci_mgmt_chan *c)
{
	if (c->channel < HCI_CHANNEL_CONTROL)
		return -EINVAL;

	mutex_lock(&mgmt_chan_list_lock);
	if (__hci_mgmt_chan_find(c->channel)) {
		mutex_unlock(&mgmt_chan_list_lock);
		return -EALREADY;
	}

	list_add_tail(&c->list, &mgmt_chan_list);

	mutex_unlock(&mgmt_chan_list_lock);

	return 0;
}
EXPORT_SYMBOL(hci_mgmt_chan_register);
```



## bluez——mgmt分析

### 1,cmd下发

linux系统的bluez的代码是存在与两部分，一部分在kernel，实现协议的一些基本功能，还有一部分在user space实现协议的一些上层功能。
两部分之间的交互通过sockt机制，就是mgmt。
cmd的下发主要调用的是mgmt.c中的mgmt_send（）函数

```c
unsigned int mgmt_send(struct mgmt *mgmt, uint16_t opcode, uint16_t index,
				uint16_t length, const void *param,
				mgmt_request_func_t callback,
				void *user_data, mgmt_destroy_func_t destroy)
{
	return mgmt_send_timeout(mgmt, opcode, index, length, param, callback,
					user_data, destroy, 0);
}

unsigned int mgmt_send_timeout(struct mgmt *mgmt, uint16_t opcode,
				uint16_t index, uint16_t length,
				const void *param, mgmt_request_func_t callback,
				void *user_data, mgmt_destroy_func_t destroy,
				int timeout)
{
	struct mgmt_request *request;

	if (!mgmt)
		return 0;

	request = create_request(mgmt, opcode, index, length, param,
					callback, user_data, destroy, timeout);
	if (!request)
		return 0;

	if (mgmt->next_request_id < 1)
		mgmt->next_request_id = 1;

	request->id = mgmt->next_request_id++;

	if (!queue_push_tail(mgmt->request_queue, request)) {
		free(request->buf);
		free(request);
		return 0;
	}

	wakeup_writer(mgmt);

	return request->id;
}
```



mgmt_send函数有8个参数，第一个是mgmt的参数，暂时没找到其定义，第二是比较重要的数值，在mgmt_api和mgmt.h中都有定义和说明，每一个opcode对应一个cmd，在kernel部分也一模一样定义了对应的opcode。
mgmt_send后就是靠opcode是数值对应在kernel中需要调用的对应函数。
第二参数是index
第三个需要传递的参数大小
第四个是需要传递的参数
第五个是传递的回调，用于执行完该cmd后需要回调的数据
第六个是user需要传递的参数，一般未NULL，第六个也是，是预留设计。

### 2，event上报

除了前面说的cmd下发注册的回调外，kernel部分的event上报一般调用mgmt.c中的mgmt_event（）函数来完成，该函数实际是调用的mgmt_send_event()

```c
static int mgmt_event(u16 event, struct hci_dev *hdev, void *data, u16 len,
		      struct sock *skip_sk)
{
	return mgmt_send_event(event, hdev, HCI_CHANNEL_CONTROL, data, len,
			       HCI_SOCK_TRUSTED, skip_sk);
}

int mgmt_send_event(u16 event, struct hci_dev *hdev, unsigned short channel,
		    void *data, u16 data_len, int flag, struct sock *skip_sk)
{
	struct sk_buff *skb;
	struct mgmt_hdr *hdr;

	skb = alloc_skb(sizeof(*hdr) + data_len, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	hdr = skb_put(skb, sizeof(*hdr));
	hdr->opcode = cpu_to_le16(event);
	if (hdev)
		hdr->index = cpu_to_le16(hdev->id);
	else
		hdr->index = cpu_to_le16(MGMT_INDEX_NONE);
	hdr->len = cpu_to_le16(data_len);

	if (data)
		skb_put_data(skb, data, data_len);

	/* Time stamp */
	__net_timestamp(skb);

	hci_send_to_channel(channel, skb, flag, skip_sk);

	if (channel == HCI_CHANNEL_CONTROL)
		hci_send_monitor_ctrl_event(hdev, event, data, data_len,
					    skb_get_ktime(skb), flag, skip_sk);

	kfree_skb(skb);
	return 0;
}
```



第一个参数无容置疑就是注册的evnet数值



# 搜索设备



```c
static const struct bt_shell_menu main_menu = {
	.name = "main",
	.entries = {
	{ "find",		"[-l|-b] [-L]",
		cmd_find,		"Discover nearby devices"	},
---
        
static void cmd_find(int argc, char **argv)
{
	struct mgmt_cp_start_discovery cp;
	uint8_t op = MGMT_OP_START_DISCOVERY;
	uint8_t type = SCAN_TYPE_DUAL;
	int opt;
	uint16_t index;

	index = mgmt_index;
	if (index == MGMT_INDEX_NONE)
		index = 0;

	while ((opt = getopt_long(argc, argv, "+lbLh", find_options,
								NULL)) != -1) {
		switch (opt) {
		case 'l':
			type &= ~SCAN_TYPE_BREDR;
			type |= SCAN_TYPE_LE;
			break;
		case 'b':
			type |= SCAN_TYPE_BREDR;
			type &= ~SCAN_TYPE_LE;
			break;
		case 'L':
			op = MGMT_OP_START_LIMITED_DISCOVERY;
			break;
		case 'h':
			bt_shell_usage();
			optind = 0;
			return bt_shell_noninteractive_quit(EXIT_SUCCESS);
		default:
			bt_shell_usage();
			optind = 0;
			return bt_shell_noninteractive_quit(EXIT_FAILURE);
		}
	}

	optind = 0;

	memset(&cp, 0, sizeof(cp));
	cp.type = type;

	if (mgmt_send(mgmt, op, index, sizeof(cp), &cp, find_rsp,
							NULL, NULL) == 0) {
		error("Unable to send start_discovery cmd");
		return bt_shell_noninteractive_quit(EXIT_FAILURE);
	}
}

```

## 内核层处理



```c
MGMT_OP_START_DISCOVERY

static int start_discovery(struct sock *sk, struct hci_dev *hdev,
			   void *data, u16 len)
{
	return start_discovery_internal(sk, hdev, MGMT_OP_START_DISCOVERY,
					data, len);
}

static int start_discovery_internal(struct sock *sk, struct hci_dev *hdev,
				    u16 op, void *data, u16 len)
{
	struct mgmt_cp_start_discovery *cp = data;
	struct mgmt_pending_cmd *cmd;
	u8 status;
	int err;

	BT_DBG("%s", hdev->name);

	hci_dev_lock(hdev);

	if (!hdev_is_powered(hdev)) {
		err = mgmt_cmd_complete(sk, hdev->id, op,
					MGMT_STATUS_NOT_POWERED,
					&cp->type, sizeof(cp->type));
		goto failed;
	}

	if (hdev->discovery.state != DISCOVERY_STOPPED ||
	    hci_dev_test_flag(hdev, HCI_PERIODIC_INQ)) {
		err = mgmt_cmd_complete(sk, hdev->id, op, MGMT_STATUS_BUSY,
					&cp->type, sizeof(cp->type));
		goto failed;
	}

	if (!discovery_type_is_valid(hdev, cp->type, &status)) {
		err = mgmt_cmd_complete(sk, hdev->id, op, status,
					&cp->type, sizeof(cp->type));
		goto failed;
	}

	/* Clear the discovery filter first to free any previously
	 * allocated memory for the UUID list.
	 */
	hci_discovery_filter_clear(hdev);

	hdev->discovery.type = cp->type;
	hdev->discovery.report_invalid_rssi = false;
	if (op == MGMT_OP_START_LIMITED_DISCOVERY)
		hdev->discovery.limited = true;
	else
		hdev->discovery.limited = false;

	cmd = mgmt_pending_add(sk, op, hdev, data, len);    //关注这里
	if (!cmd) {
		err = -ENOMEM;
		goto failed;
	}

	cmd->cmd_complete = generic_cmd_complete;

	hci_discovery_set_state(hdev, DISCOVERY_STARTING);
	queue_work(hdev->req_workqueue, &hdev->discov_update);
	err = 0;

failed:
	hci_dev_unlock(hdev);
	return err;
}
```

