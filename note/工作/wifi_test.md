    1  ifconfig
    2  insmod /system/lib/modules/8852bs.ko
    3  ifconfig
    4  ifconfig wlan0 up
    5  find ./ -name wpa_supplicant.conf
    11  vi wpa.conf
    12  wpa_supplicant -i wlan0 -D nl80211 -c wpa.conf &
    13  udhcpc -i wlan0 &
    14  wpa_cli status
    19  iw wlan0 link
    20  iperf3 -s -i 1
    21  iperf3 -c 192.168.11.1 -i 1 -t 30 -P 4
    22  iperf3 -c 192.168.11.255 -i 1 -t 30 -P 4
    23  iperf3 -s -i 1
    24  history
    
    AP 
    
    stinsmod_ko.sh -m fcu865r -c US
    
    create_ap.sh -m 11ax -c 6 -w 20M --ssid "softap_testnoah" --nss 2
    
    create_ap.sh -m 11n -c 6 -w 40M --ssid "softap_testnoah" --nss 2
    
    手机要下载一个Magic iperf的软件
    手机侧开 iperf3 -s -i 1
    模组 iperf3 -c 192.168.11.5 -t 30 -i 1 -P 4    #手机ip iperf会显示


​    /usr/libexec/bluetooth/bluetoothd -n &
​    mount -o remount rw /
​    /etc/init.d/S50pipewire start
​    sleep 0.5
​    insmod /usr/lib/modules/rtk_btusb.ko
​    sleep 0.5
​    echo 0 > /sys/class/rfkill/rfkill0/state
​    echo 0 > /proc/bluetooth/sleep/btwrite
​    sleep 0.5
​    echo 1 > /sys/class/rfkill/rfkill0/state
​    echo 1 > /proc/bluetooth/sleep/btwrite
​    sleep 0.5
​    rtk_hciattach -n -s 115200 ttyS1 rtk_h5 &
​    

    pactl list sinks short
    pactl set-default-sink 50
    
    amixer -c 0 cset numid=1 3
    amixer -c 0 cset numid=2 1



```
  bt_sound: bt-sound {
    status = "okay";
    compatible = "simple-audio-card";
    simple-audio-card,format = "dsp_a";
    //simple-audio-card,bitclock-inversion = <1>;
    simple-audio-card,mclk-fs = <256>;
    simple-audio-card,name = "rockchip,bt";
    simple-audio-card,bitclock-master = <&sound2_master>;
    simple-audio-card,frame-master = <&sound2_master>;
    simple-audio-card,cpu {
      sound-dai = <&i2s2_2ch>;
    };
    sound2_master:simple-audio-card,codec {
      sound-dai = <&bt_sco>;
    };
    //simple-audio-card,codec {
    //  sound-dai = <&bt_sco>;
    //};

  };
```




```
# Only WPA-PSK is used. Any valid cipher combination is accepted.
ctrl_interface=/var/run/wpa_supplicant

network={
#Open
        ssid="H3C-5"
        key_mgmt=NONE
#WPA-PSK
#	ssid="Quectel-Customer"
#	proto=WPA RSN
#	key_mgmt=WPA-PSK
#	pairwise=TKIP CCMP
#	group=TKIP CCMP
#	psk="Customer-Quectel"
#WEP
#	ssid="example wep network"
#	key_mgmt=NONE
#	wep_key0="abcde"
#	wep_key1=0102030405
#	wep_tx_keyidx=0
}
```

