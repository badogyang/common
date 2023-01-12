# HFP协议

HFP 全称为Hands-Free Profile，通俗的说就是蓝牙电话协议，可以通过指定好的AT command来控制通话的接听、挂断、拒接等



## 看协议的一些约定格式

在HFP协议文档里面有一个约定，这里贴出来，每种不同的标识代表不同的意思，后面会用到

“M”表示强制支持

“O”为可选支持

“X”表示排除(用于设备可能支持的功能，但免提功能Profile不能使用这些功能)

“C”表示有条件支持

“N/A”表示不适用(在给定的上下文中没有定义此功能)



![image-20230111141121093](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111141121093.png)



## 协议层级

HFP在协议栈中的层级

![image-20230111142108356](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111142108356.png)

![image-20230111140235829](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111140235829.png)



## HFP协议角色

![image-20230111145107279](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111145107279.png)

**Audio Gateway (AG)** – This is the device that is the gateway of the audio, both for input and output. Typical devices acting as Audio Gateways are cellular phones. 这个就是音频的输入输出角色，一般是手机。
**Hands-Free unit (HF)** – This is the device acting as the Audio Gateway’s remote audio input and output mechanism. It also provides some remote control means. 这里翻译过来大概就是Audio Gateway的输入输出角色，平常的耳机，车载都属于这类角色



## 特性支持

![image-20230111145715382](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111145715382.png)

![image-20230111145802177](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111145802177.png)

机翻有点生硬，对照上面看

![image-20230111145934757](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111145934757.png)

上面的表示AG和HF分别支持的功能，如果支持，则需要进行如下的操作

![image-20230111150446289](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111150446289.png)

![image-20230111150526904](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111150526904.png)

编码要求

![image-20230111150744015](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111150744015.png)

具体的可以看HFP官方文档



## 4 Hands-Free Control Interoperability Requirements  

HFP文档的第四章就是对支持功能需要哪些操作的一个细致讲解解释，只对第一个也就是Service Level Connection Establishment 进行分析

### Service Level Connection Establishment 

首先看一下整体流程，然后一步步解释

![image-20230111153307814](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111153307814.png)



建立SLC(Service Level Connection)需要有RFCOMM connection，也就是在HF和AG之间存在RFCOMM connection

HF和AG都可以发起RFCOMM connection的建立。如果AG与HF之间没有RFCOMM会话，则由发起设备先初始化RFCOMM。

RFCOMM连接的建立请参见《Generic Access Profile[5]》章节7.3和《Serial Port Profile[6]》章节3。



## Service Level Connection初始化

在RFCOMM connection建立成功后，就可以去执行SLC的初始化

### 1 Supported features exchange  (支持的特性交换)

首先，在初始化过程中，HF向AG发送 AT+BRSF=<HF支持的特征> 命令，通知AG端HF中支持的特征，并使用 +BRSF 结果代码检索AG中支持的特征。

#### AT+BRSF=<HF **supported** features bitmap>  

**Description:** Notifies the AG of the supported features available in the HF, and requests information about the
supported features in the AG. The supported features shall be represented as a decimal value.

**Values**: <**HF supported features bitmap**>: a decimal numeric string, which represents the value of a 32 bit
unsigned integer. The 32 bit unsigned integer represents a bitmap of the supported features in

**the HF as follows:**  

![image-20230111164611449](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111164611449.png)

其中 bit7 为 V1.6 才添加， bit 7,8,9 是 V1.7 才添加,bit 10,11 是 V1.8 才添加  



#### +BRSF (Bluetooth Retrieve Supported Features)

**语法结构:** +BRSF: <**AG supported features bitmap**>

**Description:** Result code sent by the AG in response to the AT+BRSF command, used to notify the HF what
features are supported in the AG. The supported features shall be represented as a decimal
value.

**Values:** <**AG supported features bitmap**>: a decimal numeric string, which represents the value of a 32 bit
unsigned integer. The 32 bit unsigned integer represents a bitmap of the supported features in

**the AG as follows:**  

