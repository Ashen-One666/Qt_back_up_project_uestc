#include "my_pack_and_encode.h"

namespace fs = std::filesystem;

void my_packup(std::string target_folder, std::vector <std::string> cho,const char* key, int aes_flag, int mode_flag)
{
    aes::AESBIT key_mode = (aes_flag == 1 ? aes::AES_128 : (aes_flag == 2 ? aes::AES_192 : aes::AES_256));
    time_t timep;
    time(&timep);
    std::string filename = (std::string)ctime(&timep);
    replace(filename.begin(), filename.end(), ':', '_');
    filename.erase(filename.end() - 1);
    std::string temp_folder = target_folder + "\\"+filename;

    switch (mode_flag)
    {
    case 1:
        for (auto path : cho)
            file_to_folder(path, target_folder);
        break;

    case 2:   //tar
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);

        my_tar(temp_folder, target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        break;

    case 3:  //tar.lz
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);
        my_tar(temp_folder, (std::string)target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        my_pack((std::string)target_folder + "\\" + filename + ".tar", (std::string)target_folder + "\\" + filename + ".tar.lz");
        remove(((std::string)target_folder + "\\" + filename + ".tar").c_str());
        break;

    case 4:  //tar.aes
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);
        my_tar(temp_folder, (std::string)target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        my_encode((std::string)target_folder + "\\" + filename + ".tar", (std::string)target_folder + "\\" + filename + ".tar.aes", key, key_mode);
        remove(((std::string)target_folder + "\\" + filename + ".tar").c_str());
        break;

    case 5:  //tar.aes.lz
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);
        my_tar(temp_folder, (std::string)target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        my_pack((std::string)target_folder + "\\" + filename + ".tar", target_folder + "\\" + filename + ".tar.lz");
        remove(((std::string)target_folder + "\\" + filename + ".tar").c_str());
        my_encode((std::string)target_folder + "\\" + filename + ".tar.lz", (std::string)target_folder + "\\" + filename + ".tar.lz.aes", key, key_mode);
        remove(((std::string)target_folder + "\\" + filename + ".tar.lz").c_str());
        break;

    default:
        break;
    }
}

void my_restore(std::string target_folder, std::vector <std::string> cho,const char* key, int aes_flag)
{
    aes::AESBIT key_mode = (aes_flag == 1 ? aes::AES_128 : (aes_flag == 2 ? aes::AES_192 : aes::AES_256));
    for (auto path : cho)
    {
        file_to_folder(path, target_folder);
        //1.��ȡ����·�����ļ���
        std::string::size_type iPos = path.find_last_of('\\') + 1;
        std::string filename = path.substr(iPos, path.length() - iPos);
        //2.��ȡ������׺���ļ���
        std::string name = filename.substr(0, filename.rfind("."));
        //3.��ȡ��׺��
        std::string suffix_str = filename.substr(filename.find_last_of('.') + 1);
        std::string new_file = (std::string)(target_folder + "\\" + filename);

        if (suffix_str != "aes" && suffix_str != "lz" && suffix_str != "tar") continue;

        while (suffix_str == "aes" || suffix_str == "lz")
        {
            if (suffix_str == "aes") {
                my_decode(new_file, std::string(target_folder + "\\" + name), key, key_mode);
                remove(new_file.c_str());
            }
            else if (suffix_str == "lz") {
                my_unpack(new_file, std::string(target_folder + "\\" + name));
                remove(new_file.c_str());
            }
            path = std::string(target_folder + "\\" + name);
            //1.��ȡ����·�����ļ���
            iPos = path.find_last_of('\\') + 1;
            filename = path.substr(iPos, path.length() - iPos);
            //2.��ȡ������׺���ļ���
            name = filename.substr(0, filename.rfind("."));
            //3.��ȡ��׺��
            suffix_str = filename.substr(filename.find_last_of('.') + 1);
            new_file = (std::string)(target_folder + "\\" + filename);
        }
  
        my_untar(new_file, target_folder);
        remove(new_file.c_str());

    }
}


