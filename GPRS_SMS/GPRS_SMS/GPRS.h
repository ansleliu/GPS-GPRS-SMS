#pragma once
#include "Serial.h"

class CGPRS
{
public:
	CGPRS(void);
	~CGPRS(void);
	BOOL GPRS_Init(LPCTSTR Port, int BaudRate, DWORD UserParam);
	BOOL GPRS_DeleteShortMsg(DWORD num);
	BOOL GPRS_ReadShortMessage(DWORD num, CString *strTelCode, CString *Msg);
	BOOL GPRS_SendShortMessage(CString strTelNum, CString Msg);
	BOOL GPRS_SetShortMSGCenterTel(CString strTelNum);
	BOOL GPRS_CheckMsgNum(CString *strNum, CString *strTotal);
	BOOL GPRS_SetUpPPPLink(void);
	BOOL GPRS_SetUpTCPLink(CString remoteIP);
	BOOL GPRS_SendData(CString senddata,CString num,CString len);
	CString GPRS_AskForPPPState(void);
	void GPRS_ClosePort(void);
private:
	CSerial* pCSerial;
	DWORD m_UserParam;	//ʹ�ñ�ģ���û�
	HANDLE m_ATCmdRespEvent;//AT ����ظ��¼�
	CString m_strRespBuf;// �ظ��������ַ���
	DWORD m_RespCnt;
	BOOL bSendATCmd;// �Ƿ��������� 
	void ResetGlobalVarial(void);
	BOOL GPRS_SendATCmd(CString strCmd);// ���� AT ���� 
	BOOL GPRS_WriteMsg(CString Msg);// ���Ͷ������� 
	BOOL GPRS_WaitResponseOK(void);	//�ȴ� AT �������Ӧ
	int FindMsgPos(int *posnum, int *numlen, int *posmsg, int *msglen);
	static void CALLBACK OnSerialRead(DWORD UserParam,
									  char* buf,
									  DWORD bufLen);
};
