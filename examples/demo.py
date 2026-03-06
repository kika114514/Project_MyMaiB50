import ctypes
import os

# 加载重命名后的 DLL
dll_path = os.path.abspath("../sdk/PJMMB.dll")
sdk = ctypes.CDLL(dll_path)

# 配置接口
sdk.inputApi.argtypes = [ctypes.c_char_p]
sdk.inputApi.restype = ctypes.c_int
sdk.infoApi.restype = ctypes.c_char_p
sdk.b50infoApi.restype = ctypes.c_char_p

def main():
    qr_url = input("请输入微信二维码链接: ").strip()
    status = sdk.inputApi(qr_url.encode('utf-8'))
    
    if status == 1:
        print("\n✅ 鉴权成功！")
        print("[基础档案]:\n", sdk.infoApi().decode('utf-8'))
        
        b50_json = sdk.b50infoApi().decode('utf-8')
        with open("b50_result.json", "w", encoding="utf-8") as f:
            f.write(b50_json)
        print("\n✅ B50 数据已保存至 b50_result.json")
    else:
        print("\n❌ 鉴权失败：二维码无效或账号正处于15分钟冷却锁定期。")

if __name__ == "__main__":
    main()