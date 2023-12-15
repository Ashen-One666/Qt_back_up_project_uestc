#pragma once
#ifndef __MY_PACK_AND_ENCODE__
#define __MY_PACK_AND_ENCODE__
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <WINDOWS.H>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <sys/stat.h>
#include <wchar.h>
#include "aesfile.h"
#include "my_tar.h"
#include "bundle.h"



void file_to_folder(std::string src_path, std::string dst_folder);
LPCWSTR stringToLPCWSTR(std::string orig);
void my_tar(std::string source, std::string target);
void my_untar(std::string source, std::string target);
void my_pack(std::string source, std::string target);
void my_unpack(std::string source, std::string target);
void my_encode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit);
void my_decode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit);
void removeDir(std::string dirPath);
void my_packup(std::string target_folder,std::vector <std::string> cho, const char* key,int aes_flag,int mode_flag);
void my_restore(std::string target_folder, std::vector <std::string> cho, const char* key, int aes_flag);


#endif