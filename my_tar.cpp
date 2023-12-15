#include "my_tar.h"
#include <fstream>
#include <direct.h>
#include <algorithm>
#include <io.h>
#include <iostream>
#include <string>

#define MAX_LEN 10
using namespace std;


#define TMAGIC   "ustar"        /* ustar and a null */
#define TMAGLEN  6
#define TVERSION "00"           /* 00 and no null */
#define TVERSLEN 2

/* Values used in typeflag field.  */
#define REGTYPE  '0'            /* regular file */
#define AREGTYPE '\0'           /* regular file */
#define LNKTYPE  '1'            /* link */
#define SYMTYPE  '2'            /* reserved */
#define CHRTYPE  '3'            /* character special */
#define BLKTYPE  '4'            /* block special */
#define DIRTYPE  '5'            /* directory */
#define FIFOTYPE '6'            /* FIFO special */
#define CONTTYPE '7'            /* reserved */

static bool dir_exit(const char* path_) {
	struct _stat st;

	/*
	//‪"C:\\Users\\fez0618\\Desktop\\1.pdf"
	string pp;
	pp = "C:\\Users\\fez0618\\Desktop\\test";
	if (pp[0] == '?') pp.erase(pp.begin());
	const char* p = pp.data();
	cout << p << " ";
	cout << (_stat(p, &st) == 0) << " ";
	cout<< st.st_mode <<" ";
	if(S_IFDIR) printf("3acc ");
	return false;
	*/
	
	if (_stat(path_, &st) == 0 && st.st_mode & S_IFDIR) {
		// cout << "done";
		return true;
	}
	return false;
	
}


bool my_Tar::detar(const char* path_, const char* dst_)
{
	if (path_ == nullptr) {
		return false;
	}

	struct _stat st;
	FILE* file = nullptr;
	if (_stat(path_, &st) != 0 || (file = fopen(path_, "rb")) == nullptr) {
		return false;
	}
	const int block_size{ 512 };
	unsigned char buf[block_size];
	my_Tar::posix_header* header = (my_Tar::posix_header*)buf;
	if (st.st_size % block_size != 0) {
		//tar file size should be a multiple of 512 bytes
		return false;
	}
	bool flag_ = true;

	//最小
	auto min_met = [](long long a, long long b)->long long {
		return a < b ? a : b;
	};


	while (1) {
		if (fread(buf, block_size, 1, file) != 1) {
			//结束
			break;
		}
		if (!verifyChecksum(buf)) {
			//校验失败
			flag_ = false;
			break;
		}
		if (strncmp(header->magic, TMAGIC, 5)) {
			flag_ = false;
			break;
		}

		long long file_size{ 0 };
		sscanf(header->size, "%lo", &file_size);

		std::string str_path_ = dst_;
		std::replace(str_path_.begin(), str_path_.end(), '\\', '/');
		if (str_path_[str_path_.size() - 1] != '/') {
			str_path_.append("/");
		}
		str_path_.append(header->name);
		if (header->typeflag == REGTYPE || header->typeflag == AREGTYPE) {
			std::ofstream out;
			out.open(str_path_.c_str(), std::ios::binary);
			while (file_size > 0) {
				fread(buf, block_size, 1, file);
				out.write((char*)buf, min_met(file_size, block_size));
				file_size -= block_size;
			}
			out.close();
		}
		else if (header->typeflag == '5') {
			mkdir(str_path_);
		}
		else {

		}
	}
	fclose(file);
	return flag_;
}

bool my_Tar::set_posix_header(std::string path_, std::string prefix, unsigned char* buf) {


	my_Tar::posix_header* header = (my_Tar::posix_header*)buf;
	struct _stat st;
	if (_stat(path_.append(prefix).c_str(), &st) != 0) {
		cout << "error";
		return false;
	}

	if ((st.st_mode & S_IFDIR)) {
		header->typeflag = DIRTYPE;
		prefix = prefix.substr(1).append("/");
	}
	else if (st.st_mode & S_IFREG) {
		header->typeflag = REGTYPE;
		prefix = prefix.substr(1);
	}
	else {
		cout << "error";
		return false;
	}


	//tar_posix_header head;
	//name
	memcpy(header->name, prefix.c_str(), prefix.size());
	header->name[prefix.size()] = '\0';


	sprintf(header->mode, "%07d", 777);
	sprintf(header->uid, "%07o", st.st_uid);
	sprintf(header->gid, "%07o", st.st_gid);
	sprintf(header->size, "%011lo", st.st_size);
	sprintf(header->mtime, "%011llo", st.st_mtime);
	memcpy(header->chksum, "        ", 8);
	memcpy(header->magic, "ustar ", 6);
	sprintf(header->version, " ");
	setCheckSum(buf, header->chksum);
	return true;
}


