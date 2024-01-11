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

	if (!queue_push_tail(mgmt->request_queue, request)) {   //如mgmt请求队列
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

