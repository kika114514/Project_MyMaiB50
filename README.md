```markdown

# 🌟 Project MyMaiB50 SDK 🌟

**专为 舞萌 DX (1.53 PRiSM) 打造的高性能本地数据拉取 SDK**


---

## 📖 项目简介

Project MyMaiB50 是一款底层采用 **Rust** 编写的高性能动态链接库 (DLL)。
内置完整的 AES-256-CBC 加解密、Zlib 压缩解析与全自动的 Session 管理机制。无需繁琐的抓包配置，只需提供微信授权链接，即可静默、安全地拉取玩家的精简游戏档案与完整的 B50 成绩数据。

## ✨ 核心特性

- 🔒 **防线穿透**：底层完整封装 1.53 版本密码学、时间戳对齐与防重放机制。
- ⚡ **跨语言兼容**：提供标准极简的 C-ABI 接口，完美兼容 Python、C++、C# 等多语言的 FFI 调用。
- 🛡️ **内存安全**：内部独立维护线程安全的 Mutex 缓冲区，调用端无需手动管理指针与内存释放。

---

## 🧰 API 接口文档

> **注意**：所有接口返回的字符串均采用 `UTF-8` 编码。DLL 导出的函数名大小写敏感。

| 接口定义 | 功能说明 | 返回值 / 状态 |
| :--- | :--- | :--- |
| `int inputApi(const char* qr_code)` | 初始化玩家会话并进行网关与服务器鉴权 | `1` 成功 <br> `0` 失败 (二维码失效或 15 分钟冷却期) |
| `const char* infoApi()` | 获取玩家轻量级基础档案 | 返回 JSON 指针 (含昵称、底力、头像ID、姓名框ID) |
| `const char* b50infoApi()` | 获取玩家完整 B50 成绩与段位数据 | 返回 JSON 指针 (含新旧版本 RatingList、对战段位) |

---

## 💻 快速起步

### 🐍 Python 调用示例 (基于 `ctypes`)

```python
import ctypes
import os

# 1. 加载 DLL
dll_path = os.path.abspath("./PJMMB.dll")
sdk = ctypes.CDLL(dll_path)

# 2. 定义 C 接口的参数与返回类型
sdk.inputApi.argtypes = [ctypes.c_char_p]
sdk.inputApi.restype = ctypes.c_int
sdk.infoApi.restype = ctypes.c_char_p
sdk.b50infoApi.restype = ctypes.c_char_p

def main():
    qr_url = input("👉 请输入微信二维码链接: ").strip()
    
    # 执行鉴权
    if sdk.inputApi(qr_url.encode('utf-8')) == 1:
        print("\n✅ 鉴权成功！\n")
        
        # 提取轻量档案
        print("[基础档案]")
        print(sdk.infoApi().decode('utf-8'))
        
        # 提取并保存 B50 数据
        b50_json = sdk.b50infoApi().decode('utf-8')
        with open("b50_result.json", "w", encoding="utf-8") as f:
            f.write(b50_json)
            
        print("\n✅ [B50 数据] 已成功保存至 b50_result.json")
    else:
        print("\n❌ 鉴权失败：二维码无效，或账号正处于 15 分钟保护冷却期。")

if __name__ == "__main__":
    main()

```

### 🪟 C++ 调用示例 (基于 `Windows.h`)

```cpp
#include <iostream>
#include <windows.h>
#include <string>

typedef int (*InputApiFunc)(const char*);
typedef const char* (*InfoApiFunc)();

int main() {
    // 动态加载 DLL
    HMODULE hDll = LoadLibraryA("PJMMB.dll");
    if (!hDll) {
        std::cerr << "DLL 加载失败！请检查文件路径。" << std::endl;
        return 1;
    }

    InputApiFunc inputApi = (InputApiFunc)GetProcAddress(hDll, "inputApi");
    InfoApiFunc infoApi = (InfoApiFunc)GetProcAddress(hDll, "infoApi");
    InfoApiFunc b50infoApi = (InfoApiFunc)GetProcAddress(hDll, "b50infoApi");

    std::string qrUrl;
    std::cout << "👉 请输入二维码链接: ";
    std::getline(std::cin, qrUrl);

    if (inputApi(qrUrl.c_str()) == 1) {
        std::cout << "\n✅ 鉴权成功！" << std::endl;
        std::cout << "\n[玩家档案]:\n" << infoApi() << std::endl;
        std::cout << "\n[B50 数据]: 成功拉取 " << strlen(b50infoApi()) << " bytes" << std::endl;
    } else {
        std::cerr << "\n❌ 鉴权失败：二维码失效或处于保护期。" << std::endl;
    }

    FreeLibrary(hDll);
    return 0;
}

```

---

## ⚠️ 使用限制与已知特性

1. **15分钟在线保护锁**：当成功通过 `inputApi` 拉取数据后，服务器会将该账号标记为“在线机台状态”。在随后的 **15 分钟**内，该账号无法再次通过外部鉴权，强制操作将直接返回失败。请妥善规划请求频率。
2. **指针生命周期**：接口返回的 `const char*` 指针指向 DLL 内部维持的静态内存。**请勿在调用端尝试 `free` 或 `delete` 该指针**，这会导致进程崩溃。下次调用时内存会自动覆写。

---

## ⚖️ 授权与免责声明

**Freeware License & Strict Disclaimer**

* 本项目仅供技术研究、学习与交流使用。
* **严禁**将本 SDK 用于任何形式的商业盈利、高频爬虫或破坏原版游戏服务器公平性与稳定性的行为。
* **严禁**对二进制文件 `PJMMB.dll` 进行反编译或逆向工程。
* 本项目为个人非官方开发，与 SEGA (世嘉)、Wahlap (华立科技) 及其相关品牌无任何关联。
* **风险自负**：作者不对使用本项目产生的任何后果（包括但不限于账号封禁、数据丢失、法律纠纷）承担任何直接或间接的责任。

```

```

