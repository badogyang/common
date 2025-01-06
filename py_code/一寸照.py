# 在这里，您可以通过 ‘args’  获取节点中的输入变量，并通过 'ret' 输出结果
# 'args' 和 'ret' 已经被正确地注入到环境中
# 下面是一个示例，首先获取节点的全部输入参数params，其次获取其中参数名为‘input’的值：
# params = args.params; 
# input = params.input;
# 下面是一个示例，输出一个包含多种数据类型的 'ret' 对象：
# ret: Output =  { "name": ‘小明’, "hobbies": [“看书”, “旅游”] };

import requests
import json
import base64
import os
import websocket
import uuid
import time
from PIL import Image
import io

class ComfyUIAPI:
    def __init__(self, host="127.0.0.1", port=8188):
        self.host = host
        self.port = port
        self.server_address = f"https://e2ff-221-234-179-183.ngrok-free.app/"
        self.ws_address = f"ws://e2ff-221-234-179-183.ngrok-free.app/ws"
        
    def _encode_image_to_base64(self, image_path):
        with open(image_path, 'rb') as image_file:
            encoded_string = base64.b64encode(image_file.read()).decode('utf-8')
        return encoded_string

    def _upload_image(self, image_path):
        # 上传图片到ComfyUI服务器
        encoded_image = self._encode_image_to_base64(image_path)
        filename = os.path.basename(image_path)
        
        response = requests.post(
            f"{self.server_address}/upload/image",
            files={
                'image': (filename, open(image_path, 'rb'), 'image/jpeg')
            }
        )
        
        if response.status_code == 200:
            return filename
        else:
            raise Exception(f"Failed to upload image: {response.text}")

    def generate_image(self, source_image_path, client_id):
        # 上传源图片和姿势图片
        # source_image_name = self._upload_image(source_image_path)
        # pose_image_name = self._upload_image(pose_image_path)
        
        # 读取工作流配置
        with open('一寸照_workflow.json', 'r', encoding='utf-8') as f:
            workflow = json.load(f)
            
        # 更新工作流中的图片路径
        # workflow["14"]["inputs"]["image"] = source_image_name
        # workflow["29"]["inputs"]["image"] = pose_image_name
        
        # 生成prompt ID
        # prompt_id = str(uuid.uuid4())
        
        # 发送请求到ComfyUI
        data = {
            "prompt": workflow,
            "client_id": client_id
        }
        
        response = requests.post(f"{self.server_address}/prompt", json=data)
        if response.status_code != 200:
            raise Exception(f"Failed to send prompt: {response.text}")
        
        print(response.text)
        
        # 通过WebSocket监听生成进度
        # ws = websocket.WebSocket()
        # ws.connect(self.ws_address)
        return ""
        # while True:
        #     out = ws.recv()
        #     if out:
        #         message = json.loads(out)
        #         if message['type'] == 'executing':
        #             print(f"Processing node: {message['data']['node']}")
        #         elif message['type'] == 'executed':
        #             if 'output' in message['data']:
        #                 output_images = message['data']['output']['images']
        #                 if output_images:
        #                     # 获取生成的图片
        #                     for image_data in output_images:
        #                         image_url = f"{self.server_address}/view?filename={image_data['filename']}"
        #                         response = requests.get(image_url)
        #                         if response.status_code == 200:
        #                             image = Image.open(io.BytesIO(response.content))
        #                             output_path = f"output_{int(time.time())}.png"
        #                             image.save(output_path)
        #                             print(f"Image saved to: {output_path}")
        #                             return output_path
        #         elif message['type'] == 'execution_error':
        #             raise Exception(f"Execution error: {message}")

def main():
    # 使用示例
    comfy = ComfyUIAPI()
    
    # 设置源图片和姿势图片的路径
    source_image = "D:\资料\截图\微信图片_20250104130548.jpg"  # 替换为你的源图片路径
    client_id = "12345"      # 替换为你的姿势图片路径
    
    try:
        output_image = comfy.generate_image(source_image, client_id)
        print(f"Successfully generated image: {output_image}")
    except Exception as e:
        print(f"Error occurred: {str(e)}")

if __name__ == "__main__":
    main()