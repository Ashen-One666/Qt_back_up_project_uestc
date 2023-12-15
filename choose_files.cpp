#include "choose_files.h"


char* LPWSTR2LPSTR(LPWSTR lpwszStrIn)
{
    LPSTR pszOut = NULL;
    if (lpwszStrIn != NULL) {
        int nInputStrLen = wcslen(lpwszStrIn);
        int nOutputStrLen = WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
        pszOut = new char[nOutputStrLen];
        if (pszOut != NULL) {
            memset(pszOut, 0x00, nOutputStrLen);
            WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
        }
    }
    return  pszOut;
}

std::vector <std::string> chooseFiles() {

    OPENFILENAME ofn;
    TCHAR szOpenFileNames[80 * MAX_PATH] = { 0 };
    TCHAR szPath[MAX_PATH];
    TCHAR szFileName[80 * MAX_PATH];
    std::vector <std::string> cho;

    int nLen = 0;
    TCHAR* p = NULL;
    ZeroMemory(&ofn, sizeof(ofn));

    // 结构体大小
    ofn.lStructSize = sizeof(ofn);

    // 拥有着窗口句柄
    ofn.hwndOwner = NULL;

    // 接收返回的文件名，注意第一个字符需要为NULL
    ofn.lpstrFile = szOpenFileNames;

    // 缓冲区长度
    ofn.nMaxFile = sizeof(szOpenFileNames);

    // _T可替换为TEXT，使用_T需要引tchar.h
    ofn.lpstrFile[0] = _T('\0');

    // 设置过滤
    ofn.lpstrFilter = _T("All\0*.*\0.mp4\0*.mp4\0.avi\0*.avi\0.mkv\0*.mkv\0.rmvb\0*.rmvb\0.f4v\0*.f4v\0.flv\0*.flv\0.m4v\0*.m4v\0.mpg\0*.mpg\0\0");

    // 过滤器索引
    ofn.nFilterIndex = 1;

    // 窗口标题
    ofn.lpstrTitle = _T("please choose the files");

    // 文件必须存在、允许多选、隐藏只读选项、对话框使用资源管理器风格的用户界面
    // 官方文档：https://docs.microsoft.com/en-us/windows/win32/api/commdlg/ns-commdlg-openfilenamea
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_EXPLORER;

    // 如果打开文件失败，则不操作
    if (!::GetOpenFileName(&ofn)) {
        std::cout << "open file failed" << std::endl;
        return cho;
    }

    // 把第一个文件名前的复制到szPath,即:  
    // 如果只选了一个文件,就复制到最后一个'/'  
    // 如果选了多个文件,就复制到第一个NULL字符  
    lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

    // 当只选了一个文件时,下面这个NULL字符是必需的.  
    // 这里不区别对待选了一个和多个文件的情况
    szPath[ofn.nFileOffset] = '\0';
    nLen = lstrlen(szPath);

    // 如果选了多个文件,则必须加上'//'  
    if (szPath[nLen - 1] != '\\') {
        lstrcat(szPath, _T("\\"));
    }

    // 把指针移到第一个文件  
    p = szOpenFileNames + ofn.nFileOffset;

    // 对szFileName进行清零
    ZeroMemory(szFileName, sizeof(szFileName));


    while (*p) {
        // 读取文件名
        std::string fileName = LPWSTR2LPSTR(p);

        // 读取文件所在文件夹路径
        std::string filePath = LPWSTR2LPSTR(szPath);

        // 拼接文件完整路径
        std::string completePath = filePath + fileName;

        // 拼接字符串
        cho.push_back(completePath);

        //移至下一个文件
        p += lstrlen(p) + 1;
    }

    return cho;
}