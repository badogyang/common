

## 启动pulseaudio 
pulseaudio -D  --exit-idle-time=-1  --log-target=file:/tmp/pulse.log



## 查看和选择sink端

pactl list sinks
pactl set-default-sink 2

#步骤6
pactl list cards
pactl set-card-profile bluez_card.44_71_47_1F_EA_B4 headset_audio_gateway

## sources端一般会根据音频zids

pactl list sources
pactl set-default-source 5（5需要对应为实际的source#）

amixer -c 0 cset numid=1 3