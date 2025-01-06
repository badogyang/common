# author: noah.yang

import os
import pandas as pd
import re
from datetime import datetime
import struct

jsondata = '{"client_id":"533ef3a3-39c0-4e39-9ced-37d290f371f8","prompt":{"3":{"inputs":{"weight":1,"weight_faceidv2":1,"weight_type":"linear","combine_embeds":"concat","start_at":0,"end_at":1,"embeds_scaling":"V only","model":["17",0],"ipadapter":["17",1],"image":["14",0],"clip_vision":["51",0],"insightface":["53",0]},"class_type":"IPAdapterFaceID","_meta":{"title":"IPAdapter FaceID"}},"7":{"inputs":{"width":512,"height":768,"batch_size":1},"class_type":"EmptyLatentImage","_meta":{"title":"Empty Latent Image"}},"8":{"inputs":{"samples":["11",0],"vae":["12",2]},"class_type":"VAEDecode","_meta":{"title":"VAE Decode"}},"9":{"inputs":{"text":"(worst_quality:1.62) (MajicNegative_V2:1.3),(Earrings:1.6), necklaces, nose rings, accessories, jewelry BadNegAnatomyV1-neg bradhands cartoon, cgi, render, illustration, painting, drawing,vague, unclear","clip":["12",1]},"class_type":"CLIPTextEncode","_meta":{"title":"CLIP Text Encode (Prompt)"}},"11":{"inputs":{"seed":578522269853077,"steps":25,"cfg":6,"sampler_name":"euler","scheduler":"normal","denoise":1,"model":["3",0],"positive":["31",0],"negative":["31",1],"latent_image":["7",0]},"class_type":"KSampler","_meta":{"title":"KSampler"}},"12":{"inputs":{"ckpt_name":"v1-5-pruned-emaonly.safetensors"},"class_type":"CheckpointLoaderSimple","_meta":{"title":"Load Checkpoint"}},"14":{"inputs":{"image":"微信图片_20250103211539.jpg","upload":"image"},"class_type":"LoadImage","_meta":{"title":"Load Image"}},"15":{"inputs":{"text":"1 girl,(blue background:1.5),(smiling:0.15),(Photorealistic:1.2),photography lighting,indoor lighting,best quality,masterpiece,(Facing the camera:1.2),(white shirt:1.5), single portrait,formal wear,depth of field","clip":["12",1]},"class_type":"CLIPTextEncode","_meta":{"title":"CLIP Text Encode (Prompt)"}},"17":{"inputs":{"preset":"FACEID PLUS V2","lora_strength":0.6,"provider":"CUDA","model":["12",0]},"class_type":"IPAdapterUnifiedLoaderFaceID","_meta":{"title":"IPAdapter Unified Loader FaceID"}},"29":{"inputs":{"image":"ff6a57290f3145e299cebae233060b54.jpeg","upload":"image"},"class_type":"LoadImage","_meta":{"title":"Load Image"}},"30":{"inputs":{"images":["40",0]},"class_type":"PreviewImage","_meta":{"title":"Preview Image"}},"31":{"inputs":{"strength":1,"start_percent":0,"end_percent":1,"positive":["15",0],"negative":["9",0],"control_net":["34",0],"image":["40",0]},"class_type":"ControlNetApplyAdvanced","_meta":{"title":"Apply ControlNet"}},"34":{"inputs":{"control_net_name":"sd-controlnet-openpose .safetensors"},"class_type":"ControlNetLoader","_meta":{"title":"Load ControlNet Model"}},"40":{"inputs":{"detect_hand":"enable","detect_body":"enable","detect_face":"disable","resolution":512,"scale_stick_for_xinsr_cn":"enable","image":["29",0]},"class_type":"OpenposePreprocessor","_meta":{"title":"OpenPose Pose"}},"42":{"inputs":{"enabled":true,"swap_model":"inswapper_128.onnx","facedetection":"retinaface_resnet50","face_restore_model":"none","face_restore_visibility":0.8,"codeformer_weight":0.5,"detect_gender_input":"no","detect_gender_source":"no","input_faces_index":"0","source_faces_index":"0","console_log_level":1,"input_image":["8",0],"source_image":["14",0]},"class_type":"ReActorFaceSwap","_meta":{"title":"ReActor 🌌 Fast Face Swap"}},"51":{"inputs":{"clip_name":"CLIP-ViT-H-14-laion2B-s32B-b79K.safetensors"},"class_type":"CLIPVisionLoader","_meta":{"title":"Load CLIP Vision"}},"53":{"inputs":{"provider":"CUDA","model_name":"buffalo_l"},"class_type":"IPAdapterInsightFaceLoader","_meta":{"title":"IPAdapter InsightFace Loader"}},"55":{"inputs":{"similarity_metric":"L2_norm","filter_thresh":100,"filter_best":0,"generate_image_overlay":true,"analysis_models":["56",0],"reference":["14",0],"image":["8",0]},"class_type":"FaceEmbedDistance","_meta":{"title":"Face Embeds Distance"}},"56":{"inputs":{"library":"insightface","provider":"CUDA"},"class_type":"FaceAnalysisModels","_meta":{"title":"Face Analysis Models"}},"58":{"inputs":{"similarity_metric":"L2_norm","filter_thresh":100,"filter_best":0,"generate_image_overlay":true,"analysis_models":["56",0],"reference":["14",0],"image":["42",0]},"class_type":"FaceEmbedDistance","_meta":{"title":"Face Embeds Distance"}},"60":{"inputs":{"filename_prefix":"ComfyUI","images":["58",0]},"class_type":"SaveImage","_meta":{"title":"Save Image"}}}}'

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
    
    data['备注'].append('wifi vid: 1EAC pid: 8005')
    data['备注'].append('BT VID:2C7C PID:7009')
    data['备注'].append('起始MAC：' + mac_increment(start_mac, 0))
    # 填充数据
    for i, file_name in enumerate(bin_files):
        data['序号'].append(i+1)
        data['filename'].append(file_name)
        data['BT_MAC'].append(mac_increment(start_mac, i))
        data['WIFI_MAC'].append(mac_increment(start_mac, count+i))
        if  i > 2:
            data['备注'].append('')


    # 创建 DataFrame 并保存到 Excel
    df = pd.DataFrame(data)
    df.to_excel(output_excel, index=False)
    print(f"已成功将数据写入 {output_excel}")

