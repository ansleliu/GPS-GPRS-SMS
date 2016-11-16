#include "StdAfx.h"
#include "GPRS.h"

CGPRS::CGPRS(void)
{
	ResetGlobalVarial();
}

CGPRS::~CGPRS(void)
{
	if (pCSerial->m_bOpened== TRUE)
	{
		pCSerial->Close();
	}

	if (pCSerial)
		delete pCSerial;
}
void CGPRS::GPRS_ClosePort()
{
	if (pCSerial->m_bOpened == TRUE)
	{
		pCSerial->Close();
	}

	if (pCSerial)
		delete pCSerial;
}
void CGPRS::ResetGlobalVarial(void)
{
	bSendATCmd = FALSE;					/* ��ǰ�����ڷ�������״̬ */	
	m_RespCnt = 0;							
	m_strRespBuf = _T("");
	ResetEvent(m_ATCmdRespEvent);		/* ��λ GPRS ��Ӧ�¼�*/
}
void CALLBACK CGPRS::OnSerialRead(DWORD UserParamr,char* buf,DWORD bufLen)
{
	CGPRS *pGPRS = (CGPRS *)UserParamr;
	CString strTmp = _T("");
	strTmp = CString(buf);
	if (pGPRS->bSendATCmd == TRUE)
	{//֮ǰ�û������� AT ָ�� 
		pGPRS->m_strRespBuf.Insert(pGPRS->m_RespCnt, strTmp);			// �������� 
		SetEvent(pGPRS->m_ATCmdRespEvent);
		return;
	}
	pGPRS->bSendATCmd = FALSE;							//��λ���ȫ�ֱ���
	pGPRS->m_RespCnt = 0;							
	pGPRS->m_strRespBuf = _T("");
	ResetEvent(pGPRS->m_ATCmdRespEvent);
}


