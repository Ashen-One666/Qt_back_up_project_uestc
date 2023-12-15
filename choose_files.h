#ifndef __CHOOSE_FILES__
#define __CHOOSE_FILES__


#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <tchar.h>


/*
TCHAR*תchar*
*/
char* LPWSTR2LPSTR(LPWSTR lpwszStrIn);
std::vector <std::string> chooseFiles();

#endif