/*-----------------------------------------
* Copyright (c) 2008 Eric Wong
* ����������߲ο������������κ���ҵ��Ϊ
*
* �ļ����ƣ� GPS.h
* �ļ���ʶ�� 
* ժҪ�����ڷ�װGPSͨѶЭ��
*
* ��ǰ�汾�� 1.0
* ���ߣ� ���� Eric Wong
* ������ڣ� 2008��1��29��
*
* ȡ���汾��
* ԭ���ߣ� 
* ������ڣ� 
----------------------------------------*/
#include "StdAfx.h"
#include "GPS.h"

//���캯��
CGPS::CGPS()
{
	m_gpsDev_State = GPS_DEV_NOTOPENED; //GPS״̬
	m_hGpsThread = NULL;				//GPS����߳̾��
	ZeroMemory(&m_gpsCurData,sizeof(m_gpsCurData));  //GPS��ǰ����
 	ZeroMemory(&m_gpsLastData,sizeof(m_gpsLastData)); //GPS��һ������
}

//��������
CGPS::~CGPS(void)
{
}

/*
*�������ܣ���GPS�豸
*��ڲ�����pWnd	:ʹ�ô�GPS��Ĵ�����
		   portNo		:���ں�
		   baud			:������
		   parity		:��żУ��
		   databits		:����λ
		   stopbits		:ֹͣλ
*���ڲ�����(��)
*����ֵ��TRUE:�ɹ���GPS�豸;FALSE:��GPS�豸ʧ��
*/
BOOL CGPS::Open(CWnd *pWnd , /*ӵ���ߴ��ھ��*/
				UINT portNo,		/*���ں�*/
				UINT baud,	/*������*/
				UINT parity, /*��żУ��*/
				UINT databits,		/*����λ*/
				UINT stopbits    /*ֹͣλ*/
				)
{
	m_pWnd = pWnd;  //���洰�ھ��
	//����GPS����߳��˳��¼�
	m_hThreadQuitEvent = CreateEvent(NULL,false,false,L"EVENT_GPS_THREAD");
	//ָ�����ڶ��ص�����
	m_ceSeries.m_OnSeriesRead = GpsOnSeriesRead;
	//��GPS�豸����
	BOOL bResult = m_ceSeries.OpenPort(this,portNo,baud,parity,databits,stopbits);

	if (bResult)
	{
		//���õ�ǰGPS״̬
		m_gpsDev_State = GPS_DEV_OPENED;
		//����GPS״̬�仯��Ϣ
		::PostMessage(m_pWnd->m_hWnd,WM_GPS_STATE_CHANGE_MESSAGE,WPARAM(GPS_DEV_OPENED),1);

		//����GPS״̬����߳�
		m_hGpsThread = CreateThread(NULL,0,GpsCheckThreadFunc,this,0,&m_dwGpsThreadID);
	}
	else
	{
		//���õ�ǰGPS״̬
		m_gpsDev_State = GPS_DEV_NOTOPENED;
		//����GPS״̬�仯��Ϣ
		::PostMessage(m_pWnd->m_hWnd,WM_GPS_STATE_CHANGE_MESSAGE,WPARAM(GPS_DEV_NOTOPENED),1);
	}
	return bResult;
}

/*
*�������ܣ��ر�GPS�豸
*��ڲ�����(��)
*���ڲ�����(��)
*����ֵ��TRUE:�ɹ��ر�GPS�豸;FALSE:�ر�GPS�豸ʧ��
*/
void CGPS::Close()
{
	//���˳�GPS����߳�
	if (m_hGpsThread != NULL)
	{
		//�����߳��˳��ź�
		SetEvent(m_hThreadQuitEvent);
		//�ȴ��߳��˳�
		if (WaitForSingleObject(m_hGpsThread,1000) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hGpsThread,0);
		}
	}

	m_hGpsThread = NULL;
	CloseHandle(m_hThreadQuitEvent);
	//���������ݻص������ÿ�
	m_ceSeries.m_OnSeriesRead = NULL;
	//�ر�GPS����
	m_ceSeries.ClosePort();
	//����GPS״̬
	m_gpsDev_State = GPS_DEV_NOTOPENED;
	//����GPS״̬�仯��Ϣ
	::PostMessage(m_pWnd->m_hWnd,WM_GPS_STATE_CHANGE_MESSAGE,WPARAM(GPS_DEV_NOTOPENED),1);
}

