import pandas as pd
import struct
import os

def update_binary_files_from_excel(input_excel):
    try:
        # 读取 Excel 文件
        df = pd.read_excel(input_excel)
        if 'filename' not in df.columns or 'mac_address' not in df.columns :
            raise ValueError("Excel 表格中必须包含 'filename', 'mac_address' 和 'offset' 三列")
    except Exception as e:
        print(f"读取 Excel 文件失败: {e}")
        return

    for _, row in df.iterrows():
        try:
            # 获取 Excel 行数据
            filename = row['filename']
            mac_address = row['mac_address']
            offset = 0x3b2

            # 检查 filename 是否为空或 NaN
            if pd.isna(filename):
                print("文件名为空，跳过该行")
                continue

            # 将 MAC 地址转换为二进制格式
            try:
                mac_bytes = bytes(int(x, 16) for x in mac_address.split(':'))
            except ValueError:
                print(f"MAC 地址格式错误：{mac_address}，跳过该行")
                continue

            # 检查文件是否存在
            if not os.path.exists(filename):
                print(f"文件未找到：{filename}，跳过该行")
                continue

            # 打开二进制文件并定位到偏移
            with open(filename, 'r+b') as bin_file:
                bin_file.seek(0xee)  #定位到VID
                bin_file.write(bytes([0x7c, 0x2c]))

                bin_file.seek(0xf0)   #定位到PID
                bin_file.write(bytes([0x09, 0x70]))

                bin_file.seek(0x3b2)  # 定位到偏移位置
                bin_file.write(mac_bytes)  # 写入 MAC 地址
                print(f"成功更新文件 '{filename}' 的 MAC 地址，偏移位置：0x3b2")
        except Exception as e:
            print(f"处理文件 '{filename}' 时发生错误: {e}")

# 示例调用
input_excel_path = "20241120_手动写号校准记录表.xlsx"  # 替换为你的 Excel 文件路径
update_binary_files_from_excel(input_excel_path)
