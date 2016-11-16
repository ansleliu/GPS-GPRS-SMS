#pragma once

//���崮�ڽ������ݺ�������
typedef void (CALLBACK* ONSERIESREAD)(void* pOwner /*������ָ��*/
									  ,BYTE* buf  /*���յ��Ļ�����*/
									  ,DWORD dwBufLen /*���յ��Ļ���������*/);
class CCESeries
{
public:
	CCESeries(void);
	~CCESeries(void);
public:
	//�򿪴���
	BOOL OpenPort(CWnd* pOwner,/*ָ��ָ��*/
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
	BOOL WriteChar(char *buf,int nsize);
	BOOL WriteChar(CString str);
	BOOL WriteChar(LPCTSTR lpString);
	//���ô��ڶ�ȡ��д�볬ʱ
	BOOL SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts);
	//�õ������Ƿ��
	BOOL GetComOpened();
private:
    //���ڶ��̺߳���
    static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);
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
	CWnd* m_pOwner; //ָ��������ָ��
public:
	ONSERIESREAD m_OnSeriesRead; //���ڶ�ȡ�ص�����
};
