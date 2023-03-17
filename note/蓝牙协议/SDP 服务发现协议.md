# SDP 服务发现协议

服务发现协议(Service Discovery protocol, SDP)为应用程序提供了一种方法，用于发现哪些服务可用，并确定这些可用服务的特征。

# 概述

### SDP CLIENT-SERVER ARCHITECTURE  

SDP使用client-server架构，如下图所示

![image-20230222193710656](D:/%E8%B5%84%E6%96%99/common-master/note/%E8%93%9D%E7%89%99%E5%8D%8F%E8%AE%AE/img/image-20230222193710656.png)

服务发现机制为客户端应用程序提供了发现服务器应用程序提供的服务是否存在以及这些服务的属性的方法。服务的属性包括所提供服务的类型或类别，以及利用服务所需的机制或协议信息。

