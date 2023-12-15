#pragma once
#ifndef __CHOOSE_FOLDER__ 
#define __CHOOSE_FOLDER__


#ifndef BIF_NEWDIALOGSTYLE  
#define  BIF_NEWDIALOGSTYLE  0x0040  
#endif  


#include <Shlobj.h>
#include <string>
#include <windows.h>
#include <WINDOWS.H>
#include <tchar.h>
#include <iostream>
#include <fstream>

std::string Wchar_to_Char(const wchar_t* wc);
std::string TCHAR2STRING(TCHAR* STR);

class CSelectFolderDlg
{
public:
    //创建一个选择文件夹的对话框，返回所选路径  
    static std::string Show()
    {
        TCHAR           szFolderPath[MAX_PATH] = { 0 };
        std::string     strFolderPath = Wchar_to_Char(TEXT(""));

        BROWSEINFO      sInfo;
        ::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
        sInfo.pidlRoot = 0;
        sInfo.lpszTitle = _T("please choose a folder: ");
        sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
        sInfo.lpfn = NULL;

        // 显示文件夹选择对话框  
        LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
        if (lpidlBrowse != NULL)
        {
            // 取得文件夹名  
            if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
            {
                strFolderPath = TCHAR2STRING(szFolderPath);
            }
        }
        if (lpidlBrowse != NULL)
        {
            ::CoTaskMemFree(lpidlBrowse);
        }

        return strFolderPath;

    }

};

#endif 