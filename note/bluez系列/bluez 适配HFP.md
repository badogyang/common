bluez+ofono+pulseaudio+alsa 适配HFP



ALSA 是 Advanced Linux Sound Architecture 的缩写，即高级 Linux声音架构,在 Linux 操作系统上提供了对音频和 MIDI（Musical InstrumentDigital Interface，音乐设备数字化接口）的支持。在 Linux2.6 版本内核以后，ALSA 已经成为默认的声音子系统，用来替换 2.4 版本内核中的OSS（Open Sound System，开放声音系统）。



[关于linux：Pulseaudio为A2DP和HFP连接设置接收器和源 | 码农家园 (codenong.com)](https://www.codenong.com/52311637/)

[【嵌入式流媒体开发】Linux ALSA 声卡数据采集与播放_alsa 查看声卡_与光同程的博客-CSDN博客](https://blog.csdn.net/yy197696/article/details/122789698)



操作步骤

```shell
#步骤1
mount -o remount rw /
/etc/init.d/S46ofono stop
sleep 0.5
echo 0 > /sys/class/rfkill/rfkill0/state
sleep 0.5
echo 1 > /sys/class/rfkill/rfkill0/state
sleep 0.5
insmod /usr/lib/modules/hci_uart.ko
sleep 0.5
rtk_hciattach -n -s 115200 ttyS1 rtk_h5 &

#步骤2
/usr/libexec/bluetooth/bluetoothd -n -d &

#步骤3
pulseaudio -D  --exit-idle-time=-1  --log-target=file:/tmp/pulse.log
ofonod --debug=DEBUG --plugin=hfp_bluez5 &

#步骤4
bluetoothctl

#步骤5
pactl list sinks
pactl set-default-sink 2

amixer -c 0 cset numid=1 3

#步骤6     hfp需要
pactl list cards
pactl set-card-profile bluez_card.44_71_47_1F_EA_B4 headset_audio_gateway
arecord -Dhw:0,0 -d 60 -f cd -r 44100 -c 2 -t wav test.wav   #必须执行这一步才不会有电流声
arecord -D bluealsa:HCI=hci0,DEV=44:71:47:1F:EA:B4,PROFILE=sco /as.wav
```