bool my_Tar::tar(const char* src_, const char* dst_) {

	//cout << dst_;

	if (src_ == nullptr || !dir_exit(src_)) {
		return false;
	}
	// cout << "done2"<<" ";

	std::string path_ = src_;
	

	std::replace(path_.begin(), path_.end(), '\\', '/');
	if (path_[path_.size() - 1] == '/') {
		path_ = path_.substr(0, path_.size() - 1);
	}
	std::string prefix = path_.substr(path_.rfind('/'));
	path_ = path_.substr(0, path_.rfind('/'));

	

	const int block_size{ 512 };
	unsigned char buf[block_size];

	memset(buf, 0, block_size);
	set_posix_header(path_, prefix, buf);
	//cout << prefix;
	std::ofstream out;
	out.open(dst_, std::ios::binary);
	out.write((char*)buf, 512);

	tar_handle(out, path_, prefix);

	for (int i = 0; i < 1024; i++) {
		out.put('\0');
	}
	out.close();
	//printf("111");
	return true;
}

bool my_Tar::tar_handle(std::ofstream& out, std::string& path, std::string& prefix)
{
	//文件句柄，win10用long long，win7用long
	long long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;


	std::string p;
	if ((hFile = _findfirst(p.assign(path).append(prefix).append("/*").c_str(), &fileinfo)) != -1) {
		do {
			if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
				const int block_size{ 512 };
				unsigned char buf[block_size];

				memset(buf, 0, block_size);

				set_posix_header(path, p.assign(prefix).append("/").append(fileinfo.name), buf);
				out.write((char*)buf, 512);
			}
			//如果是目录,迭代之 //如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					tar_handle(out, path, p.assign(prefix).append("/").append(fileinfo.name));
				}
			}
			else {
				copy(out, p.assign(path).append(prefix).append("/").append(fileinfo.name).c_str());
				int k = fileinfo.size % 512;
				for (int i = k; i < 512; i++) {
					out.put('\0');
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	else {
		return false;
	}
	return true;
}



/* Parse an octal number, ignoring leading and trailing nonsense. */
int my_Tar::parseoct(const unsigned char* p, std::size_t n) {
	int i = 0;

	while ((*p < '0' || *p > '7') && n > 0) {
		++p;
		--n;
	}
	while (*p >= '0' && *p <= '7' && n > 0) {
		i *= 8;
		i += *p - '0';
		++p;
		--n;
	}
	return (i);
}

/* Verify the tar checksum. */
int my_Tar::verifyChecksum(const unsigned char* p) {
	int n, u = 0;
	for (n = 0; n < 512; ++n) {
		if (n < 148 || n > 155)
			/* Standard tar checksum adds unsigned bytes. */
			u += ((unsigned char*)p)[n];
		else
			u += 0x20;
	}
	return (u == parseoct(p + 148, 8));
}

void my_Tar::setCheckSum(const unsigned char* p, char* buf_) {
	int n, u = 0;
	for (n = 0; n < 512; ++n) {
		if (n < 148 || n > 155)
			/* Standard tar checksum adds unsigned bytes. */
			u += ((unsigned char*)p)[n];
		else
			u += 0x20;
	}
	sprintf(buf_, "%06o", u);
}



bool my_Tar::mkdir(std::string& path_) {


	if (dir_exit(path_.c_str())) {
		return true;
	}
	//替换
	std::replace(path_.begin(), path_.end(), '/', '\\');

	std::string::size_type pos = path_.find('\\', 0) + 1;
	while ((pos = path_.find('\\', pos)) != std::string::npos) {
		std::string str_ = path_.substr(0, pos);
		if (!dir_exit(str_.c_str())) {
			// 不存在该文件夹
			if (_mkdir(str_.c_str()) < 0) {
				return false;
			}
		}
		++pos;
	}
	if (_mkdir(path_.c_str()) < 0) {
		return false;
	}
	return true;
}

bool my_Tar::copy(std::ofstream& out, const char* msg_) {


	std::ifstream in;
	in.open(msg_, std::ios::binary);
	if (!in) {
		return false;
	}
	out << in.rdbuf();
	in.close();
	return true;
}