BOOL CGPRS::GPRS_Init(LPCTSTR Port, int BaudRate, DWORD UserParam)
{
	BOOL ret;
	DWORD Trycount = 2;

	do
	{	// ���Գ�ʼ�� GPRS, ��ೢ������
	     Trycount--;

		pCSerial = new CSerial;
		ret = pCSerial->Open((DWORD)this,Port,BaudRate,8,0,1);
		if (ret == FALSE)	//�򿪴���, ����λΪ8,ֹͣλΪ1,��У��λ
		{
			delete pCSerial;
			return FALSE;
		}
		pCSerial->m_OnSeriesRead = OnSerialRead;//���ڽ��ճɹ��ص����� 
		m_UserParam = UserParam;//�����û��������Ĳ��� 
		m_ATCmdRespEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//���� GPRS ��Ӧ�¼�
		for(int i=0;i<=1;i++)
		{
			GPRS_SendATCmd(_T("AT"));//����ģ���Ƿ��ڼ���״̬ 
			GPRS_WaitResponseOK();
		}
		GPRS_SendATCmd(_T("AT+IPR=115200"));							
		ret = GPRS_WaitResponseOK();
		if (ret == FALSE)
		{
			delete pCSerial;
			if (Trycount == 0) 
				return FALSE;
		}
		else
			Trycount = 0;
	}while(Trycount > 0);
	GPRS_SendATCmd(_T("AT+CREG=1"));				
	ret = GPRS_WaitResponseOK();
	if (ret == FALSE)
		return FALSE;
	GPRS_SendATCmd(_T("AT+CLIP=1"));// ����������ʾ 
	ret = GPRS_WaitResponseOK();
	if (ret == FALSE)
		return FALSE;
	GPRS_SendATCmd(_T("AT+CMGF=1"));//����Ϊ�ı�ģʽ 
	ret = GPRS_WaitResponseOK();
	if (ret == FALSE)
		return FALSE;
	GPRS_SendATCmd(_T("AT+CSCS=\"GSM\""));//�����ַ��� 
	ret = GPRS_WaitResponseOK();
	if (ret == FALSE)
		return FALSE;
	return TRUE;
}
BOOL CGPRS::GPRS_SendATCmd(CString strCmd)
{
	int len = strCmd.GetLength();
	char *psendbuf = new char[len + 2];
	
	m_RespCnt = 0;
	bSendATCmd = TRUE;	//���뷢������״̬ 

	for(int i = 0; i < len;i++)
		psendbuf[i] = (char)strCmd.GetAt(i);//ת��Ϊ���ֽ��ַ� 

	psendbuf[len] = '\r';	//��������� 
	psendbuf[len + 1] = '\n';
	//�Ӵ��ڷ�������
	pCSerial->SendData(psendbuf,len + 2);
	delete[] psendbuf;
	return TRUE;
}
BOOL CGPRS::GPRS_WaitResponseOK(void)
{
	DWORD ret;
	BOOL bOK = TRUE;

	while(1)
	{	// �ȴ�** ��, ����ò����ظ�, ��Ϊ����
		ret = WaitForSingleObject(m_ATCmdRespEvent, 500);
		if (ret == WAIT_OBJECT_0)
		{
			if ((m_strRespBuf.Find(_T("ERROR\r\n"), 0) >= 0) ||
				(m_strRespBuf.Find(_T("error\r\n"), 0) >= 0))
			{	//��Ӧ����"ERROR" 
				bOK = FALSE;
				break;
			}
			if ((m_strRespBuf.Find(_T("OK\r\n"), 0) >= 0) ||
				(m_strRespBuf.Find(_T("ok\r\n"), 0) >= 0))
			{	//��Ӧ����"OK"
				bOK = TRUE;
				break;
			}

			ResetEvent(m_ATCmdRespEvent);
		}
		else
		{
			bOK = FALSE;//��Ӧ��ʱ 
			break;
		}
	}

	ResetGlobalVarial();//��λ�õ���ȫ�ֱ���
	if (FALSE == bOK)
		return FALSE;

	return TRUE;
}
BOOL CGPRS::GPRS_WriteMsg(CString Msg)
{
	int len = Msg.GetLength();
	char *psendbuf = new char[len + 3];

	for(int i = 0; i < len;i++)
		psendbuf[i] = (char)Msg.GetAt(i);	// ת��Ϊ���ֽ��ַ�

	psendbuf[len] = '\r';	// ��������� 
	psendbuf[len + 1] = '\n';
	psendbuf[len + 2] = 0x1A;
	//�Ӵ��ڷ������� 
	pCSerial->SendData(psendbuf,len + 3);
	delete[] psendbuf;
	return TRUE;
}
BOOL CGPRS::GPRS_SetShortMSGCenterTel(CString strTelNum)
{
	CString strTmp;

	strTmp = strTelNum + _T(",145");
	bSendATCmd = TRUE;
	GPRS_SendATCmd(strTelNum);

	return (GPRS_WaitResponseOK());
}
int CGPRS::FindMsgPos(int *posnum, int *numlen, int *posmsg, int *msglen)
{
	// ���ſ�ʼλ��
	int posStart = m_strRespBuf.Find(_T("+CMGR:"), 0);					
	if (posStart < 0)
		return -1;
	// �绰���뿪ʼλ��
	*posnum = m_strRespBuf.Find(_T("\"REC READ\",\""), posStart);	
	if (*posnum < 0)
		return -1;
	*posnum = *posnum + wcslen(L"\"REC READ\",\"");			
	// �绰�������λ��
	int posnumend = m_strRespBuf.Find(_T("\""), *posnum + 1);
	if (posnumend < 0)
		return -1;
	// �绰���볤��
	*numlen = posnumend - *posnum;					
	// ���ſ�ʼλ��
	*posmsg = m_strRespBuf.Find(_T("\r\n"), posStart);				
	if (*posmsg < 0)
		return -1;
	*posmsg = *posmsg + wcslen(L"\r\n");		
	// ���Ž���λ��
	int posmsgend = m_strRespBuf.Find(_T("\r\n\r\nOK"), *posmsg);
	*msglen = posmsgend - *posmsg;	
	return 1;
}
BOOL CGPRS::GPRS_ReadShortMessage(DWORD num, CString *strTelCode, CString *Msg)
{
	CString strReadNum;
	BOOL ret, retvalue;

	strReadNum.Format(_T("AT+CMGR=%d"), num);
	bSendATCmd = TRUE;
	GPRS_SendATCmd(strReadNum);	//����: ���� num ������
	while(1)
	{
		ret = WaitForSingleObject(m_ATCmdRespEvent, 2000);
		if (ret == WAIT_OBJECT_0)
		{
			if (m_strRespBuf.Find(_T("ERROR\r\n"), 0) > 0)	//��ӦΪ ERROR
			{
				retvalue = FALSE;
				break;
			}
			
			int posnum, numlen, posmsg, msglen;	
			int pos = FindMsgPos(&posnum, &numlen, &posmsg, &msglen);//Ѱ�Ҷ���λ��
			if (pos > 0)
			{
				*strTelCode = m_strRespBuf.Mid(posnum, numlen);
				*Msg = m_strRespBuf.Mid(posmsg, msglen);// ȡ����������
				retvalue = TRUE;
				break;
			}		
			ResetEvent(m_ATCmdRespEvent);		
		}//end of if (ret == WAIT_OBJECT_0)
		else
		{
			retvalue = FALSE;
			break;
		}
	}
	ResetGlobalVarial();
	if (retvalue == FALSE)
		return FALSE;
	return TRUE;
}
BOOL CGPRS::GPRS_SendShortMessage(CString strTelNum, CString Msg)
{
	BOOL ret, retvalue;
	int pos;

	/* 
	*	1. ���͵绰����
	*/
	CString strNum;
	strNum.Format(_T("AT+CMGS=\"%s\""), strTelNum);
	bSendATCmd = TRUE;
	GPRS_SendATCmd(strNum);	//���͵绰����

	while(1)
	{
		ret = WaitForSingleObject(m_ATCmdRespEvent, 2000);
		if (ret == WAIT_OBJECT_0)	
		{	
			pos = m_strRespBuf.Find(_T('>'), 0);
			if (pos > 0)
			{	//�յ���ʾ��, ���ڿ�����ģ��д�����������
				retvalue = TRUE;
				break;
			}
			pos = m_strRespBuf.Find(_T("ERROR\r\n"), 0);
			if (pos > 0)
			{
				retvalue = FALSE;//GPRS ��Ӧ�ַ��а�����"ERROR"
				break;
			}
		}
		else
		{
			retvalue = FALSE;
			break;
		}
	}

	ResetGlobalVarial();
	if (retvalue == FALSE) return FALSE;

	/* 
	*	2. �ȴ� '>' ��ʾ��, ���Ͷ�������
	*/
	bSendATCmd = TRUE;
	GPRS_WriteMsg(Msg);	
	while(1)
	{
		ret = WaitForSingleObject(m_ATCmdRespEvent,500);
		if (ret == WAIT_OBJECT_0)	
		{
			int pos = m_strRespBuf.Find(_T("ERROR\r\n"), 0);
			if (pos > 0)
			{
				retvalue = FALSE;
				break;
			}
			pos = m_strRespBuf.Find(_T("CMGS"), 0);
			if (pos > 0)
			{	//��Ӧ��ȷ, ���ͳɹ� 
				retvalue = TRUE;
				break;
			}
		}
	}	
	ResetGlobalVarial();
	if (retvalue == FALSE)
		return FALSE;
	return TRUE;
}
BOOL CheckMsgNum(CString str, CString *strNum, CString *strTotal)
{
	int pos1 = str.Find(_T("\"SM\","), 0);			/* ��ʶ�ַ� */
	if (pos1 < 0)
		return FALSE;

	int pos2 = str.Find(_T(','), pos1 + 5);			/* �ָ����� */
	if (pos2 < 0)
		return FALSE;

	*strNum = str.Mid(pos1 + 5, pos2 - (pos1 + 5));	/* ���ж������� */

	int pos3 = str.Find(_T(","), pos2 + 1);
	if (pos3 < 0)
		return FALSE;

	*strTotal = str.Mid(pos2 + 1, pos3 - pos2 - 1); /* �����ݵ����������� */

	return TRUE;
}

