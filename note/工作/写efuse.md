```
insmod /system/lib/modules/8723ds.ko rtw_rfe_type=02
ifconfig wlan0 up
sleep 0.5
rtwpriv wlan0 mp_start 
//查看efuse剩余空间
rtwpriv wlan0 efuse_get ableraw
//load map和mask
rtwpriv wlan0 efuse_file /system/etc/wifi/FCS945R_wl_efuse.map
rtwpriv wlan0 efuse_mask /system/etc/wifi/FCS945R_wl_efuse.mask

rtwpriv wlan0 efuse_set wldumpfake
rtwpriv  wlan0 efuse_set wlwfake,1d8,1111 

3. rtwpriv wlan0 efuse_get wlrfkmap  
4. rtwpriv wlan0 efuse_set wlfk2map     //确认修改后，将fake map写入efuse
5. rtwpriv wlan0 efuse_get realmap     //查看efuse内容，看0x400是否已修改


rtlbtmp 
enable uart5:/dev/ttyS1
quit

rtwpriv wlan0 bt_efuse_file  /vendor/xx/bt_efuse.map
rtwpriv wlan0 efuse_bt_mask /vendor/XX/bt_efuse.mask

rtwpriv wlan0 efuse_set btwfake,ed,01  (8852开始不再使用btwmap 写真实efuse的方式)
rtwpriv wlan0 efuse_get btrfkmap  (查看目前的fake map)
rtwpriv wlan0 efuse_set btfk2map (寫入HW EFUE)
rtwpriv wlan0 efuse_get btrealmap (讀出HW EFUSE)


rtwpriv wlan0 mp_start
rtwpriv wlan0 efuse_mask off
rtwpriv wlan0 efuse_get realmap
rtwpriv wlan0 efuse_get btfmap
rtwpriv wlan0 efuse_get btbmap


rtw_tx_pwr_lmt_enable=0 rtw_rfe_type=02
insmod /system/lib/modules/8723ds.ko 
sleep 0.5
ifconfig wlan0 up
sleep 0.5
rtwpriv wlan0 mp_start     
sleep 0.5
rtwpriv wlan0 mp_setrfpath 0          
sleep 0.5
rtwpriv wlan0 mp_ant_tx b            
sleep 0.5
echo 0 > /sys/class/rfkill/rfkill0/state
echo 0 > /proc/bluetooth/sleep/btwrite
sleep 0.5
echo 1 > /sys/class/rfkill/rfkill0/state
echo 1 > /proc/bluetooth/sleep/btwrite
sleep 0.5
insmod /usr/lib/modules/hci_uart.ko

rtlbtmp 
enable uart5:/dev/ttyS1


rtwpriv wlan0 efuse_set btwmap,ED,09
rtwpriv wlan0 efuse_get btfmap


AP 

insmod /system/lib/modules/8851bu.ko rtw_country_code=CN
create_ap.sh -m 11ax -w 40M -c 6 --ssid fcu856r_2.4G
 create_ap.sh -m 11ax -w 80M -c 36 --ssid fcu856r_5G
stinsmod_ko.sh -m fcu865r -c US

create_ap.sh -m 11n -c 6 -w 20M --ssid "softap_test" --nss 2

create_ap.sh -m 11ax -c 6 -w 40M --ssid "softap_test_noah" --nss 2

手机要下载一个Magic iperf的软件
手机侧开 iperf3 -s -i 1
模组 iperf3 -c 192.168.11.9 -i 1 -P 4 -t 50    #手机ip iperf会显示

while true; do cat /proc/net/$(ls /proc/net/|grep rtl)/wlan0/btcoex; sleep 2; done;

echo btc show > /proc/net/$(ls /proc/net/|grep rtl)/wlan0/odm/phl_cmd

cat /proc/net/$(ls /proc/net/|grep rtl)/wlan0/odm/phl_cmd


at_server /dev/ttyGS0

make realtek-dirclean && make realtek-rebuild

热插拔
cat /sys/kernel/debug/mmc2/remove
拔掉已测模组，再插上待测模组
cat /sys/kernel/debug/mmc2/rescan

rtwpriv wlan0 efuse_get btfmap  查看efuse
rtwpriv wlan0 efuse_get btbmap

rtwpriv wlan0 efuse_get btrealmap


/usr/libexec/bluetooth/bluetoothd -n &
mount -o remount rw /
/etc/init.d/S50pipewire start
sleep 0.5
insmod /usr/lib/modules/hci_uart.ko
sleep 0.5
echo 0 > /sys/class/rfkill/rfkill0/state
echo 0 > /proc/bluetooth/sleep/btwrite
sleep 0.5
echo 1 > /sys/class/rfkill/rfkill0/state
echo 1 > /proc/bluetooth/sleep/btwrite
sleep 0.5
rtk_hciattach -n -s 115200 ttyS1 rtk_h5 &

pactl set-default-sink 50

amixer -c 0 cset numid=1 3
amixer -c 0 cset numid=2 1
```





rtwpriv wlan0 mp_start

//查看efuse剩余空间
rtwpriv wlan0 efuse_get ableraw

//load map和mask
rtwpriv wlan0 efuse_file /system/etc/wifi/FCS945R_wl_efuse.map
rtwpriv wlan0 efuse_mask /system/etc/wifi/FCS945R_wl_efuse.mask

//查看fake map
rtwpriv wlan0 efuse_get wlrfkmap
rtwpriv wlan0 efuse_get wlrfkmap
rtwpriv wlan0 efuse_get wlrfkmap

//写MAC地址，先修改再写
rtwpriv wlan0 efuse_set wlwfake,400,ec1d9e4016a2

//查看0x400地址是否已修改
rtwpriv wlan0 efuse_get wlrfkmap
rtwpriv wlan0 efuse_get wlrfkmap
rtwpriv wlan0 efuse_get wlrfkmap

//确认修改后，将fake map写入efuse
rtwpriv wlan0 efuse_set wlfk2map  

//查看efuse剩余空间
rtwpriv wlan0 efuse_get ableraw

//查看efuse内容，看0x400是否已修改
rtwpriv wlan0 efuse_get realmap
rtwpriv wlan0 efuse_get realmap
rtwpriv wlan0 efuse_get realmap

重启，加载驱动，查看mac地址是否变化