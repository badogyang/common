```
insmod /usr/lib/modules/3.18.20/extra/hci_uart.ko
echo 0 > /sys/class/rfkill/rfkill1/state
echo 1 > /sys/class/rfkill/rfkill1/state
rtk_hciattach -n -s 115200 ttyHS6 rtk_h5 &   


/usr/lib/bluez5/bluetooth/bluetoothd -n -d &

bluetoothctl
[bluetooth]# power on
[bluetooth]# agent NoInputNoOutput //可以设置其他 IO caps, 如 KeyboardDisplay
[bluetooth]# default-agent
[bluetooth]# discoverable on //开发可被发现
[bluetooth]# scan on //扫描到对应的设备后，使用 scan off 关闭 scan。
[bluetooth]# pair e0:08:71:1a:de:31 //配对远端设备，也可在手机上搜索到设备之后点击配对，注意需要使用苹果设备，安卓的目前无法配对。

```

