# android 抓取hcilog

```
setprop persist.bluetooth.btsnoopenable true
setprop persist.bluetooth.btsnooplogmode full
setprop persist.bluetooth.btsnoopdefaultmode full
setprop persist.bluetooth.btsnoopsize 0xfffffff
setprop persist.bluetooth.btsnooppath /data/misc/bluetooth/logs/btsnoop_hci.log


settings get secure bluetooth_address   //获取蓝牙mac

adb shell svc bluetooth enable

```

