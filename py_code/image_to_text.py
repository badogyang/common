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
text = pytesseract.image_to_string(image, lang='chi_sim')  # lang参数指定识别的语言，可按需修改

print(text)

print('end')