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

    // �ṹ���С
    ofn.lStructSize = sizeof(ofn);

    // ӵ���Ŵ��ھ��
    ofn.hwndOwner = NULL;

    // ���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
    ofn.lpstrFile = szOpenFileNames;

    // ����������
    ofn.nMaxFile = sizeof(szOpenFileNames);

    // _T���滻ΪTEXT��ʹ��_T��Ҫ��tchar.h
    ofn.lpstrFile[0] = _T('\0');

    // ���ù���
    ofn.lpstrFilter = _T("All\0*.*\0.mp4\0*.mp4\0.avi\0*.avi\0.mkv\0*.mkv\0.rmvb\0*.rmvb\0.f4v\0*.f4v\0.flv\0*.flv\0.m4v\0*.m4v\0.mpg\0*.mpg\0\0");

    // ����������
    ofn.nFilterIndex = 1;

    // ���ڱ���
    ofn.lpstrTitle = _T("please choose the files");

    // �ļ�������ڡ������ѡ������ֻ��ѡ��Ի���ʹ����Դ�����������û�����
    // �ٷ��ĵ���https://docs.microsoft.com/en-us/windows/win32/api/commdlg/ns-commdlg-openfilenamea
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_EXPLORER;

    // ������ļ�ʧ�ܣ��򲻲���
    if (!::GetOpenFileName(&ofn)) {
        std::cout << "open file failed" << std::endl;
        return cho;
    }

    // �ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:  
    // ���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'  
    // ���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�  
    lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

    // ��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.  
    // ���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
    szPath[ofn.nFileOffset] = '\0';
    nLen = lstrlen(szPath);

    // ���ѡ�˶���ļ�,��������'//'  
    if (szPath[nLen - 1] != '\\') {
        lstrcat(szPath, _T("\\"));
    }

    // ��ָ���Ƶ���һ���ļ�  
    p = szOpenFileNames + ofn.nFileOffset;

    // ��szFileName��������
    ZeroMemory(szFileName, sizeof(szFileName));


    while (*p) {
        // ��ȡ�ļ���
        std::string fileName = LPWSTR2LPSTR(p);

        // ��ȡ�ļ������ļ���·��
        std::string filePath = LPWSTR2LPSTR(szPath);

        // ƴ���ļ�����·��
        std::string completePath = filePath + fileName;

        // ƴ���ַ���
        cho.push_back(completePath);

        //������һ���ļ�
        p += lstrlen(p) + 1;
    }

    return cho;
}