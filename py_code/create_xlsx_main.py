# author: noah.yang

import os
import pandas as pd
import re
from datetime import datetime

def mac_increment(mac_address, increment):
    """
    将 MAC 地址自增指定的值
    :param mac_address: 起始 MAC 地址（格式为 "XX:XX:XX:XX:XX:XX"）
    :param increment: 自增值
    :return: 自增后的 MAC 地址
    """
    # 去除 MAC 地址中的分隔符
    cleaned_mac = re.sub(r'[^a-fA-F0-9]', '', mac_address)
    if len(cleaned_mac) != 12:
        raise ValueError(f"无效的 MAC 地址: {mac_address}")
    
    # 将 MAC 地址转换为整数
    mac_int = int(cleaned_mac, 16)
    
    # 自增
    mac_int += increment
    
    # 将整数转换回 MAC 地址格式
    new_mac = f"{mac_int:012X}"  # 转换为 12 位十六进制字符串
    new_mac = ":".join([new_mac[i:i+2] for i in range(0, 12, 2)])  # 添加冒号分隔符
    return new_mac

def create_excel_with_bin_files(output_excel, start_mac):
    """
    读取当前目录下的所有 .bin 文件，并将文件名和自增的 MAC 地址写入 Excel 表格
    :param output_excel: 输出的 Excel 文件名
    :param start_mac: 起始 MAC 地址
    """
    # 获取当前目录下的所有 .bin 文件
    bin_files = [f for f in os.listdir('.') if f.endswith('.bin')]
    bin_files.sort()  # 按文件名排序
    
    # 创建数据字典
    data = {
        '序号': [],
        'filename': [],
        'BT_MAC': [],
        'WIFI_MAC': [],
        '备注': []
    }
    count = len(bin_files)
    
    # 填充数据
    for i, file_name in enumerate(bin_files):
        data['序号'].append(i+1)
        data['filename'].append(file_name)
        data['BT_MAC'].append(mac_increment(start_mac, i))
        data['WIFI_MAC'].append(mac_increment(start_mac, count+i))
    
    data['备注'].append('wifi vid: 1EAC pid: 8005')
    data['备注'].append('BT VID:2C7C PID:7009')
    data['备注'].append('号段：AC:D9:29:C6:E2:F2 - C6:E3:0F')
    # 创建 DataFrame 并保存到 Excel
    df = pd.DataFrame(data)
    df.to_excel(output_excel, index=False)
    print(f"已成功将数据写入 {output_excel}")

# 示例调用
if __name__ == "__main__":
     # 获取当前日期，格式为 YYYYMMDD
    current_date = datetime.now().strftime("%Y%m%d")
    
    # 设置输出 Excel 文件名
    output_excel = f"{current_date}FME175T写号记录表.xlsx"
    start_mac = "AC:D9:29:C6:E2:F2"  # 起始 MAC 地址
    create_excel_with_bin_files(output_excel, start_mac)