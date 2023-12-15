#include "aesfile.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <Windows.h>
#include <string>

#if TYPE_OS == OS_WINDOWS
#include <io.h>
#define lstat		_stati64
#define	stat		_stati64
#else
#include <sys/vfs.h>
#endif // TYPE_OS == OS_WINDOWS

using namespace std;

static const unsigned int READ_BUF_LEN = 1024 * 1024;
#if AES_BIT == 256
unsigned char gAESKEY[AES_KEYLEN] = { 0x45,0x4d,0x5e,0xdf,0x8c,0xae,0x53,0x23,
								   0xc4,0xed,0xac,0x99,0xaa,0x78,0x29,0x45,
								   0x11,0x54,0x36,0x65,0x45,0x88,0xac,0xea,
								   0xde,0xdd,0xea,0xeb,0xbd,0x8b,0x9a,0xb5 };
#elif AES_BIT == 192
unsigned char gAESKEY[AES_KEYLEN] = { 0x85,0x4d,0x55,0xdf,0x8c,0xae,0x53,0x23,
								   0xca,0xed,0xac,0x99,0xaa,0x76,0x29,0x45,
								   0x11,0x54,0x31,0x65,0x45,0x28,0xac,0xea };

#elif AES_BIT == 128
unsigned char gAESKEY[AES_KEYLEN] = { 0x85,0x4d,0x5e,0xdf,0x8c,0x5e,0x53,0x23,
								   0xca,0xed,0xac,0x97s,0xaa,0x78,0x29,0x45 };
#endif

/**
  * @brief 构造函数
  */
aes::aes()
{
	aesBit = AES_BIT;
	aesKeyLen = AES_KEYLEN;
	myKey = new unsigned char[aesKeyLen];
	memcpy(myKey, gAESKEY, aesKeyLen);
}

/**
  * @brief 析构函数
  */
aes::~aes()
{
	delete[] myKey;
}

/**
  * @brief 设置密钥和加密位数
  * @para1 key 密钥
  * @para2 bit 加密位数
  * @expl
  * @return 0:成功 -1:失败
  */
int aes::setKey(unsigned char* key, AESBIT bit)
{
	aesBit = bit;
	aesKeyLen = bit / 8;
	delete[] myKey;
	myKey = new unsigned char[aesKeyLen];
	memcpy(myKey, key, aesKeyLen);
	return 0;
}



/**
  * @brief AES文件加密
  * @para1 src_path 源文件,未加密的文件
  * @para2 dst_path 目的文件,加密文件
  * @expl
  * @return 0:成功 其他:失败
  */
int aes::encryptFile(const char* src_path, const char* dst_path)
{

	AES_KEY aeskey;
	struct stat src_stat;
	int read_fd, write_fd, ret, add_tail = 0;
	long long read_file_size;
	int read_size, remain;
	int block, block_remain;
	unsigned char* inbuf = new unsigned char[READ_BUF_LEN + 32];
	unsigned char* outbuf = new unsigned char[READ_BUF_LEN + 32];

	/* 获取文件属性 */
	if (lstat(src_path, &src_stat) != 0)
		return ErrSrcNotExist;

	read_file_size = src_stat.st_size;

	if (read_file_size % 16 == 0 || read_file_size == 0)
		add_tail = 1;


	read_fd = open(src_path, O_RDONLY | O_BINARY);
	if (read_fd < 0)
		return ErrSrcOpenFail;


	write_fd = open(dst_path, O_CREAT | O_WRONLY | O_BINARY);
	if (write_fd < 0) {
		close(read_fd);
		return ErrDstOpenFail;
	}

	AES_set_encrypt_key(myKey, aesBit, &aeskey);

	while (read_file_size > 0) {
		read_size = read(read_fd, inbuf, READ_BUF_LEN);
		block = read_size / 16;
		block_remain = read_size % 16;
		for (int i = 0; i < block; i++) {
			AES_encrypt(&inbuf[i * 16], &outbuf[i * 16], &aeskey);
		}
		if (block_remain != 0) {
			remain = 32 - block_remain;
			for (int i = block_remain; i < 16; i++)
				inbuf[i + block * 16] = remain;
			AES_encrypt(&inbuf[block * 16], &outbuf[block * 16], &aeskey);
			block++;
			for (int i = 0; i < 16; i++)
				inbuf[i + block * 16] = remain;
			AES_encrypt(&inbuf[block * 16], &outbuf[block * 16], &aeskey);
			block++;
		}
		ret = write(write_fd, outbuf, block * 16);
		if (ret < 0)
			goto END;
		read_file_size -= read_size;
	}

	if (add_tail == 1) {
		for (int k = 0; k < 2; k++) {
			memset(inbuf, 32, 16);
			AES_encrypt(inbuf, outbuf, &aeskey);
			ret = write(write_fd, outbuf, 16);
			if (ret < 0)
				goto END;
		}
	}

END:
	close(read_fd);
	close(write_fd);

	delete[] inbuf;
	delete[] outbuf;
	if (ret < 0)
		return ErrWriteFail;

	return ErrNone;
}

/**
  * @brief AES文件解密
  * @para1 src_path 源文件,解密前的密文
  * @para2 dst_path 目的文件,解密后的明文
  * @expl
  * @return 0:成功 其他:失败
  */
int aes::decryptFile(const char* src_path, const char* dst_path)
{
	AES_KEY aeskey;
	struct stat src_stat;
	int read_fd, write_fd, ret, same = 0, write_len;
	long long read_file_size;
	int read_size, block;
	unsigned char* inbuf = new unsigned char[READ_BUF_LEN + 32];
	unsigned char* outbuf = new unsigned char[READ_BUF_LEN + 32];
	unsigned char data;
	char* dstfileDir = NULL;

	/* 获取文件属性 */
	if (lstat(src_path, &src_stat) != 0)
		return ErrSrcNotExist;

	read_file_size = src_stat.st_size;

	/* 判断加密文件长度是否合法,正常为16的倍数 */
	if (read_file_size % 16 != 0 || read_file_size == 0)
		return ErrAesFileFormat;

	read_fd = open(src_path, O_RDONLY | O_BINARY);
	if (read_fd < 0)
		return ErrSrcOpenFail;

	/* 删除已存在的目的文件 */
	unlink(dst_path);

	write_fd = open(dst_path, O_CREAT | O_WRONLY | O_BINARY);
	if (write_fd < 0) {
		close(read_fd);
		return ErrDstOpenFail;
	}

	AES_set_decrypt_key(myKey, aesBit, &aeskey);

	while (read_file_size > 0) {
		read_size = read(read_fd, inbuf, READ_BUF_LEN);

		block = read_size / 16;

		for (int i = 0; i < block; i++)
			AES_decrypt(&inbuf[i * 16], &outbuf[i * 16], &aeskey);

		write_len = read_size;

		if (read_size == read_file_size) {
			data = outbuf[read_size - 1];
			for (int i = 0; i < 32; i++) {
				if (data == outbuf[read_size - 1 - i]) {
					same++;
				}
				else {
					break;
				}
			}
			if (same > 0 && same >= data) {
				write_len = read_size - data;
			}
		}
		ret = write(write_fd, outbuf, write_len);
		if (ret < 0)
			goto END;
		read_file_size -= read_size;
	}
END:
	close(read_fd);
	close(write_fd);

	delete[] inbuf;
	delete[] outbuf;

	if (ret < 0)
		return ErrWriteFail;
	return ErrNone;
}