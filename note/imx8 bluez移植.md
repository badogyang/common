imx8 bluez移植







source /opt/fsl-imx-xwayland/5.10-gatesgarth/environment-setup-cortexa53-crypto-poky-linux



./bootstrap-configure --disable-android --disable-midi --prefix=$PWD/out --host=aarch64-poky-linux CC=aarch64-poky-linux-gcc CFLAGS="--sysroot=/opt/fsl-imx-xwayland/5.10-gatesgarth/sysroots/cortexa53-crypto-poky-linux"

这里可能会编译报错，我们直接把报错的地方给屏蔽



vim configure.ac

![image-20230316194829250](./img/image-20230316194829250.png)