/*
*�������ܣ���ȡGPS�豸״̬
*��ڲ�����(��)
*���ڲ�����(��)
*����ֵ������GPS�豸״̬
*/
GPSDEV_STATE CGPS::GetGpsState()
{
	return m_gpsDev_State;
}


/*
*�������ܣ��õ���ǰGPS����
*��ڲ�����(��)
*���ڲ�����(��)
*����ֵ������GPS�豸��ǰGPS����
*/
GPSData CGPS::GetCurGpsData()
{
	return m_gpsCurData;
}

/*--------------------------------------------------------------------
���������ܡ�: ��pArray������������subString�ַ���������ڣ����ص�ǰλ�ã����򷵻�-1
����ڲ�����: pArray:ָ�����յ��Ļ���������
�����ڲ�����: pArray:ָ�����յ��Ļ��������У���������Ҫ�����ʵ��޸�
������  ֵ��: -1��ʾû���ҵ�ָ�����Ӵ���>=0��ʾ���ֵ�1���Ӵ���λ��
---------------------------------------------------------------------*/
int CGPS::Pos(LPCSTR subString , CByteArray * pArray,int iPos)
{
	//�õ��Ӵ�����
	int subLen = strlen(subString);
	//�õ��������ĳ���
	int bufLen = pArray->GetUpperBound()+1;

	bool aResult = TRUE;
	//
	for ( int i=iPos;i<bufLen-subLen+1;i++)
	{
		aResult = TRUE;
		for (int j=0;j<subLen;j++)
		{
			if (pArray->GetAt(i+j) != *(subString + j))
			{
				aResult = FALSE;
				break;
			}
			int k = 0;
		}
		if (aResult)
		{
			return i;
		}
	}
	return -1;
}


/*
*�������ܣ��ж��Ƿ������ЧGPS����
*��ڲ�����aRecvStr ����������
*���ڲ�����aRecvStr : �������ݣ�outStr:�õ���һ��������GPS����
*����ֵ��TRUE : �ɹ���ʼ�� , FALSE : ��ʼ��ʧ��
*/
BOOL CGPS::HaveValidGPSData(CByteArray * pArray,CString &outStr)
{
	int tmpPos1,tmpPos2;

	tmpPos1 = Pos("$GPRMC",pArray,0);

	tmpPos2 = Pos("$GPRMC",pArray,tmpPos1+6);

	if (tmpPos2 >= 0)  //�����Ѱ�������$GPRMC
	{   
		if (tmpPos1 >= 0 )
		{
			BYTE *pBuf = pArray->GetData();
			char *sBuf = new char[tmpPos2-tmpPos1+1];
			ZeroMemory(sBuf,tmpPos2-tmpPos1+1);
			CopyMemory(sBuf,pBuf+tmpPos1,tmpPos2-tmpPos1+1);
			outStr = CString(sBuf);

			//�ͷ��ڴ�
			delete[] sBuf;
			sBuf = NULL;
			pArray->RemoveAt(0,tmpPos2);
			return TRUE;
		}
	}
	return FALSE;
}

