// 在这里，您可以通过 ‘params’  获取节点中的输入变量，并通过 'ret' 输出结果
// 'params' 和 'ret' 已经被正确地注入到环境中
// 下面是一个示例，获取节点输入中参数名为‘input’的值：
// const input = params.input; 
// 下面是一个示例，输出一个包含多种数据类型的 'ret' 对象：
// const ret = { "name": ‘小明’, "hobbies": [“看书”, “旅游”] };

var jsonData = `{
    "3": {
      "inputs": {
        "weight": 1,
        "weight_faceidv2": 1,
        "weight_type": "linear",
        "combine_embeds": "concat",
        "start_at": 0,
        "end_at": 1,
        "embeds_scaling": "V only",
        "model": [
          "17",
          0
        ],
        "ipadapter": [
          "17",
          1
        ],
        "image": [
          "14",
          0
        ],
        "clip_vision": [
          "51",
          0
        ],
        "insightface": [
          "53",
          0
        ]
      },
      "class_type": "IPAdapterFaceID",
      "_meta": {
        "title": "IPAdapter FaceID"
      }
    },
    "7": {
      "inputs": {
        "width": 512,
        "height": 768,
        "batch_size": 1
      },
      "class_type": "EmptyLatentImage",
      "_meta": {
        "title": "Empty Latent Image"
      }
    },
    "8": {
      "inputs": {
        "samples": [
          "11",
          0
        ],
        "vae": [
          "12",
          2
        ]
      },
      "class_type": "VAEDecode",
      "_meta": {
        "title": "VAE Decode"
      }
    },
    "9": {
      "inputs": {
        "text": "(worst_quality:1.62) (MajicNegative_V2:1.3),(Earrings:1.6), necklaces, nose rings, accessories, jewelry BadNegAnatomyV1-neg bradhands cartoon, cgi, render, illustration, painting, drawing,vague, unclear",
        "clip": [
          "12",
          1
        ]
      },
      "class_type": "CLIPTextEncode",
      "_meta": {
        "title": "CLIP Text Encode (Prompt)"
      }
    },
    "11": {
      "inputs": {
        "seed": 578522269853077,
        "steps": 25,
        "cfg": 6,
        "sampler_name": "euler",
        "scheduler": "normal",
        "denoise": 1,
        "model": [
          "3",
          0
        ],
        "positive": [
          "31",
          0
        ],
        "negative": [
          "31",
          1
        ],
        "latent_image": [
          "7",
          0
        ]
      },
      "class_type": "KSampler",
      "_meta": {
        "title": "KSampler"
      }
    },
    "12": {
      "inputs": {
        "ckpt_name": "v1-5-pruned-emaonly.safetensors"
      },
      "class_type": "CheckpointLoaderSimple",
      "_meta": {
        "title": "Load Checkpoint"
      }
    },
    "14": {
      "inputs": {
        "image": "微信图片_20250103211539.jpg",
        "upload": "image"
      },
      "class_type": "LoadImage",
      "_meta": {
        "title": "Load Image"
      }
    },
    "15": {
      "inputs": {
        "text": "1 girl,(blue background:1.5),(smiling:0.15),(Photorealistic:1.2),photography lighting,indoor lighting,best quality,masterpiece,(Facing the camera:1.2),(white shirt:1.5), single portrait,formal wear,depth of field",
        "clip": [
          "12",
          1
        ]
      },
      "class_type": "CLIPTextEncode",
      "_meta": {
        "title": "CLIP Text Encode (Prompt)"
      }
    },
    "17": {
      "inputs": {
        "preset": "FACEID PLUS V2",
        "lora_strength": 0.6,
        "provider": "CUDA",
        "model": [
          "12",
          0
        ]
      },
      "class_type": "IPAdapterUnifiedLoaderFaceID",
      "_meta": {
        "title": "IPAdapter Unified Loader FaceID"
      }
    },
    "29": {
      "inputs": {
        "image": "ff6a57290f3145e299cebae233060b54.jpeg",
        "upload": "image"
      },
      "class_type": "LoadImage",
      "_meta": {
        "title": "Load Image"
      }
    },
    "30": {
      "inputs": {
        "images": [
          "40",
          0
        ]
      },
      "class_type": "PreviewImage",
      "_meta": {
        "title": "Preview Image"
      }
    },
    "31": {
      "inputs": {
        "strength": 1,
        "start_percent": 0,
        "end_percent": 1,
        "positive": [
          "15",
          0
        ],
        "negative": [
          "9",
          0
        ],
        "control_net": [
          "34",
          0
        ],
        "image": [
          "40",
          0
        ]
      },
      "class_type": "ControlNetApplyAdvanced",
      "_meta": {
        "title": "Apply ControlNet"
      }
    },
    "34": {
      "inputs": {
        "control_net_name": "sd-controlnet-openpose .safetensors"
      },
      "class_type": "ControlNetLoader",
      "_meta": {
        "title": "Load ControlNet Model"
      }
    },
    "40": {
      "inputs": {
        "detect_hand": "enable",
        "detect_body": "enable",
        "detect_face": "disable",
        "resolution": 512,
        "scale_stick_for_xinsr_cn": "enable",
        "image": [
          "29",
          0
        ]
      },
      "class_type": "OpenposePreprocessor",
      "_meta": {
        "title": "OpenPose Pose"
      }
    },
    "42": {
      "inputs": {
        "enabled": true,
        "swap_model": "inswapper_128.onnx",
        "facedetection": "retinaface_resnet50",
        "face_restore_model": "none",
        "face_restore_visibility": 0.8,
        "codeformer_weight": 0.5,
        "detect_gender_input": "no",
        "detect_gender_source": "no",
        "input_faces_index": "0",
        "source_faces_index": "0",
        "console_log_level": 1,
        "input_image": [
          "8",
          0
        ],
        "source_image": [
          "14",
          0
        ]
      },
      "class_type": "ReActorFaceSwap",
      "_meta": {
        "title": "ReActor 🌌 Fast Face Swap"
      }
    },
    "51": {
      "inputs": {
        "clip_name": "CLIP-ViT-H-14-laion2B-s32B-b79K.safetensors"
      },
      "class_type": "CLIPVisionLoader",
      "_meta": {
        "title": "Load CLIP Vision"
      }
    },
    "53": {
      "inputs": {
        "provider": "CUDA",
        "model_name": "buffalo_l"
      },
      "class_type": "IPAdapterInsightFaceLoader",
      "_meta": {
        "title": "IPAdapter InsightFace Loader"
      }
    },
    "55": {
      "inputs": {
        "similarity_metric": "L2_norm",
        "filter_thresh": 100,
        "filter_best": 0,
        "generate_image_overlay": true,
        "analysis_models": [
          "56",
          0
        ],
        "reference": [
          "14",
          0
        ],
        "image": [
          "8",
          0
        ]
      },
      "class_type": "FaceEmbedDistance",
      "_meta": {
        "title": "Face Embeds Distance"
      }
    },
    "56": {
      "inputs": {
        "library": "insightface",
        "provider": "CUDA"
      },
      "class_type": "FaceAnalysisModels",
      "_meta": {
        "title": "Face Analysis Models"
      }
    },
    "58": {
      "inputs": {
        "similarity_metric": "L2_norm",
        "filter_thresh": 100,
        "filter_best": 0,
        "generate_image_overlay": true,
        "analysis_models": [
          "56",
          0
        ],
        "reference": [
          "14",
          0
        ],
        "image": [
          "42",
          0
        ]
      },
      "class_type": "FaceEmbedDistance",
      "_meta": {
        "title": "Face Embeds Distance"
      }
    },
    "60": {
      "inputs": {
        "filename_prefix": "ComfyUI",
        "images": [
          "58",
          0
        ]
      },
      "class_type": "SaveImage",
      "_meta": {
        "title": "Save Image"
      }
    }
  }`;

