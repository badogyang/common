### 1)使用alsa中的amixer

设置绝对音量，只要大于0即可，一般限制为0-100.如设置为50，则:

```
amixer set -c 0 Master  50
```

如果报错为找不到Master，则执行如下命令打开可视化界面来修改默认声卡:

```
alsamixer
```

按F6来选择在使用的声卡，波动鼠标中的滚轮，观察音柱会不会变化，，如果变化了，就是要用的声卡，切换为该声卡即可。

若执行上述命令时，出现mixer cannot found时，请使用第二个方法。

### 2 使用pactl

设置绝对音量，0%-100%,1表示声卡号。

```
pactl set-sink-volume 0 40%
```

设置相对音量，增大10%

```
pactl set-sink-volume 1 +10%
```

设置相对音量，减小10%

```
pactl set-sink-volume 1 -10%
```

### 3总结

一般alsa就很强大了，在pc上通用，如果在嵌入式设备如jetson nano上alsa出现设备找不到的情况时，可以使用pactl方法。

```
https://git-master.quectel.com/wifi.bt/fc6xe/-/tree/master/BT/Linux/tool/NvmUtility
```























































