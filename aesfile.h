#ifndef __AES_H__
#define __AES_H__

#define OS_LINUX	0x00
#define	OS_WINDOWS	0x01

#define TYPE_OS		OS_WINDOWS	//windows��ʹ��

#if TYPE_OS == OS_WINDOWS
//ȥ�����뾯��
#define _CRT_NONSTDC_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS


#endif

#define AES_BIT		256//192//256
#define AES_KEYLEN  AES_BIT/8

 //�����붨��
enum SysErrCode {
	ErrNone = 0,
	ErrSrcNotExist,
	ErrSrcOpenFail,
	ErrDstOpenFail,
	ErrWriteFail,
	ErrAesFileFormat
};

class aes {
public:
	enum AESBIT {
		AES_256 = 256,
		AES_192 = 192,
		AES_128 = 128,
	};

	int aesBit;
	int aesKeyLen;
	unsigned char* myKey;

	/**
	  * @brief aes���캯��
	  */
	aes();

	/**
	  * @brief aes��������
	  */
	~aes();

	/**
	  * @brief ���ü���key
	  */
	int setKey(unsigned char* key, AESBIT bit);


	/**
	  * @brief AES�ļ�����
	  */
	int encryptFile(const char* src_path, const char* dst_path);

	/**
	  * @brief AES�ļ�����
	  */
	int decryptFile(const char* src_path, const char* dst_path);

};

#endif