![image-20230111165018843](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111165018843.png)

其中 bit9 为 V1.6 才添加， bit10,11 是 V1.7 才添加， bit 11,12 是 V1.8 才添加  



### 2 Codec Negotiation  编解码协商

其次，在初始化过程中，如果HF支持编解码协商特性，则需要检查AG的AT+BRSF命令响应是否表明其支持编解码协商特性。如果HF和AG都支持编解码器协商特性，HF就应该向AG发送AT+BAC=<HF可用编解码器>命令，通知AG端HF中可用的编解码器

编解码方式， mSBC 就是 WBS(宽带电话， 16KHz)的编解码方式,ID 如下表所示  

![image-20230111150744015](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111150744015.png)



### 3 AG Indicators  

> After having retrieved the supported features in the AG, the HF shall determine which indicators are supported by the AG, as well as the ordering of the supported indicators. This is because, according to the 3GPP 27.007 specification [2], the AG may support additional indicators not provided for by the Hands-Free Profile, and because the ordering of the indicators is implementation specific. The HF uses the AT+CIND=? Test command to retrieve information about the supported indicators and their ordering. Once the HF has the necessary supported indicator and ordering information, it shall retrieve the current status of the indicators in the AG using the AT+CIND? Read command.
>
> After having retrieved the status of the indicators in the AG, the HF shall then enable the "Indicators status update" function in the AG by issuing the AT+CMER command, to which the AG shall respond with OK. As a result, the AG shall send the +CIEV unsolicited result code with the corresponding indicator value whenever a change in service, call, or call setup status occurs. When an update is required for both the call and call setup indicators, the AG shall send the +CIEV unsolicited result code for the call indicator before sending the +CIEV unsolicited result code for the call setup indicator. The HF shall use the information provided by the +CIEV code to update its own internal and/or external indications. Once the "Indicators status update" function has been enabled, the AG shall keep the function enabled until either the AT+CMER command is issued to disable it, or the current Service Level Connection between the AG and the HF is dropped for any reason.
>
> After the HF has enabled the “Indicators status update” function in the AG, and if the “Call waiting and 3- way calling” bit was set in the supported features bitmap by both the HF and the AG, the HF shall issue the AT+CHLD=? test command to retrieve the information about how the call hold and multiparty services are supported in the AG. The HF shall not issue the AT+CHLD=? test command in case either the HF or the AG does not support the "Three-way calling" feature.  

整理一下

1) 发送AT+CIND=?问询支持的Indicators，包括(service/call/callsetup/callheld/signal/roam/battchg )，我们拿到了这个值才能对应的解析 AG 发送给 HF 的+CIEV AT 命令！  call的index是1，callsetup的index是2，service的index是3，依次类推

   ![image-20230111173422598](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111173422598.png)

2) 发送 AT+CIND?问询各个 indicators 的 status ,就是统一问下各个 indicator 的值， 接着上图问询 AT+CIND?  

   ![image-20230111175205668](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111175205668.png)



**各个value代表的意思如下：**

![image-20230111183552212](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111183552212.png)

![image-20230111183611146](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111183611146.png)



3) 发送 AT+CMER enable 各个 indicators,发送这个后， 如果某一个 indicator 有变化， 那么 AG 就可以发送+CIEV 来告知  **AT+CMER 是 Standard event reporting activation/deactivation AT command.说白了就是使能/失能 indicator**， 一共有两种格式

   AT+CMER=3,0,0,1 activates “indicator events reporting” .

   AT+CMER=3,0,0,0 deactivates “indicator events reporting” .

   使能之后， AG 可以发送+CIEV 命令来汇报各个 indicator 的变化

     

   **+CIEV:**Standard “indicator events reporting” unsolicited result code.格式为： 

   +CIEV: <ind>,<value>， 举例来说（此部分要根据 AT+CIND=?问询到的index 来解析， 每个 AG 可能 index 不同， 所以代码中有解析 index 的动作， 这个 index 是根据 前面的 index 来讲解）  强调 **这个 index 是根据 AT+CIND=?来获取到的， 每个 AG 可能不同的** ！！！ 

   如果后续 AG 发送过来 +CIEV:3,x 那么就是 service 有变化， 值为 x, 来个具体的例子， 如图  



