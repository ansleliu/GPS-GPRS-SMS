/*-----------------------------------------
* Copyright (c) 2008 Eric Wong
* ����������߲ο������������κ���ҵ��Ϊ
*
* �ļ����ƣ� CESeries.h
* �ļ���ʶ�� 
* ժҪ�����ڷ�װWINCE ����ͨѶ
*
* ��ǰ�汾�� 1.0
* ���ߣ� ���� Eric Wong
* ������ڣ� 2008��1��17��
*
* ȡ���汾��
* ԭ���ߣ� 
* ������ڣ� 
----------------------------------------*/
#pragma once

//���崮�ڽ������ݺ�������
typedef void (CALLBACK* ONSERIESREAD)(void * pOwner /*������ָ��*/
									  ,BYTE* buf  /*���յ��Ļ�����*/
									  ,DWORD dwBufLen /*���յ��Ļ���������*/);


class CCESeries
{
public:
	CCESeries(void);
	~CCESeries(void);
public:
	//�򿪴���
	BOOL OpenPort(void* pOwner,/*ָ��ָ��*/
				  UINT portNo	= 1,		/*���ں�*/
				  UINT baud		= 9600,	/*������*/
				  UINT parity	= NOPARITY, /*��żУ��*/
				  UINT databits	= 8,		/*����λ*/
				  UINT stopbits	= 0        /*ֹͣλ*/
				  );
	//�رմ���
	void ClosePort();
	//ͬ��д������
	BOOL WriteSyncPort(const BYTE*buf , DWORD bufLen);
	//���ô��ڶ�ȡ��д�볬ʱ
	BOOL SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts);
	//�õ������Ƿ��
	BOOL GetComOpened();
private:
    //���ڶ��̺߳���
    static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);
private:
	//�رն��߳�
	void CloseReadThread();
private:
    //�Ѵ򿪵Ĵ��ھ��
	HANDLE	m_hComm;
	//���߳̾��
	HANDLE m_hReadThread;
	//���߳�ID��ʶ
	DWORD m_dwReadThreadID;
	//���߳��˳��¼�
	HANDLE m_hReadCloseEvent;
	BOOL m_bOpened; //�����Ƿ��
	void * m_pOwner; //ָ��������ָ��
public:
	ONSERIESREAD m_OnSeriesRead; //���ڶ�ȡ�ص�����
};
