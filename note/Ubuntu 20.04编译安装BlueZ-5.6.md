# Ubuntu 20.04编译安装BlueZ-5.6
BlueZ是Linux官方的蓝牙协议栈。可以通过BlueZ提供的接口，进行蓝牙相关的开发。

## 下载BlueZ
官网链接: http://www.bluez.org/download/.
github链接: https://github.com/bluez/bluez.

也可以使用wget下载,然后解压。

wget http://www.kernel.org/pub/linux/bluetooth/bluez-5.60.tar.xz
tar xvf bluez-5.60.tar.xz



git clone git://git.kernel.org/pub/scm/bluetooth/bluez.git 

git clone git://git.kernel.org/pub/scm/libs/ell/ell.git

## 编译BlueZ
### 进入到bluez目录下，执行./configure文件

./configure --prefix=$PWD/out --mandir=/usr/share/man --sysconfdir=/etc --localstatedir=/var

### 根据错误提示安装对应软件包
1.error: GLib >= 2.28 is required

sudo apt install libglib2.0-dev

2.error: D-Bus >= 1.6 is required

sudo apt install libdbus-1-dev

3.error: libudev >= 172 is required

sudo apt install libudev-dev

4.error: libical is required

sudo apt install libical-dev

5.error: readline header files are required

sudo apt install libreadline-dev

6.error: rst2man is required

sudo apt install python-docutils
sudo which rst2man

sudo apt-get install libdw-dev



./bootstrap: 5: libtoolize: not found

注意，每安装一次需要执行一下./configure文件


最后成功创建makefile就算成功了。

```
config.status: creating Makefile
config.status: creating src/bluetoothd.rst
config.status: creating lib/bluez.pc
config.status: creating mesh/bluetooth-meshd.rst
config.status: creating config.h
config.status: executing depfiles commands
config.status: executing libtool commands
```



编译和安装
然后执行make

sudo make
sudo make install



## bluez工具使用

###  1 bluetoothctl
在编译BlueZ时，如果configure配置添加了“–enable-client”，就会编译生成Bluetoothctl工具。如果类比于WiFi，bluetoothd相当于wpa_supplicant，bluetoothctl相当于wpa_cli。

bluetoothctl内部集成了一个shell交互功能，直接在命令行运行bluetoothctl工具即可进入该工具的内部shell，输入help可以查看该工具支持的命令。

[bluetooth]# help
Menu main:

#### Available commands:

```shell
advertise                                         Advertise Options Submenu
scan                                              Scan Options Submenu
gatt                                              Generic Attribute Submenu
list                                              List available controllers
show [ctrl]                                       Controller information
select <ctrl>                                     Select default controller
devices                                           List available devices
paired-devices                                    List paired devices
system-alias <name>                               Set controller alias
reset-alias                                       Reset controller alias
power <on/off>                                    Set controller power
pairable <on/off>                                 Set controller pairable mode
discoverable <on/off>                             Set controller discoverable mode
discoverable-timeout [value]                      Set discoverable timeout
agent <on/off/capability>                         Enable/disable agent with given capability
default-agent                                     Set agent as the default one
advertise <on/off/type>                           Enable/disable advertising with given type
set-alias <alias>                                 Set device alias
scan <on/off>                                     Scan for devices
info [dev]                                        Device information
pair [dev]                                        Pair with device
trust [dev]                                       Trust device
untrust [dev]                                     Untrust device
block [dev]                                       Block device
unblock [dev]                                     Unblock device
remove <dev>                                      Remove device
connect <dev>                                     Connect device
disconnect [dev]                                  Disconnect device
menu <name>                                       Select submenu
version                                           Display version
quit                                              Quit program
exit                                              Quit program
help                                              Display help about this program
export                                            Print environment variables

```

以下列举了一些常用的命令：

