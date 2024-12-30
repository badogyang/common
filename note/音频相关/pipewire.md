# 概述

PipeWire 是一个新的底层多媒体框架。 它旨在以最低的延迟为音频和视频提供录制和播放功能，并支持基于 PulseAudio、JACK、ALSA 和 GStreamer 的应用程序。

基于该框架的守护进程可以配置为音频服务器(具有 PulseAudio 和 JACK 特性)和视频录制服务器。

PipeWire 还支持像 Flatpak 这样的容器，不依赖于 audio 和 video 用户组。 相反，它采用了类似于 Polkit的安全模式，向 Flatpak 或 Wayland 请求许可以录制屏幕或音频。



```
加载驱动
#realtek
/usr/libexec/bluetooth/bluetoothd -n &
mount -o remount rw /
/etc/init.d/S50pipewire start
sleep 0.5
insmod /usr/lib/modules/hci_uart.ko
sleep 0.5
echo 0 > /sys/class/rfkill/rfkill0/state
echo 0 > /proc/bluetooth/sleep/btwrite
sleep 0.5
echo 1 > /sys/class/rfkill/rfkill0/state
echo 1 > /proc/bluetooth/sleep/btwrite
sleep 0.5
rtk_hciattach -n -s 115200 ttyS1 rtk_h5 &

pactl set-default-sink 64

amixer -c 0 cset numid=1 3
amixer -c 0 cset numid=2 1

pactl list sinks short

1988   940
1888   945


#高通
/usr/libexec/bluetooth/bluetoothd -n &
mount -o remount rw /
/etc/init.d/S50pipewire start
sleep 0.5
echo 0 > /sys/class/rfkill/rfkill0/state
echo 0 > /proc/bluetooth/sleep/btwrite
sleep 0.5
echo 1 > /sys/class/rfkill/rfkill0/state
echo 1 > /proc/bluetooth/sleep/btwrite
sleep 0.5
hciattach /dev/ttyS1 qca -t120 3000000 flow &
```





```
root@rockchip:/# arecord -l
**** List of CAPTURE Hardware Devices ****
card 0: rockchiprk809co [rockchip,rk809-codec], device 0: fe410000.i2s-rk817-hifi rk817-hifi-0 [fe410000.i2s-rk817-hifi rk817-hifi-0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
card 1: rockchipbt [rockchip,bt], device 0: fe420000.i2s-bt-sco-pcm bt-sco-pcm-0 [fe420000.i2s-bt-sco-pcm bt-sco-pcm-0]
  Subdevices: 1/1
  Subdevice #0: subdevice #0
e_number test-mic.wav^Cd --duration=5 --format=dat --device=hw:card_number,device
root@rockchip:/# arecord --duration=5 --format=dat --device=hw:0,0 test-mic.wav
Recording WAVE 'test-mic.wav' : Signed 16 bit Little Endian, Rate 48000 Hz, Stereo
root@rockchip:/# arecord --duration=5 --format=dat --device=hw:Loopback,1 test-mic.wav
Recording WAVE 'test-mic.wav' : Signed 16 bit Little Endian, Rate 48000 Hz, Stereo
root@rockchip:/# ls
bin               info        media    proc           sys           usr
build.prop        init        misc     rockchip-test  system        var
busybox.fragment  lib         mnt      root           test-mic.wav  vendor
data              lib64       oem      run            tmp
dev               linuxrc     opt      sbin           udisk
etc               lost+found  persist  sdcard         userdata
root@rockchip:/# aplay test-mic.wav
Playing WAVE 'test-mic.wav' : Signed 16 bit Little Endian, Rate 48000 Hz, Stereo

aplay --channels=1 --format=S16_LE --rate=8000 --device=plughw:1,0 - | arecord --duration=5 --channels=1 --format=S16_LE --rate=8000 --device=plughw:1,0 test-mic.wav

aplay --channels=1 --format=S16_LE --rate=8000 --device=plughw:1,0 test-mic.wav

arecord --channels=1 --format=S16_LE --rate=8000 --device=plughw:1,0 test-mic.wav

aplay --channels=1 --format=S16_LE --rate=8000 --device=plughw:0,0 test-mic.wav

arecord --channels=1 --format=S16_LE --rate=8000 --device=plughw:1,0 - | aplay --channels=1 --format=S16_LE --rate=8000 --device=plughw:0,0 -


pw-record  - | pw-play -


pactl list sinks short
pactl set-default-sink 69

amixer -c 0 cset numid=1 3
amixer -c 0 cset numid=2 1
```



wpctl命令 用于查看设置音频输入输入设备等

```sh
root@rockchip:/# wpctl --help
Usage:
  wpctl [OPTION?] COMMAND [COMMAND_OPTIONS] - WirePlumber Control CLI

Commands:
  status
  get-volume ID
  inspect ID         ## 查看ID信息
  set-default ID     ## 设置默认音频通道
  set-volume ID VOL[%][-/+]
  set-mute ID 1|0|toggle
  set-profile ID INDEX
  clear-default [ID]

Help Options:
  -h, --help       Show help options

```

