#include "choose_folder.h"

std::string Wchar_to_Char(const wchar_t* wc)
{
    char* m_char = NULL;
    delete m_char;
    m_char = NULL;
    int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
    m_char = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
    m_char[len] = '\0';
    std::string pKey = m_char;
    return pKey;
}

std::string TCHAR2STRING(TCHAR* STR)
{
    int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
    char* chRtn = new char[iLen * sizeof(char)];
    WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
    std::string str(chRtn);
    delete chRtn;
    return str;
}