void file_to_folder(std::string src_path, std::string dst_folder)
{
    std::string dst_path = dst_folder + '\\' + fs::path(src_path).filename().string();
    std::ifstream src_file(src_path, std::ios::binary);
    std::ofstream dst_file(dst_path, std::ios::binary);
    dst_file << src_file.rdbuf();
    src_file.close();
    dst_file.close();
    std::cout << src_path << "File copied to " << dst_path << std::endl;
}

LPCWSTR stringToLPCWSTR(std::string orig)
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
}

void my_tar(std::string source, std::string target)
{
    if (source[0] == '?') source.erase(source.begin());
    if (target[0] == '?') target.erase(target.begin());
    const char* p = source.data();
    const char* q = target.data();
    my_Tar::getInstance()->tar(p, q);
}

void my_untar(std::string source, std::string target)
{
    if (source[0] == '?') source.erase(source.begin());
    if (target[0] == '?') target.erase(target.begin());
    const char* p = source.data();
    const char* q = target.data();
    my_Tar::getInstance()->detar(p, q);
}

void my_pack(std::string source, std::string target)
{
    std::ifstream ifs;
    ifs.open(source, std::ios::binary);//��ԭʼ�ļ�

    ifs.seekg(0, std::ios::end);//��ת��дλ�õ��ļ�ĩβ
    size_t fsize = ifs.tellg();//��ȡĩβƫ������Ҳ���ǻ�ȡ�ļ���С��
    ifs.seekg(0, std::ios::beg);//��ת����ʼ

    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);//��ȡ�ļ����ݵ�body��

    //��LZIP�ķ�ʽѹ���ļ�����
    std::string packed = bundle::pack(bundle::LZIP, body);

    std::ofstream ofs;
    ofs.open(target, std::ios::binary);//��ѹ�����ļ�
    ofs.write(&packed[0], packed.size());//��ѹ���������д��ѹ�����ļ�

    ifs.close();
    ofs.clear();
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}

void my_unpack(std::string source, std::string target)
{
    std::ifstream ifs;
    ifs.open(source, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    size_t fsize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);
    ifs.close();

    std::string unpacked = bundle::unpack(body);
    std::ofstream ofs;
    ofs.open(target, std::ios::binary);
    ofs.write(&unpacked[0], unpacked.size());
    ofs.close();
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}


void my_encode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit)
{

    aes Aes;

    Aes.setKey((unsigned char*)key, Aes_bit);
    Aes.encryptFile(source.c_str(), target.c_str());
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}

void my_decode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit)
{
    aes Aes;
    Aes.setKey((unsigned char*)key, Aes_bit);
    Aes.decryptFile(source.c_str(), target.c_str());
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}

void  removeDir(std::string dirPath)
{
    struct _finddata_t fb;   // ������ͬ�����ļ��Ĵ洢�ṹ��
    std::string path;
    long long  handle; // ע��˴���Ҫlong*2
    int   noFile;            // ��ϵͳ�����ļ��Ĵ�����
    noFile = 0;
    handle = 0;
    struct stat s;

    path = dirPath + "/*";

    handle = _findfirst(path.c_str(), &fb);
    // �ҵ���һ��ƥ����ļ�
    if (handle != 0)
    {
        // �����Լ����ҵ�ƥ����ļ�������ִ��
        while (0 == _findnext(handle, &fb))
        {
            // windows�£����и�ϵͳ�ļ�����Ϊ��..��,������������
            noFile = strcmp(fb.name, "..");

            if (0 != noFile)
            {
                path = dirPath + "/" + fb.name;

                // ����ֵΪ16����˵�����ļ��У�����
                if (fb.attrib == 16)
                {
                    removeDir(path);
                }
                // ���ļ��е��ļ���ֱ��ɾ�������ļ�����ֵ�����û����ϸ���飬���ܻ������������
                else
                {
                    remove(path.c_str());
                }
            }
        }
        //ɾ����Ŀ¼
        _wrmdir((wchar_t*)dirPath.c_str());
        // �ر��ļ��У�ֻ�йر��˲���ɾ����������������˺ܾã���׼c���õ���closedir
        // ������ܣ�һ�����Handle�ĺ���ִ�к󣬶�Ҫ���йرյĶ�����
        _findclose(handle);
    }
}

