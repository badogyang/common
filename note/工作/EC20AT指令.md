# FCS945R执行顺序

```
[2023-09-13_16:52:37:974]at+qwificfg="fcs945r",0,1

[2023-09-13_16:52:41:215]OK
[2023-09-13_16:52:45:610]at+qwificfg="fcs945r",0,0

[2023-09-13_16:52:46:585]OK
[2023-09-13_16:52:48:700]at+qwificfg="fcs945r_ble",0

[2023-09-13_16:52:50:695]OK
[2023-09-13_16:52:51:641]at+qwificfg="fcs945r_ble",1

[2023-09-13_16:52:54:237]send cmd:enable uart5:/dev/ttyHS6
[2023-09-13_16:52:54:237]enable[Success:0]


[2023-09-13_16:52:54:237]OK
[2023-09-13_16:53:00:733]at+qwificfg="fcs945r_ble",9,0

[2023-09-13_16:53:00:778]wlan0    bt_efuse_file:BT efuse file file_read OK



[2023-09-13_16:53:00:778]OK
[2023-09-13_16:53:02:983]at+qwificfg="fcs945r_ble",10

[2023-09-13_16:53:03:028]wlan0    efuse_bt_mask:BT efuse mask file read OK



[2023-09-13_16:53:03:028]OK
[2023-09-13_16:53:05:548]at+qwificfg="fcs945r_ble",17
[2023-09-13_16:53:05:668]DBG=====ble at, BT_MAC:71BAEF3741C4
[2023-09-13_16:53:05:668]OK
```



展锐

```
/etc/init.d/fcs950u_drv start
iwnpi wlan0 get_mac_efuse

EC20版本需要ql_passwd.sh生成密码
```

