# ble

## ble server 发送数据

```
Bluetoothctl
[bluetoothctl] power on
[bluetoothctl] menu advertise
[bluetoothctl] name “gatts”
[bluetoothctl] back
[bluetoothctl] menu gatt
3.注册服务
register-service e2d36f99-8909-4136-9a49-d825508b297b

re4.注册读属性
register-characteristic 0x1234 read
5.注册写属性
register-characteristic 0x5678 write,read,notify


6.启动服务
register-application
7.	开启广播
back
advertise on
8.	手机使用ble软件如nrf connect扫描并连接对应bluez设备
9.	bluez读写验证：
1）	连接成功后,gatts测试需要添加local显示为本地提供的服务，不添加默认显示手机服务
2）	menu gatt
list-attributes local
2）bluez端选择0x1234或0x5678对应的UUID，注意需要添加local,如
select-attribute  local 0x5678
3）手机端选择和bluez select-attribute同样的UUID进行收发验证。
4）write 7 发送数据7

```