BOOL CGPRS::GPRS_CheckMsgNum(CString *strNum, CString *strTotal)
{	
	BOOL ret, retvalue;
	CString strTmp;

	bSendATCmd = TRUE;
	GPRS_SendATCmd(_T("AT+CPMS?"));//��ѯ����
	while(1)
	{
		ret = WaitForSingleObject(m_ATCmdRespEvent, 500);
		if (ret == WAIT_OBJECT_0)	
		{
			int pos = m_strRespBuf.Find(_T("ERROR\r\n"), 0);
			if (pos > 0)
			{
				retvalue = FALSE;
				break;
			}

			if (CheckMsgNum(m_strRespBuf, strNum, strTotal) == TRUE)
			{	// ��ѯ�ɹ� 
				retvalue = TRUE;
				break;
			}
			ResetEvent(m_ATCmdRespEvent);
		}
		else
		{
			retvalue = FALSE;
			break;			
		}
	}
	ResetGlobalVarial();
	if (retvalue == FALSE) 
		return FALSE;
	return TRUE;
}
BOOL CGPRS::GPRS_DeleteShortMsg(DWORD num)
{
	CString strCmd;
	BOOL ret;
	strCmd.Format(_T("AT+CMGD=%d"), num);
	GPRS_SendATCmd(strCmd);
	ret = GPRS_WaitResponseOK();
	if (ret == FALSE)
		return FALSE;
	return TRUE;
}