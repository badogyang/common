接上一章[一个从读取excel表格里面的文件名和mac地址打开对应文件写入对应地址的脚本](https://blog.csdn.net/qq_37182906/article/details/144848975?fromshare=blogdetail&sharetype=blogdetail&sharerId=144848975&sharerefer=PC&sharesource=qq_37182906&sharefrom=from_link)，加了一个读取当前目录文件名，写入excel表格，并给对应分拣分配mac的脚本，这样可以配合后面的使用，自动化处理
需求：

1. **读取当前目录下的所有 `.bin` 文件**，并将文件名写入 Excel 表格的“文件名”一栏。
2. **MAC 地址栏**：从给定的起始 MAC 地址依次自增 1，并写入 Excel 表格的“MAC地址”一栏。

以下是实现这个需求的代码：

---

### **实现步骤**
1. 使用 `os` 模块获取当前目录下的所有 `.bin` 文件。
2. 使用 `pandas` 创建一个 Excel 表格，并将文件名写入“文件名”一栏。
3. 实现 MAC 地址自增逻辑，并将结果写入“MAC地址”一栏。
4. 将数据保存到 Excel 文件中。

---

### **代码实现**

```python
import os
import pandas as pd
import re

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
        '文件名': [],
        'MAC地址': []
    }
    
    # 填充数据
    for i, file_name in enumerate(bin_files):
        data['文件名'].append(file_name)
        data['MAC地址'].append(mac_increment(start_mac, i))
    
    # 创建 DataFrame 并保存到 Excel
    df = pd.DataFrame(data)
    df.to_excel(output_excel, index=False)
    print(f"已成功将数据写入 {output_excel}")

# 示例调用
if __name__ == "__main__":
    output_excel = "output.xlsx"  # 输出的 Excel 文件名
    start_mac = "00:00:00:00:00:00"  # 起始 MAC 地址
    create_excel_with_bin_files(output_excel, start_mac)
```

---

### **代码说明**
1. **`mac_increment` 函数**：
   - 将 MAC 地址转换为整数，自增指定的值，然后转换回 MAC 地址格式。
   - 支持 MAC 地址的自增逻辑，例如 `00:00:00:00:00:00` 自增 1 后变为 `00:00:00:00:00:01`。
2. **`create_excel_with_bin_files` 函数**：
   - 使用 `os.listdir` 获取当前目录下的所有 `.bin` 文件。
   - 使用 `pandas` 创建 Excel 表格，并将文件名和自增的 MAC 地址写入表格。
   - 保存 Excel 文件。

---

### **示例输出**

#### **当前目录下的文件**
```
file1.bin
file2.bin
file3.bin
```

#### **生成的 Excel 文件 (`output.xlsx`)**
| 文件名    | MAC地址           |
| --------- | ----------------- |
| file1.bin | 00:00:00:00:00:00 |
| file2.bin | 00:00:00:00:00:01 |
| file3.bin | 00:00:00:00:00:02 |

---

### **依赖库**
- `pandas`：用于创建和保存 Excel 文件。
- `openpyxl`：`pandas` 的依赖库，用于处理 Excel 文件。

安装依赖库：
```bash
pip install pandas openpyxl
```

---

### **注意事项**
1. **MAC 地址格式**：
   - 起始 MAC 地址需要符合标准格式（如 `XX:XX:XX:XX:XX:XX`）。
   - 如果 MAC 地址自增后超过 `FF:FF:FF:FF:FF:FF`，会继续从 `00:00:00:00:00:00` 开始循环。
2. **文件排序**：
   - 文件名按字母顺序排序，如果需要其他排序方式，可以调整 `bin_files.sort()` 的逻辑。