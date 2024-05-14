# 接口不通，响应超时

![157642c01f12de47fb7187c0b59a8e7](./img/157642c01f12de47fb7187c0b59a8e7.png)

一般是模组问题引起，换一块模组尝试一下



# 高通patch未合入，芯片型号不识别

![4fa5624cffa8c3dd888c6a504d8d8a4](./img/4fa5624cffa8c3dd888c6a504d8d8a4.png)



# Can't init device hci0: Invalid request code (56)

![dddb7f80fe9bc1429c5f233d8709b9b](./img/dddb7f80fe9bc1429c5f233d8709b9b.png)

ubuntu 内核版本问题，从5.15换到5.4后解决



# bluetooth挂了

Failed to set power off: org.freedesktop.DBus.Error.NoReply



bluez 编译报错 ell




## Can't init device hci0: Operation not permitted (1)

hciconfig hci0 up使用这个报错

需要加sudo 权限

![image-20231018104757873](./img/image-20231018104757873.png)




## Realtek Bluetooth ERROR: h5_download_patch: Retransmission exhausts

在EC20 852R陪测版本遇到，固件版本问题，更新固件后解决。

![image-20231102102008727](./img/image-20231102102008727.png)



## Set scan parameters failed: Input/output error

![3e9eea89ac1476c0b9cf3e6222b7ece](./img/3e9eea89ac1476c0b9cf3e6222b7ece.png)

执行 hcitool cmd 0x03 0x0003 可解决

