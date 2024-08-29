## 1.1.1、查看当前Soc的声卡状态
  cat /proc/asound/cards  

例如，插入USB声卡之后，会新增声卡节点，USB声卡无声可优先查看该状态。

## 1.1.2、查看当前声卡工作状态
声卡分两种通道，一种是Capture、一种是Playback。Capture是输入通道，Playback是输出通道。例如pcm0p属于声卡输出通道，pcm0c属于声卡输入通道。

例如声卡0的目录

### 1.1.2.1、查看所有输入通道状态：

cat /proc/asound/card0/pcm*c/sub0/status

### 1.1.2.2、查看所有输出通道状态

cat /proc/asound/card0/pcm*p/sub0/status

例如，查看HDMI tx输出通道(pcm1p)状态。

### 1.1.2.3、查看当前运行状态

cat /proc/asound/card0/pcm1p/sub0/status

state: 当前输出运行状态
owner_pid:调用者的线程号
delay: 当前buffer中可用数据大小（单位为：帧）
avail：当前buffer中空闲空间大小（单位为：帧）[为pcm*c录音时，该值为可用数据大小]
hw_ptr: alsa驱动读取指针位置 [为pcm*c录音时，该值为 alsa驱动写入指针位置]
appl_ptr:alsa写入数据者的指针位置 [为pcm*c录音时，该值为alsa读取数据者的指针位置]



### 1.1.2.4、查看当前通道的软件参数

cat /proc/asound/card0/pcm1p/sub0/hw_params

format: 当前数据位宽
channels：当前声道数
rate：当前采样率
period：单次处理数据的大小（单位为：帧）
buffer_size: alsa 的buffer大小（单位为：帧）
通过buffer_size可计算出buffer的时间长度：

48k，缓存buffer大小为3072帧， (3072/48000)*1000 = 64ms缓存时间



### 1.1.2.5、查看当前通道的硬件参数

cat /proc/asound/card0/pcm1p/sub0/sw_params
start_threshold: 当起播时buffer中的可用数据大小大于等于start_threshold时alsa才启动播放
stop_threshold：当播放过程中buffer空闲大小大于等于stop_threshold时alsa停止播放
boundary：虚拟的buffer大小（一个回卷的大小）



tinymix
## 1.2.1、可通过该命令查看当前音频运行状态

## 1.2.2、可通过该命令debug
通过tinymix 12 可查看当前ID为12的状态
通过tinymix 12 1 可修改ID为12的参数的值