class ComfyUIAPI {
    serverAddress:any;
    wsAddress:any;
    constructor() {
        this.serverAddress = `https://e2ff-221-234-179-183.ngrok-free.app`;
        this.wsAddress = `ws://https://e2ff-221-234-179-183.ngrok-free.app/ws`;
    }

    async uploadImage(file) {
        const formData = new FormData();
        formData.append('image', file);

        const response = await fetch(`${this.serverAddress}/upload/image`, {
            method: 'POST',
            body: formData
        });

        if (!response.ok) {
            throw new Error(`Failed to upload image: ${response.statusText}`);
        }

        return file.name;
    }

    async generateImage(sourceImageFile, poseImageFile) {
        try {
            // 上传源图片和姿势图片
           // const sourceImageName = await this.uploadImage(sourceImageFile);
            //const poseImageName = await this.uploadImage(poseImageFile);

            // 读取工作流配置
            //const response = await fetch(jsonData);
            const workflow =  JSON.parse(jsonData);

            // 更新工作流中的图片路径
            //workflow["14"]["inputs"]["image"] = sourceImageName;
            //workflow["29"]["inputs"]["image"] = poseImageName;

            // 生成prompt ID
            const promptId = crypto.randomUUID();

            // 发送请求到ComfyUI
            const promptResponse = await fetch(`${this.serverAddress}/prompt`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({
                    prompt: workflow,
                    client_id: promptId
                })
            });

            const data = await promptResponse.json();

            return data;
        } catch (error) {
            throw new Error(`Generation failed:`);
        }
    }
}

async function main() {
    const comfyuiApi = new ComfyUIAPI();
    var data = comfyuiApi.generateImage(params.source_image, params.client_id)

    const ret = {
        "key0": JSON.stringify(data), // 拼接两次入参 input 的值
    };

    console.log("ret", ret);
}