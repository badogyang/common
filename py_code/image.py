import time
from PIL import ImageGrab   # 截图

time.sleep(3)
# 截图
img = ImageGrab.grab()
# print(img.size) # 960 540 480 270
#img = img.resize((960, 540))
# 保存图片
img.save(f'imgs/{str(time.time())}.jpg')
# 修改name
time.sleep(0.1)