power on：打开蓝牙总开关，同理power off就是关闭蓝牙总开关，下面的命令也是同理。
show：显示本地蓝牙适配器的信息。
discoverable on：设置经典蓝牙可被发现，开启后手机的蓝牙设置界面可以搜索到该蓝牙设备。
pair on：使能经典蓝牙的可配对。
system-alias：设置设备的蓝牙别名，即手机蓝牙设置界面扫描时看到的设备名。
agent NoInputNoOutput：设置配对能力级为无输入无输出，设置成该agent后经典蓝牙配对时设备无需输入配对码，手机上点击确定即可完成配对。
scan on：开启蓝牙扫描，如果设备支持双模蓝牙，那么会同时扫描经典蓝牙和低功耗蓝牙，扫描一段时间后可以通过scan off关闭扫描。
devices：列出当前可用列表，该列表包含了已配对的设备和刚扫描到的设备。
info：查看某一设备的具体信息。
connect：尝试连接设备。
disconnect：断开设备连接。
pair：发起配对。
advertise on：开启BLE广播。
remove：移除设备，如果是已配对的设备，绑定关系也会移除。
menu：进入子菜单，目前有advertise、scan、gatt三个子菜单，分别用于设置BLE广播信息、扫描过滤、GATT设置。
举例说明如何使用bluetoothctl连接蓝牙外设：

运行bluetoothctl，输入power on命令使能蓝牙。
蓝牙外设开机并使其广播，使用scan on命令开启扫描，等扫描到目标蓝牙外设后使用scan off命令停止扫描。
使用devices命令查看扫描到的设备地址，，复制该外设对应的蓝牙MAC地址xx:xx:xx:xx:xx:xx。
使用connect xx:xx:xx:xx:xx:xx命令连接蓝牙外设，有些外设首次连接需要配对，将connect改为pair即可。

### 2 hciconfig

hciconfig工具可以实现一些bluetoothctl工具无法完成的功能，bluetoothctl偏向于蓝牙应用层，而hciconfig则偏向于蓝牙层本身。例如hciconfig工具可以设置BR/EDR的inquiry scan参数和page scan参数、设置ACL、SCO的MTU等。具体的用法可以使用hciconfig --help命令来获得。(命令示例 hciconfig hci0 reset)

### 3 l2ping

l2ping工具类似于ping工具，可以测试两个蓝牙设备之间L2CAP链路的通信是否正常。其原理为发送方发送L2CAP_ECHO_REQ命令，接收方应答。

![image-20230224135058727](D:/%E8%B5%84%E6%96%99/common-master/note/img/image-20230224135058727.png)

l2ping用法如下：

```bash
l2ping - L2CAP ping
Usage:
        l2ping [-i device] [-s size] [-c count] [-t timeout] [-d delay] [-f] [-r] [-v] <bdaddr>
        -f  Flood ping (delay = 0)
        -r  Reverse ping
        -v  Verify request and response payload
```

各参数含义如下：

-i：指定hci接口，例如-i 0是指定hci0，默认就是hci0
-s：指定ping包大小，默认是44字节
-c：指定ping包个数
-t：指定最大超时时间，单位是秒
-d：指定两个ping包之间的间隔，单位是秒
-f：相当于-d 0，ping包之间没有间隔，只要收到应答就发送下一个ping包
-r：接收ping包
-v：校验发送与接收到的ping包内容
：指定目标设备的蓝牙地址，格式为xx:xx:xx:xx:xx:xx
命令示例：

```bash
/home/bluez # ./l2ping -f -s 512 -c 10 -t 1 34:1C:F0:F1:E5:3F
Ping: 34:1C:F0:F1:E5:3F from 40:24:B2:D1:F2:A8 (data size 512) ...
512 bytes from 34:1C:F0:F1:E5:3F id 0 time 209.38ms
512 bytes from 34:1C:F0:F1:E5:3F id 1 time 83.60ms
512 bytes from 34:1C:F0:F1:E5:3F id 2 time 81.16ms
512 bytes from 34:1C:F0:F1:E5:3F id 3 time 41.26ms
512 bytes from 34:1C:F0:F1:E5:3F id 4 time 42.35ms
512 bytes from 34:1C:F0:F1:E5:3F id 5 time 108.65ms
512 bytes from 34:1C:F0:F1:E5:3F id 6 time 38.67ms
512 bytes from 34:1C:F0:F1:E5:3F id 7 time 48.65ms
512 bytes from 34:1C:F0:F1:E5:3F id 8 time 44.94ms
512 bytes from 34:1C:F0:F1:E5:3F id 9 time 56.19ms
10 sent, 10 received, 0% loss

```

