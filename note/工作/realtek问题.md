HFP

蓝牙固件需要模式是sco over hci，需要改成走PCM，原厂改，需要提供如下信息

1.	Frame-sync frequency(由PCM master 决定,在PCM I/F Setting 3 里面设置)
2.	Frame-sync起始位置，长度，比如：frame-sync rises at bclk falling还是bclk rising，长度1 bclk or 2 bclks..... (由PCM master 决定, 在PCM I/F Setting 3 里面设置)
3.	Drive/Latch data的起始位置(这里主要指: lat_pos, drv_pos, lat_offset)
4.	bit clock (可以推算出2个frame-sync之间的slots数量)
5.	MSB first还是LSB first
6.	Who is the PCM master
这些参数

默认

<img src="./img/a59124d3b41c8f1673258b54154839c.png" alt="a59124d3b41c8f1673258b54154839c" style="zoom: 80%;" />



硬件流控相关

```
Realtek Bluetooth :Start downloading...
Realtek Bluetooth :Send additional packet 32
Realtek Bluetooth :Send additional packet 33
Realtek Bluetooth :Send additional packet 34
Realtek Bluetooth :Last packet 163
Realtek Bluetooth :Send last pkt
Realtek Bluetooth :Enable host hw flow control
Realtek Bluetooth :h5_hci_reset: Issue hci reset cmd
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth WARN: OP_HCI_RESET Transmission timeout
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth ERROR: Out-of-order packet arrived, got(6)expected(7)
Realtek Bluetooth WARN: OP_HCI_RESET Transmission timeout
Realtek Bluetooth ERROR: Retransmission exhausts
```