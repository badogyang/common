如下为加载BT驱动时的log，加载成功后就会有/dev/ttyBT0设备，搜了一下，没有ANDROID的宏定义，详细log已放附件

[   59.480440] mtty_init entry!
[   59.481029] bt mtty_probe no dts
[   59.481345] mtty_probe init device addr: 0x000000000a75d423
[   59.482719] -->rfkill_bluetooth_init
[   59.482924] <--rfkill_bluetooth_init
[   59.482992] mtty_probe get hw type:0
[   59.483010] WCN BUS: [+]bus_chn_init(17, 0)
[   59.483025] WCN BUS: [-]bus_chn_init(17)
[   59.483042] WCN BUS: [+]bus_chn_init(3, 0)
[   59.483051] WCN BUS: [-]bus_chn_init(3)



root@rockchip:/# ls -l /dev/ttyBT0
crw-rw---- 1 root dialout 510, 0 Jan  1 00:00 /dev/ttyBT0





```
下面是执行结果，这次只进入到了sprd_vendor_hci_init函数，过了一段时间就段错误了

root@rockchip:/# usr/bin/unisoc/hciattach_sprd -l
any       0x0000,0x0000
ericsson  0x0000,0x0000
digi      0x0000,0x0000
bcsp      0x0000,0x0000
sprd      0x0000,0x0000
root@rockchip:/# usr/bin/unisoc/hciattach_sprd /dev/ttyBT0  sprd
open: /dev/ttyBT0, fd, 3
initsprd_vendor_hci_initSegmentation fault

看这个执行结果是在hci_server_thread下的UNSEND(param)中段错误了
root@rockchip:/# usr/bin/unisoc/hciattach_sprd /dev/ttyBT0  sprd
open: /dev/ttyBT0, fd, 3
initsprd_vendor_hci_init begin
sprd_vendor_hci_init end
hci_server_threadSegmentation fault

static void hci_server_thread(void* param){
    socklen_t           clilen;
    struct sockaddr_in  cliaddr, servaddr;
    int n = 1, ret = 0;
    struct timespec timeout;

    fprintf(stderr, "%s", __func__);

    UNUSED(param);

    //BTD("%s1", __func__);
    fprintf(stderr, "%s 1\n", __func__);
    
    
排查到是在读取adapter_module->init的时候段错误了

root@rockchip:/# usr/bin/unisoc/hciattach_sprd /dev/ttyBT0  sprd
open: /dev/ttyBT0, fd, 3
init
sprd_vendor_hci_init begin
sprd_vendor_hci_init end
get_adapter_module begin
get_adapter_module end
adapter_module->init() begin
Segmentation fault

代码：
static int init(const bt_vendor_callbacks_t* p_cb,
                unsigned char* local_bdaddr)
{
...
    fprintf(stderr, "get_adapter_module begin\n");
    adapter_module = get_adapter_module();
    fprintf(stderr, "get_adapter_module end\n");


    fprintf(stderr, "adapter_module->init() begin\n");
    if (adapter_module->init != NULL) {
        fprintf(stderr, "adapter_module->init() != null\n");
        adapter_module->init();
        fprintf(stderr, "adapter_module->init() end\n");
    }


目前来看，是在等待bttools时挂了，accept(s_listen, (struct sockaddr *) &cliaddr, &clilen)，在这里一直没有等到回复，麻烦帮忙看看是什么问题
root@rockchip:/# usr/bin/unisoc/hciattach_sprd /dev/ttyBT0  sprd
open: /dev/ttyBT0, fd, 3
tcflush(fd, TCIOFLUSH)
cfmakeraw(&ti)
tcflush(fd, TCIOFLUSH) 1
u->init && u->init(fd, u, &ti) < 0 fd: 3
sprd_vendor_hci_init
sprd_vendor_hci_inithci_server_threadsocket(AF_INET, SOCK_STREAM, 0);
hci_server_thread wati for btools


    do
    {
        printf("%s wati for btools", __func__);
#ifdef __ANDROID__
        s_socket = accept_server_socket(s_listen);
#else
        s_socket = accept(s_listen, (struct sockaddr *) &cliaddr, &clilen);
#endif
        printf("%s got cmd from btools", __func__);

```





```

现在设备目录有了，但是会报sprd-mtty.0/chipid err 2 No such file or directory，帮忙看看

Jan  1 00:00:43 rockchip user.debug hciattach_sprd: marlin3_init select /sys/devices/platform/sprd-mtty.0/
Jan  1 00:00:43 rockchip user.debug hciattach_sprd: marlin3_init open /sys/devices/platform/sprd-mtty.0/chipid err 2 No such file or directory

dtsi添加了sprd-mtty设备节点，然后mtty.c里面改成了module_init(mtty_init)


```

