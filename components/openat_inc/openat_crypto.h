#ifndef __OPENAT_CRYPTO_H__
#define __OPENAT_CRYPTO_H__

/*+\NEW\wangyuan\2020.04.14\ BUG_1445:rsa�ӽ��ܽӿڲ�����*/
typedef enum E_AMOPENAT_RSA_KEY_MODE_TAG
{
    OPENAT_RSA_PUBLIC_KEY,        /* ��Կ*/
    OPENAT_RSA_PRIVATE_KEY,     /* ˽Կ*/
    OPENAT_RSA_KEY_MAX
}E_AMOPENAT_RSA_KEY_MODE;

typedef enum E_AMOPENAT_RSA_CRYPT_MODE_TAG
{
    OPENAT_RSA_PUBLIC_KEY_CRYPT,        /* ��Կ���ܻ��߽���*/
    OPENAT_RSA_PRIVATE_KEY_CRYPT,     /* ˽Կ���ܻ��߽���*/
    OPENAT_RSA_CRYPT_MODE_MAX
}E_AMOPENAT_RSA_CRYPT_MODE;

int openat_rsa_encrypt(int nKeyMode, unsigned char *pKeyBuf, int nKeyLen,
                                             unsigned char *pPswd, int nPswdLen,
      								  int nEncryptMode,
      								  unsigned char *pInBuf, int nInLen,
      								  unsigned char *pOutBuf);


int openat_rsa_decrypt(int nKeyMode, unsigned char *pKeyBuf, int nKeyLen,
                                             unsigned char *pPswd, int nPswdLen,
      								  int nDecryptMode,
      								  unsigned char *pInBuf, int nInLen,
      								  unsigned char *pOutBuf, int *pOutLen, int nOutBufSize);


int openat_rsa_sha256_sign(int nKeyMode, unsigned char *pKeyBuf, int nKeyLen,
                                             unsigned char *pPswd, int nPswdLen,
      								  int nEncryptMode,
      								  unsigned char *pInBuf, int nInLen,
      								  unsigned char *pOutBuf);


int openat_rsa_sha256_verify(int nKeyMode, unsigned char *pKeyBuf, int nKeyLen,
                                                     unsigned char *pPswd, int nPswdLen,
      								  int nEncryptMode,
      								  unsigned char *pInBuf, int nInLen,
      								  unsigned char *pInPlainBuf, int nInPlainLen);



#endif
/*-\NEW\wangyuan\2020.04.14\ BUG_1445:rsa�ӽ��ܽӿڲ�����*/