![image-20230111191023621](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111191023621.png)



4) 以上三个发送完毕，如果HF和AG都支持三方通话，那么发送AT+CHLD=? 此部分是 HF 问询 AG 三方通话的支持的特性都有哪些，一共有如下特性

   

   ~~~
   Standard call hold and multiparty handling AT command. In the AT+CHLD=<n> command, this specification only covers values for <n> of 0, 1, 1<idx>, 2, 2<idx>, 3 and 4, where:
   
   - 0 = Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call.
   --> 相当于此时正在通话中， 拒接所有等待中的电话， 告知对方 busy 状态
   
   - 1 = Releases all active calls (if any exist) and accepts the other (held or waiting) call.
   --> 挂断所有在通话中的电话， 接听来电
   
   - 1<idx> = Releases call with specified index (<idx>).Hands-Free Profile / Profile Specification Bluetooth SIG Proprietary Page 86 of 139
   --> 挂断 idx 的通话中的电话
   
   - 2 = Places all active calls (if any exist) on hold and accepts the other (held or waiting) call.
   --> 把所有通话中的电话设置为 hold 状态， 然后接听电话 
   
   - 2<idx> = Request private consultation mode with specified call (<idx>). (Place all calls on hold EXCEPT the call indicated by <idx>.)
   --> 请求接受<idx>标识电话， 让其它电话保持。
   
   - 3 = Adds a held call to the conversation.
   --> 增加一个保持电话到对话中
   
   - 4 = Connects the two calls and disconnects the subscriber from both calls (Explicit Call Transfer). Support for this value and its associated functionality is optional for the HF.
   --> 连接两个电话并且断开两个电话的订阅。 HF 侧可选。
   
   - Where both a held and a waiting call exist, the above procedures shall apply to the waiting call(i.e., not to the held call) in conflicting situation.
   ~~~

   ![image-20230111193154762](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111193154762.png)

### 4 HF Indicators  

> If the HF supports the HF indicator feature, it shall check the +BRSF response to see if the AG also supports the HF Indicator feature.
>
> If both the HF and AG support the HF Indicator feature, then the HF shall send the AT+BIND=<HF supported HF indicators> command to the AG to notify the AG of the supported indicators’ assigned numbers in the HF. The AG shall respond with OK. 
>
> After having provided the AG with the HF indicators it supports, the HF shall send the AT+BIND=? to request HF indicators supported by the AG. The AG shall reply with the +BIND response listing all HF indicators that it supports followed by an OK. 
>
> Once the HF receives the supported HF indicators list from the AG, the HF shall send the AT+BIND? command to determine which HF indicators are enabled. The AG shall respond with one or more +BIND responses. The AG shall terminate the list with OK. (See Section 4.36.1.3). 
>
> From this point onwards, the HF may send the AT+BIEV command with the corresponding HF indicator value whenever a change in value occurs of an enabled HF indicator.
>
> The AG may enable or disable the notification of any HF indicator at any time by using the +BIND
> unsolicited response (See Section 4.36.1.4).  

这部分有几个重点：

1) 如果 HF & AG 都支持 HF Indicators 的 feature,那么 HF 发送 AT+BIND=<**HF supported HF indicators**>来告知 AG 支持那些 indicator， HFP 的 indicator 一共有两个， 如下图：

   ![image-20230112094801382](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230112094801382.png)

2) 发送 AT+BIND=?问询 AG 支持哪些 indicator

3)  发送 AT+BIND?问询 AG 哪些 indicator 是 enable 的

4)  发送 AT+BIEV 来使能某一个 indicator  



到这里，一个SLC连接就算建立了，回看一下流程图

![image-20230111153307814](./HFP%E5%8D%8F%E8%AE%AE%E5%88%86%E6%9E%90/image-20230111153307814.png)