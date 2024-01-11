# ubuntu 编译安装Bluez



## 1. 获取源码

git clone git://git.kernel.org/pub/scm/bluetooth/bluez.git 

git clone git://git.kernel.org/pub/scm/libs/ell/ell.git

### 获取芯片patch(公司提供的压缩包)  

1) bluez$ git am ../0001-hciattach-add-support-for-qualcomm-chip.patch

## 2. 编译

1. ./bootstrap-configure --disable-android --disable-midi  --prefix=$PWD/out  

2. 根据错误提示安装对应软件包

   1.error: GLib >= 2.28 is required

   sudo apt install libglib2.0-dev

   2.error: D-Bus >= 1.6 is required

   sudo apt install libdbus-1-dev

   3.error: libudev >= 172 is required

   sudo apt install libudev-dev

   4.error: libical is required

   sudo apt install libical-dev

   5.error: readline header files are required

   sudo apt install libreadline-dev

   6.error: rst2man is required

   sudo apt install python-docutils
   sudo which rst2man

   sudo apt-get install libdw-dev

   

   ./bootstrap: 5: libtoolize: not found

   

   这一步会出现一些包找不到的问题，按部就班给他解决就行；还出现了一个json-c版本过低的问题，解决方法

   ```shell
   
   wget https://s3.amazonaws.com/json-c_releases/releases/json-c-0.13.1.tar.gz
   
   tar xvf json-c-0.13.1.tar.gz
   cd json-c-0.13.1/
   
   ./configure --prefix=/usr --disable-static
   
   make
   sudo make install
   ```

   

3. $ make 

4. $ sudo make install



## 3. 烧录

### 3.1 添加固件

**步骤一：**     将AH20C蓝牙固件包中的BCM20703A1.hcd拷贝至Linux系统/*etc/firmware/*路径下；(固件路径：可能芯片固件会有所不同，)

**步骤二：**     执行*$ls /etc/firmware/ BCM20703A1.hcd*查看固件是否拷贝成功。

### 3.2 烧录固件

sudo hciattach /dev/ttyUSB0 bcm43xx -t120 3000000 flow  (一般只替换芯片平台，bcmxx/qca，注意看自己的usb，使用sudo ls /dev/ttyUSB* 查看)

这一步我这边输出
bcm43xx_init
Failed to write reset command
Can't initialize device: Success

后续测试的时候好像也可以

### 3.3 验证