### 4 l2test

l2test工具用于测试蓝牙L2CAP层的性能，有点类似于iperf工具。l2test工具使用的是服务器-客户端模型，基于L2CAP层建立socket连接。测试时，一台设备需作为server，另一台作为client。下面是测试命令示例，其中-r表示接收模式，-b表示接收多少数据，-s表示发送模式，xx:xx:xx:xx:xx:xx是接收端的地址。l2test工具较为灵活，这个例子是服务器接收、客户端连接并发送，也可以改成服务器发送、客户端连接并接收，只需要将-r改为-w，-s改为-u

服务器：./l2test -r -b 100000 
发送端：./l2test -s xx:xx:xx:xx:xx:xx

这个例子中，服务器端的效果如下：

```bash
/home/bluez # ./l2test -r -b 100000
l2test[836]: Waiting for connection on psm 4113 ...
bluetoothd[489]: src/adapter.c:connected_callback() hci0 device 78:F2:35:0E:D0:46 connected eir_len 11
bluetoothd[489]: src/adapter.c:new_link_key_callback() hci0 new key for 78:F2:35:0E:D0:46 type 4 pin_len 0 store_hint 0
bluetoothd[489]: src/device.c:device_bonding_complete() bonding (nil) status 0x00
bluetoothd[489]: src/adapter.c:resume_discovery() 
l2test[837]: Connected to 78:F2:35:0E:D0:46 (bredr, psm 4113, dcid 64)
l2test[837]: Local device B8:4D:43:35:42:9D (bredr, psm 4113, scid 64)
l2test[837]: Options [imtu 672, omtu 672, flush_to 65535, mode 0, handle 2, class 0x000000, priority 0, rcvbuf 163840]
l2test[837]: Receiving ...
l2test[837]: 100128 bytes in 0.84 sec, 117.10 kB/s
l2test[837]: 100128 bytes in 0.87 sec, 112.98 kB/s
l2test[837]: 100128 bytes in 0.89 sec, 110.14 kB/s
l2test[837]: 100128 bytes in 0.81 sec, 120.81 kB/s
l2test[837]: 100128 bytes in 0.83 sec, 118.36 kB/s
```




l2test还可以用于测试蓝牙反复连接-断开-连接的稳定性，服务器的命令不变，客户端命令从-s改为-c即可。

### 5 hcidump

hcidump可以抓取HCI层的数据，可以直接在控制台打印，也可以保存为文件，这在分析问题时非常有用。下面是使用hcidump在后台运行并直接打印在控制台的效果。

< HCI Command: LE Set Advertising Parameters (0x08|0x0006) plen 15
    min 100.000ms, max 100.000ms
    type 0x00 (ADV_IND - Connectable undirected advertising) ownbdaddr 0x00 (Public)
    directbdaddr 0x00 (Public) 00:00:00:00:00:00
    channelmap 0x07 filterpolicy 0x00 (Allow scan from any, connection from any)
> HCI Event: Command Complete (0x0e) plen 4
> LE Set Advertising Parameters (0x08|0x0006) ncmd 2
> status 0x00
> < HCI Command: LE Set Advertise Enable (0x08|0x000a) plen 1
> HCI Event: Command Complete (0x0e) plen 4
> LE Set Advertise Enable (0x08|0x000a) ncmd 2
> status 0x00
> 直接打印在控制台的方式比较简陋，而且不利于后期分析，可以用hcidump -w output_file & 命令将日志记录在指定文件中，并在后台运行，待记录完数据后再将日志传回PC，直接使用Wireshark打开，效果如下图所示。

