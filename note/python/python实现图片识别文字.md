在 Python 中可以使用 `pytesseract` 库结合 `Tesseract OCR` 引擎来识别图片中的文字，以下是一个基本的示例代码，演示如何实现这个功能：

### 安装依赖

首先需要安装 `pytesseract` 库以及 `Tesseract OCR` 引擎（不同操作系统安装方式有所不同）：

- **Windows 系统**：
  - 安装 `pytesseract`：在命令行中运行 `pip install pytesseract`。
  - 安装 `Tesseract OCR`：从官网（https://github.com/UB-Mannheim/tesseract/wiki）下载对应的安装包进行安装，安装完成后，需要配置环境变量，将 `Tesseract OCR` 的安装路径添加到系统环境变量的 `Path` 变量中（例如安装路径为 `C:\Program Files\Tesseract-OCR`，则添加此路径）。
- **Linux 系统（以 Ubuntu 为例）**：
  - 安装 `pytesseract`：在命令行中运行 `pip install pytesseract`。
  - 安装 `Tesseract OCR`：在终端运行 `sudo apt-get install tesseract-ocr`。
- **macOS 系统**：
  - 安装 `pytesseract`：在命令行中运行 `pip install pytesseract`。
  - 安装 `Tesseract OCR`：可以使用 `brew` 工具安装，在终端运行 `brew install tesseract`。

### Python 示例代码

以下是使用 `pytesseract` 识别图片文字的示例代码：

```python
import pytesseract
from PIL import Image

# 打开图片文件（这里假设图片文件名为test_image.png，你可以替换成实际的文件名及路径）
image = Image.open('test_image.png')

# 使用pytesseract识别图片中的文字
text = pytesseract.image_to_string(image, lang='eng')  # lang参数指定识别的语言，这里是英语，可按需修改

print(text)
```

在上述代码中：

1. 首先通过 `PIL`（Python Imaging Library，这里使用的是 `Pillow`，它是 `PIL` 的一个分支，功能更强大且易于维护）库的 `Image.open()` 方法打开需要识别文字的图片文件（示例中文件名假设为 `test_image.png`，你可以根据实际情况替换成对应的文件名及路径）。
2. 然后调用 `pytesseract` 库的 `image_to_string()` 方法，将打开的图片对象传递进去，并通过 `lang` 参数指定识别文字所使用的语言（这里设置为 `eng`，表示英语，你可以根据图片中实际文字的语言来修改这个参数，例如 `chi_sim` 表示简体中文等），该方法会返回识别出来的文字内容，并最终将其打印输出。

### 提高识别准确率的方法

- 图片预处理
  - **灰度化**：如果图片是彩色的，可以先将其转换为灰度图，有时候能提高识别准确率。示例代码如下：



```python
from PIL import Image, ImageOps

image = Image.open('test_image.png')
gray_image = ImageOps.grayscale(image)
text = pytesseract.image_to_string(gray_image, lang='eng')
print(text)
```

- **二值化**：将图片转换为黑白两色，突出文字部分，去除一些背景干扰等，比如：

收起

```python
import pytesseract
import cv2
import numpy as np  # 导入numpy库
from PIL import Image

# 打开图片文件（这里假设图片文件名为test_image.png，你可以替换成实际的文件名及路径）
print('start')

image = Image.open('imgs/game_text.jpg')

image_cv = cv2.cvtColor(np.array(image), cv2.COLOR_RGB2BGR)
_, binary_image = cv2.threshold(image_cv, 127, 255, cv2.THRESH_BINARY)
binary_image_pil = Image.fromarray(cv2.cvtColor(binary_image, cv2.COLOR_BGR2RGB))

# 使用pytesseract识别图片中的文字
text = pytesseract.image_to_string(image, lang='chi_sim+eng')  # lang参数指定识别的语言，可按需修改

print(text)

print('end')
```

上述代码使用了 `opencv` 库先将 `PIL` 图片转换为 `opencv` 格式，进行二值化处理后再转换回 `PIL` 格式用于文字识别。

- 选择合适的语言包和配置参数
  - 根据图片中的文字语言准确选择 `Tesseract OCR` 的语言包，像识别中文和英文混合的文字时，可以尝试使用 `chi_sim+eng` 这样的语言组合（在 `pytesseract` 调用时设置 `lang='chi_sim+eng'`）。
  - 还可以通过 `pytesseract` 的其他参数来调整识别效果，例如设置 `config` 参数来指定一些 `Tesseract OCR` 的配置选项，像 `--psm`（页面分割模式）等，示例如下：

```python
text = pytesseract.image_to_string(image, lang='eng', config='--psm 6')
```

通过以上方法可以在一定程度上提高文字识别的准确率，但不同图片的情况各异，可能需要根据实际情况灵活调整和优化。