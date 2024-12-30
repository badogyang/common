

``` shell
                        Java                                                                                   
+--------------------------------+                                                                              
  +-----------------+     C++/C                                                                                 
  |       BTIF      |                                                                                           
  +-----------------+                                                                                           
  |       BTA       |                                                                                           
  +-----------------+                                                                                           
  | Bluedroid Stack |                                                                                           
  +-----------------+  user space                                                                               
+---------------------------------+                                                                             
                      kernel space         

```



![image-20211014154417552](C:\Users\xin.yang6\AppData\Roaming\Typora\typora-user-images\image-20211014154417552.png)



Bluedroid主要分为3个部分：**BTIF, BTA, Stack。**
 作为蓝牙核心服务，**Bluetooth Stack模块则由Bluetooth Application Layer（BTA）和Bluetooth Embedded System（缩写为BTE）两大部分组成。**
 **BTE：bluedroid的内部处理，又可以细分为BTA，BTU，BTM，HCI等**

#### BTIF (Bluetooth Profile Interface)

BTIF：Bluetooth Application task(BTA)和JNI层之间从当媒介（网上也说胶水层）。
 对上层JNI提供所有profile功能行的接口。该层还存在Bluetooth Interface Instance,所有Profile操作接口注册在其中（GAP, AV, DM, PAN, HF,HH, HL, Storage, Sockets）。Client应用通过Instance来操作Profile

#### BTA （Bluetooth Appication）

BTA：蓝牙应用层。指bluedroid中对各个profile实现和处理。上层下来的请求经过BTA层，通过消息发送的方式将请求传到BTA层中处理。

所有BTA消息送到BTU_TASK中，由bta_sys_event来处理；如果是Gatt相关的消息，由bta_gatt_hdl_event处理。

> Stack：实现蓝牙底层操作。

#### BTU （Bluetooth Upper Layer）

BTU：承接BTA和HCI

#### BTM（Bluetooth Manager ）

BTM：Bluedroid中的管理层。蓝牙配对和链路管理

#### HCI

HCI：读取和写入数据到蓝牙HW。主机与BT控制器之间的接口。

#### GKI模块

内核统一接口。该层是一个适配层，适配了OS相关的进程、内存相关的管理，还可以用于线程间传递消息 。主要通过变量gki_cb实现对进程的统一管理。GKI模块在Bluedroid中主要用于线程间通信。

#### bluedroid协议栈消息传递和处理

蓝牙协议栈里通信通过**消息队列**完成。

## BLE

#### 概念

BLE根据需要提供短数据包，然后关闭链路，BLE低功耗的原因之一。相对于常规蓝牙的传统配对方式，BLE设备尽在需要收发信息时才进行链接。

BLE通信方式极其严密。设备显示收发数据的服务，后者包含称之为特征的内容，用于定义可共享的数据。特征可包含描述符，帮助 定义数据。

大多数BLE API都支持搜索本地设备和发现有关设备的服务、特征和描述符。

**ATT（Attribute Protoco：属性定义）**
 ATT是专门针对BLE设备而设计的优化型协议。ATT通过发送字节尽可能少的数据。所有属性均带有通用唯一标识符（UUID），后者为标准的128位字符串ID，以唯一的方式识别信息。ATT传输的属性被格式化为特征和服务。

- 特征（Characteristic）：包含一个单独数据以及0个或多个描述符以描述特征值。
- 描述符（Descriptor）：描述符制定了属性，可以描述特征值。可读的描述如可注明单位或测量，或定义可以接受的数值范围
- 服务（Service）：服务指特征的集合。例如一个service叫做“Heart Rate Monitor”，它可能包含多个Characteristics，其中可能包含一个叫做“heart rate measurement"的Characteristic。

**GATT（Generic Attribute Profile：通用属性配置文件）**
 GATT配置文件是关于通过蓝牙低功耗链路收发短数据（称为属性）的通用规范。当前BLE应用配置文件均以GATT为基础。SIG对BLE设备的配置文件数量进行了预定义。这些配置文件是关于描述设备使用方法的规范。

**GAP（ Generic Access Profile：通用访问规范）**
 定义了设备如何发现，建立连接，实现绑定。

**SM（Security Manager）**
 负责BLE通信中安全。

**中心设备和外围设备**
 Central与peripheral
 GATT server与GATT client

GAP用于外设设备与中心设备，每个设备可以充当多种角色，同一时间只能充当一种角色
 GATT用于客户端与服务器。

