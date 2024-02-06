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
    21  iperf3 -c 192.168.122.1 -i 1 -t 30 -P 4
    22  iperf3 -c 192.168.122.255 -i 1 -t 30 -P 4
    23  iperf3 -s -i 1
    24  history
   





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