#  写入value
def update_binary_files_from_excel(input_excel):
    try:
        # 读取 Excel 文件
        df = pd.read_excel(input_excel)
        if 'filename' not in df.columns or 'BT_MAC' not in df.columns or 'BT_MAC' not in df.columns or 'WIFI_MAC' not in df.columns :
            raise ValueError("Excel 表格中必须包含 'filename', 'BT_MAC' 'WIFI_MAC'")
    except Exception as e:
        print(f"读取 Excel 文件失败: {e}")
        return

    for _, row in df.iterrows():
        try:
            # 获取 Excel 行数据
            filename = row['filename']
            bt_mac = row['BT_MAC']
            wifi_mac = row['WIFI_MAC']

            # 检查 filename 是否为空或 NaN
            if pd.isna(filename):
                print("文件名为空，跳过该行")
                continue

            # 将 MAC 地址转换为二进制格式
            try:
                bt_mac_bytes = bytes(int(x, 16) for x in bt_mac.split(':'))
                wifi_mac_bytes = bytes(int(x, 16) for x in wifi_mac.split(':'))
            except ValueError:
                print(f"地址格式错误：bt {bt_mac} , wifi {wifi_mac}，跳过该行")
                continue

            # 检查文件是否存在
            if not os.path.exists(filename):
                print(f"文件未找到：{filename}，跳过该行")
                continue

            # 打开二进制文件并定位到偏移
            with open(filename, 'r+b') as bin_file:
                bin_file.seek(0x10)  #定位到PCIE ID
                bin_file.write(bytes([0x1a, 0x09]))

                bin_file.seek(0x12)   #定位到PID
                bin_file.write(bytes([0x5e, 0x04]))

                bin_file.seek(0x04)  # 定位到偏移位置
                bin_file.write(wifi_mac_bytes)  # 写入 WIFI MAC 地址

                bin_file.seek(0xee)  #定位到VID
                bin_file.write(bytes([0x7c, 0x2c]))

                bin_file.seek(0xf0)   #定位到PID
                bin_file.write(bytes([0x09, 0x70]))

                bin_file.seek(0x3b2)  # 定位到偏移位置
                bin_file.write(bt_mac_bytes)  # 写入 BT 地址
                print(f"成功更新文件 '{filename}'")
        except Exception as e:
            print(f"处理文件 '{filename}' 时发生错误: {e}")

# 示例调用
if __name__ == "__main__":
     # 获取当前日期，格式为 YYYYMMDD
    current_date = datetime.now().strftime("%Y%m%d")
    
    # 设置输出 Excel 文件名
    excel_name = f"{current_date}FME175T写号记录表.xlsx"

    # 获取用户输入的起始 MAC 地址
    start_mac = input("请输入起始 MAC 地址（格式为 XX:XX:XX:XX:XX:XX）：").strip()

    create_excel_with_bin_files(excel_name, start_mac)

    update_binary_files_from_excel(excel_name)