/*
*�������ܣ�����GPS����
*��ڲ�����aRecvStr ��ָ��������GPS��������
*���ڲ�����(��)
*����ֵ��ָCGPSData�ṹ���ָ��,�����Ч��Ϊ��NULL;
*/
PGPSData CGPS::AnalyseGpsData(CString &aRecvStr)
{
	CString tmpTime;
	CString tmpState;
	CString tmpDate;
	CString tmpLONG;
	CString tmpLONGType;
	CString tmpLAT;
	CString tmpLATType;
	CString tmpSpeed;

	LPSTR pStrDate = NULL;
	LPSTR pStrTime = NULL;
	LPSTR pStrLong = NULL;
	LPSTR pStrLongType = NULL;
	LPSTR pStrLat = NULL;
	LPSTR pStrLatType = NULL;
	LPSTR pStrSpeed = NULL;

	PGPSData pGpsData = NULL;
	int tmpPos,tmpPos1;
	int len;

	tmpPos = aRecvStr.Find(',',0); //��1��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);

	//�õ�ʱ��
	tmpTime = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);
	tmpTime = tmpTime.Mid(0,2)+L":"+tmpTime.Mid(2,2)+L":"+tmpTime.Mid(4,2);

	len = tmpTime.GetLength();
	pStrTime = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpTime.GetBuffer(len),len
		,pStrTime,len ,NULL,NULL);

	//����״̬���Ƿ���Ч
	tmpPos = aRecvStr.Find(',',tmpPos+1);  //��2��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	tmpState = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);

	if (tmpState != 'A')//����������Ч������
	{
		if (m_gpsDev_State != GPS_INVALID_DATA)
		{
			//����GPS״̬
			m_gpsDev_State = GPS_INVALID_DATA;
			//����GPS״̬�仯��Ϣ
			::PostMessage(m_pWnd->m_hWnd,WM_GPS_STATE_CHANGE_MESSAGE,WPARAM(GPS_INVALID_DATA),1);
		}
		LocalFree(pStrTime);
		return NULL;
	}
	else  //����������Ч
	{
		if (m_gpsDev_State != GPS_VALID_DATA)
		{
			//����GPS״̬
			m_gpsDev_State = GPS_VALID_DATA;
			//����GPS״̬�仯��Ϣ
			::PostMessage(m_pWnd->m_hWnd,WM_GPS_STATE_CHANGE_MESSAGE,WPARAM(GPS_VALID_DATA),1);
		}
	}

	//�õ�γ��ֵ
	tmpPos = aRecvStr.Find(',',tmpPos+1);//��3��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	tmpLAT	= aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);

	len = tmpLAT.GetLength();
	pStrLat = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpLAT.GetBuffer(len),len
		,pStrLat,len ,NULL,NULL);

	tmpPos = aRecvStr.Find(',',tmpPos+1);//��4��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	tmpLATType = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);

	len = tmpLATType.GetLength();
	pStrLatType = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpLATType.GetBuffer(len),len
		,pStrLatType,len ,NULL,NULL);

	//�õ�����ֵ
	tmpPos = aRecvStr.Find(',',tmpPos+1);//��5��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	tmpLONG = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);

	len = tmpLONG.GetLength();
	pStrLong = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpLONG.GetBuffer(len),len
		,pStrLong,len ,NULL,NULL);

	tmpPos = aRecvStr.Find(',',tmpPos+1);//��6��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	tmpLONGType = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);

	len = tmpLONGType.GetLength();
	pStrLongType = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpLONGType.GetBuffer(len),len
		,pStrLongType,len ,NULL,NULL);

	//�õ�����
	tmpPos = aRecvStr.Find(',',tmpPos+1);////��7��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	tmpSpeed = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);

	len = tmpSpeed.GetLength();
	pStrSpeed = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpSpeed.GetBuffer(len),len
		,pStrSpeed,len ,NULL,NULL);

	tmpPos = aRecvStr.Find(',',tmpPos+1);////��8��ֵ

	//�õ�����
	tmpPos = aRecvStr.Find(',',tmpPos+1);////��9��ֵ
	tmpPos1 = aRecvStr.Find(',',tmpPos+1);
	//��ʽ��һ��
	tmpDate = aRecvStr.Mid(tmpPos+1,tmpPos1-tmpPos-1);
	tmpDate = L"20"+tmpDate.Mid(4,2)+L"-"+tmpDate.Mid(2,2)+L"-"+tmpDate.Mid(0,2);

	len = tmpDate.GetLength();
	pStrDate = LPSTR(LocalAlloc(LMEM_ZEROINIT,len));
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,tmpDate.GetBuffer(len),len
		,pStrDate,len ,NULL,NULL); 

	pGpsData = new GPSData();
	ZeroMemory(pGpsData,sizeof(GPSData));
	//�õ�GPS����ָ��
	CopyMemory(pGpsData->date,pStrDate,10);
	CopyMemory(pGpsData->time,pStrTime,8);
	CopyMemory(pGpsData->latitude_type,pStrLatType,1);
	CopyMemory(pGpsData->latitude,pStrLat,9);
	CopyMemory(pGpsData->longitude_type,pStrLongType,1);
	CopyMemory(pGpsData->longitude,pStrLong,10);
	//����Ĭ���ٶ�0
	FillMemory(pGpsData->speed,5,'0');
	CopyMemory(pGpsData->speed,pStrSpeed,5);

	//�ͷ��ڴ�
	LocalFree(pStrTime);
	LocalFree(pStrDate);
	LocalFree(pStrLatType);
	LocalFree(pStrLat);
	LocalFree(pStrLongType);
	LocalFree(pStrLong);
	LocalFree(pStrSpeed);

	return pGpsData;
}

