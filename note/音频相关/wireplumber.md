

![image-20240813165756474](./img/image-20240813165756474.png)



```shell
root@rockchip:/# wpctl --help
Usage:
  wpctl [OPTION?] COMMAND [COMMAND_OPTIONS] - WirePlumber Control CLI

Commands:
  status
  get-volume ID
  inspect ID         ## 查看ID信息
  set-default ID     ## 设置默认音频通道
  set-volume ID VOL[%][-/+]
  set-mute ID 1|0|toggle
  set-profile ID INDEX
  clear-default [ID]

Help Options:
  -h, --help       Show help options

Pass -h after a command to see command-specific options

root@rockchip:/# wpctl get-volume 32
Volume: 0.40
root@rockchip:/# wpctl set-volume 32 10%  #设置音量
root@rockchip:/# wpctl get-volume 32
Volume: 0.10
root@rockchip:/# wpctl set-volume 32 50%

```



pactl list sinks short
pactl set-default-sink 51

amixer -c 0 cset numid=1 3
amixer -c 0 cset numid=2 1
