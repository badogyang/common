````


抓取CP2 日志，请尝试如下方法：

开机后输入如下命令，
touch /data/unisoc_cp2log_config.txt
chmod 777 /data/unisoc_cp2log_config.txt
echo "wcn_cp2_log_limit_size=500M;\n" > /data/unisoc_cp2log_config.txt
echo "wcn_cp2_file_max_num=20;\n" >> /data/unisoc_cp2log_config.txt
echo "wcn_cp2_file_cover_old=true;\n" >> /data/unisoc_cp2log_config.txt
echo "wcn_cp2_log_path="/data/unisoc_dbg";\n" >> /data/unisoc_cp2log_config.txt
以上命令输入一次即可

以下命令请在每次开机后输入
rm /data/unisoc_dbg/*
mkdir /data/unisoc_dbg
chmod 777 /data/unisoc_dbg
touch unisoc_cp2log_0.txt
chmod 777 /data/unisoc_dbg/*
echo "at+armlog=1\r" > /proc/mdbg/at_cmd
echo "at+armlog=1\r" > /proc/mdbg/at_cmd
echo "at+armlog=1\r" > /proc/mdbg/at_cmd

cat /dev/slog_wcn0 >/data/cp2.log

settings get secure bluetooth_address   //获取蓝牙mac

adb shell svc bluetooth enable
````





iwnpi工具使用

```
iwnpi wlan0 start
iwnpi wlan0 get_mac_efuse
```

