#pragma once
typedef struct {
	char SCA[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
	char TPA[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
	char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
	char TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
	char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[160];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	short index;			// ����Ϣ��ţ��ڶ�ȡʱ�õ�
} SM_PARAM;

class CEncode
{
public:
	CEncode(void);
	~CEncode(void);
public:
	int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength);
	int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength);
	int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
	int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);
	int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst);
	int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst);
};
