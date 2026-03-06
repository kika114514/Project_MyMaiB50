#include <iostream>
#include <windows.h>
#include <string>

typedef int (*InputApiFunc)(const char*);
typedef const char* (*InfoApiFunc)();

int main() {
    // 加载重命名后的 DLL
    HMODULE hDll = LoadLibraryA("../sdk/PJMMB.dll");
    if (!hDll) {
        std::cerr << "DLL 加载失败！请检查 PJMMB.dll 是否在正确路径。" << std::endl;
        return 1;
    }

    InputApiFunc inputApi = (InputApiFunc)GetProcAddress(hDll, "inputApi");
    InfoApiFunc infoApi = (InfoApiFunc)GetProcAddress(hDll, "infoApi");
    InfoApiFunc b50infoApi = (InfoApiFunc)GetProcAddress(hDll, "b50infoApi");

    std::string qrUrl;
    std::cout << "请输入二维码链接: ";
    std::getline(std::cin, qrUrl);

    if (inputApi(qrUrl.c_str()) == 1) {
        std::cout << "\n鉴权成功！\n[档案]:\n" << infoApi() << std::endl;
        std::cout << "\n[B50]: 已成功拉取 " << strlen(b50infoApi()) << " bytes 数据。" << std::endl;
    } else {
        std::cerr << "鉴权失败(15分钟冷却或二维码无效)。" << std::endl;
    }

    FreeLibrary(hDll);
    return 0;
}