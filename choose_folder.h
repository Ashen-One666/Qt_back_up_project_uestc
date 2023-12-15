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
    //����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��  
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

        // ��ʾ�ļ���ѡ��Ի���  
        LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
        if (lpidlBrowse != NULL)
        {
            // ȡ���ļ�����  
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