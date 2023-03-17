# ubuntu 编译安装Bluez



## 1. 获取源码

git clone git://git.kernel.org/pub/scm/bluetooth/bluez.git 

### 获取芯片patch(公司提供的压缩包)  

1) bluez$ git am ../patches/0001-bluetooth-Add-bluetooth-support-for-QCA6174-chip.patch
2) bluez$ git am ../patches/0002-hciattach-set-flag-to-enable-HCI-reset-on-init.patch
3) bluez$ git am ../patches/0003-hciattach-instead-of-strlcpy-with-strncpy-to-avoid-r.patch
4) bluez$ git am ../patches/0004-Add-support-for-Tufello-1.1-SOC.patch
5) bluez$ git am ../patches/0005-bluetooth-Add-support-for-multi-baud-rate.patch
6) bluez$ git am ../patches/0006-bluetooth-Add-support-quectel-FC6X-module.patch

## 2. 编译

1. $ ./bootstrap-configure --disable-android --disable-midi  

   这一步会出现一些包找不到的问题，按部就班给他解决就行；还出现了一个json-c版本过低的问题，解决方法

   ```shell
   
   wget https://s3.amazonaws.com/json-c_releases/releases/json-c-0.13.1.tar.gz
   
   tar xvf json-c-0.13.1.tar.gz
   cd json-c-0.13.1/
   
   ./configure --prefix=/usr --disable-static
   
   make
   sudo make install
   ```

   

2. $ make 

3. $ sudo make install



## 3. 烧录

### 3.1 添加固件

**步骤一：**     将AH20C蓝牙固件包中的BCM20703A1.hcd拷贝至Linux系统/*etc/firmware/*路径下；(固件路径：可能芯片固件会有所不同，)

**步骤二：**     执行*$ls /etc/firmware/ BCM20703A1.hcd*查看固件是否拷贝成功。

### 3.2 烧录固件

sudo hciattach /dev/ttyUSB0 bcmxx -t120 3000000 flow  (一般只替换芯片平台，bcmxx/qca，注意看自己的usb，使用sudo ls /dev/ttyUSB* 查看)

这一步我这边输出
bcm43xx_init
Failed to write reset command
Can't initialize device: Success

后续测试的时候好像也可以

### 3.3 验证





