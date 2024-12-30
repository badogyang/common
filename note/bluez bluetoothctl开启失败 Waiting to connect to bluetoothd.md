

# bluez bluetoothctl开启失败 Waiting to connect to bluetoothd...



noah@noah-OptiPlex-7090:~/project/bluez$ bluetoothctl
**Waiting to connect to bluetoothd...**



解决方法

 systemctl status bluetooth 查看蓝牙状态

![image-20230328115549917](./img/image-20230328115549917.png)

尝试启动蓝牙

 systemctl start bluetooth

![image-20230328115647253](./img/image-20230328115647253.png)

提示我们重新加载

systemctl daemon-reload

![image-20230328120003834](./img/image-20230328120003834.png)

重新加载后再次启动一下蓝牙后成功解决

![image-20230328115901121](./img/image-20230328115901121.png)



# Failed to set power on: org.bluez.Error.Blocked

rfkill unblock bluetooth

 hciconfig hci0 up



# D-Bus setup failed: Connection ":1.248" is not allowed to own the service "org.bluez" due to security policies in the configuration file

sudo vim /etc/dbus-1/system.d/avahi-dbus.conf

<allow own="org.bluez"/>

![e46aa73fd7ce990d0d9156c81462e3d](./img/e46aa73fd7ce990d0d9156c81462e3d.png)

![b5206fd5a793a4dbc53d3bb631e73c0](./img/b5206fd5a793a4dbc53d3bb631e73c0.png)
