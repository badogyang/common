# bluez源码结构

## 源码目录

src：核心程序bluetoothd的源码位置。 其中还包括了shared文件夹，该文件夹编译生成了一个共享库，供bluetoothd和其他程序使用（shared库是重点）。

client：bluetoothctl程序的源码位置。

mesh：mesh协议栈的源码位置，但是不包含proxy相关的功能。

tools：hciattach，hciconfig，hcitool等工具的源码目录，如果打开--enable-testing，--enable-test的话，在该目录中，还会有相关的*-tester执行程序。

monitor：btmon工具的源码位置，到目前5.66为止，这个工具已经很强大了，监控生成的hci数据，可以转化为支持ellisys软件打开的.pkt格式，也可以转化为支持wireshare软件打开的格式.cfa格式。

test：测试蓝牙功能的py脚本，通过dbus接口和bluetoothd进行通信。 dbus提供支持python和c语言的接口。

emulator：从字面意思可以推测出它是用来仿真的，通过阅读代码发现它是仿真controller的，通过btdev_create函数来创建一个虚拟的蓝牙controller设备。 kernel也是支持的，"/dev/vhci"这个设备被虚拟为一个虚拟的蓝牙设备。

lib：bluez的一些基础访问库的源码位置，shared库也引用了lib里面的函数定义。 编译生成libbluetooth-internal.la和libbluetooth.la

gdbus：dbus的源码仓库。

ell：The Embedded Learning Library，对嵌入式系统的支持，需要在configure时指定使用ell库，方便编译出更少内存和flash的嵌入式平台的bluez的相关固件。

android：早期的android系统使用的是bluez的开源库，后面改为使用bludroid协议栈了。

peripheral：一个ble外围设备的demo，没有通过dbus和bluetoothd进行通信，而是自己实现了一套，使用了shared库的api。 默认不会编译，需要在configure的时候，--enable-test --enable-testing才会编译该目录的源码。

profiles：android平台的协议栈支持的一部分，配合和android目录一起使用。

plugins：貌似是为bluetoothd程序提供一些插件功能。

obexd：经典蓝牙的obex文件传输协议的源码实现。

unit：一些测试shared库的单元测试case，c代码辨析的。

attrib：gatttool工具的实现源码位置。

btio：bt_io_*的相关函数源码位置，bluetooth的io操作的基础库。