//GPS���������¼�
void CALLBACK CGPS::GpsOnSeriesRead(void * powner,BYTE* buf,DWORD  dwBufLen)
{
	CGPS * pGps = (CGPS*)powner;
	//�õ�����ָ��
	CByteArray * pArray = &(pGps->m_aRecvBuf);

	//�õ���������С
	int iMaxSize = pArray->GetSize();
	//�õ���������ʹ�õĴ�С
	int iUpperBound = pArray->GetUpperBound();
	for (int i=0;i<dwBufLen;i++)
	{
		pArray->Add(*(buf+i));
	}

	//���յ������ݷ�����������ʾ����
	char* pRecvBuf = new char[dwBufLen+1];
	ZeroMemory(pRecvBuf,dwBufLen+1);
	CopyMemory(pRecvBuf,buf,dwBufLen);

	//���ͽ��մ���ԭʼ����WINDOWS��Ϣ֪ͨ
	//��Ϣ������Ϻ�Ӧ�ͷ��ڴ�
	::PostMessage(pGps->m_pWnd->m_hWnd,WM_GPS_RECV_BUF,WPARAM(pRecvBuf),dwBufLen+1);

	CString strGps;
	//����Ƿ��Ѿ�������Ч��GPS����
	if (pGps->HaveValidGPSData(pArray,strGps))
	{
		PGPSData pGpsData = NULL;
		pGpsData = pGps->AnalyseGpsData(strGps);
		if (pGpsData != NULL) 
		{
			//�����յ���GPS������䵽���µ�ǰ����
			pGps->m_gpsCurData = (*pGpsData);
			//���ͽ�����ЧGPSλ����ϢWINDOWS��Ϣ֪ͨ
			//����Ϣ�������ͷ��ڴ�
			::PostMessage(pGps->m_pWnd->m_hWnd,WM_GPS_RECV_VALID_LONGLAT,WPARAM(pGpsData),0);
		}
	}
}

//���GPS��ǰ����
DWORD WINAPI CGPS::GpsCheckThreadFunc(LPVOID lparam)
{
	//�õ���ǰGPSָ��
	CGPS *pGps = (CGPS*)lparam;

	int iRecCount = 0;
	//Ȼ��ʼ��ѭ����⣬���Ϊ1��
	while (TRUE)
	{
		//�ж������յ���ʱ���Ƿ���ͬ
		if (strcmp(pGps->m_gpsCurData.time,pGps->m_gpsLastData.time) == 0)
		{
			//������1
			iRecCount++;
		}
		else
		{
			//����ǰ��GPS���ݸ�����ʷֵ
			pGps->m_gpsLastData = pGps->m_gpsCurData;
			iRecCount = 0 ;
		}

		//������������û���յ�����
		if (iRecCount == 3)
		{
			if (pGps->m_gpsDev_State != GPS_NODATA)
			{
				//��GPS״̬��Ϊ�������ݡ�
				pGps->m_gpsDev_State = GPS_NODATA;
				//����GPS״̬�ı���Ϣ
				::PostMessage(pGps->m_pWnd->m_hWnd,WM_GPS_STATE_CHANGE_MESSAGE,WPARAM(GPS_NODATA),1);
			}
		}

		//��ʱ1��
		for (int i =0; i<10;i++)
		{
			//�߳��˳�
			if (WaitForSingleObject(pGps->m_hThreadQuitEvent,100) == WAIT_OBJECT_0)
			{
				goto finish;
			}
		}
	}
finish:
	TRACE(L"GPS ����߳��˳�\n");
	return 0;
}