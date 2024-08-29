# 概述

PipeWire 是一个新的底层多媒体框架。 它旨在以最低的延迟为音频和视频提供录制和播放功能，并支持基于 PulseAudio、JACK、ALSA 和 GStreamer 的应用程序。

基于该框架的守护进程可以配置为音频服务器(具有 PulseAudio 和 JACK 特性)和视频录制服务器。

PipeWire 还支持像 Flatpak 这样的容器，不依赖于 audio 和 video 用户组。 相反，它采用了类似于 Polkit的安全模式，向 Flatpak 或 Wayland 请求许可以录制屏幕或音频。

