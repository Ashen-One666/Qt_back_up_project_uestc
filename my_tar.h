#pragma once
#ifndef __MY_TAR__
#define __MY_TAR__

#include <vector>
#include <string>
class my_Tar
{
private:
	my_Tar() {}
public:
	virtual ~my_Tar() {}
	static my_Tar* getInstance() {
		static my_Tar tar_;
		return &tar_;
	}
	struct posix_header {                                     /* byte offset */
		char name[100];               /*   0 */
		char mode[8];                 /* 100 */
		char uid[8];                  /* 108 */
		char gid[8];                  /* 116 */
		char size[12];                /* 124 */
		char mtime[12];               /* 136 */
		char chksum[8];               /* 148 */
		char typeflag;                /* 156 */
		char linkname[100];           /* 157 */
		char magic[6];                /* 257 */
		char version[2];              /* 263 */
		char uname[32];               /* 265 */
		char gname[32];               /* 297 */
		char devmajor[8];             /* 329 */
		char devminor[8];             /* 337 */
		char prefix[155];             /* 345 */
										  /* 500 */
	};
public:

	bool detar(const char* path_, const char* dst_);
	bool tar(const char* src_, const char* dst_);
private:
	bool tar_handle(std::ofstream& out, std::string& path, std::string& prefix);
	int verifyChecksum(const unsigned char* p);
	int parseoct(const unsigned char* p, std::size_t n);
	void setCheckSum(const unsigned char* p, char* buf_);
	bool mkdir(std::string& path_);
	bool set_posix_header(std::string path_, std::string prefix, unsigned char* buf);
	bool copy(std::ofstream& out, const char* msg_);
};

#endif