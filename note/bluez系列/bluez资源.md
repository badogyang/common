# bluez学习资料

Linux操作系统中默认的蓝牙协议栈是Bluez。

**1、简介**

Bluez的应用方式，有两种：

（1）借助于Bluez的工具（如：bluetoothctl），可以实现：扫描、发现设备、配对、连接等常用的操作。优点：不需要写代码，简单。缺点：动态去扫描蓝牙设备，把蓝牙设备加入一个列表，在屏幕上显示，用户选择与某个蓝牙设备连接，以及接收通知、写特性。上述需求，用shell命令存在很大局限。

（2）通过D-BUS [API](https://so.csdn.net/so/search?q=API&spm=1001.2101.3001.7020)编写代码，实现蓝牙应用。缺点：D-BUS API入门学习门槛比较高（参考资源很少）。优点：能实现更复杂的应用。

**2、Bluez DBUS API参考资源**

值得参考的资源：

（1）BluezV5.42 DBUS C API for BLE?（[链接](https://stackoverflow.com/questions/43475751/bluezv5-42-dbus-c-api-for-ble)）：stackoverflow网站的一个讨论D-BUS API的帖子。

（2）Library to access GATT information from BLE (Bluetooth Low Energy) devices（[链接](https://github.com/labapart/gattlib)）：D-BUS的实例代码。

（3）bluez源代码（[链接](https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/)）

下面是中文的参考资源（有2篇谈及D-BUS）

（4）BlueZ5.45 D-Bus总线 GATT API 分析（[链接](http://blog.csdn.net/csdn_zyp2015/article/details/73089380)）

（5）BlueZ5（[链接](http://blog.csdn.net/Archer1991/article/details/62233164?locationNum=16&fps=1)）

目前只找到这么多，后续如有更好的资源，再补充。

**3、蓝牙规范文档**

学习蓝牙必需参考官方文档。

蓝牙4.1规范（[Core_V4.1.pdf](https://www.bluetooth.org/DocMan/handlers/DownloadDoc.ashx?doc_id=282159)）

蓝牙4.2规范（[Core_V4.2.pdf](https://www.bluetooth.org/DocMan/handlers/DownloadDoc.ashx?doc_id=286439&_ga=1.139498554.805272089.1452604944)）