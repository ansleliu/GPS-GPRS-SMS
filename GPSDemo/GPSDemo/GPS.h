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
#pragma once

#include "CESeries.h"

//����GPS���ݽ�����ʾ
#define WM_GPS_RECV_BUF				WM_USER + 101
//����GPS״̬�ı���Ϣ
#define WM_GPS_STATE_CHANGE_MESSAGE	WM_USER + 102
//�����յ���ȷ��GPSλ����Ϣ
#define WM_GPS_RECV_VALID_LONGLAT	WM_USER + 103

//����GPS�豸״̬����
enum GPSDEV_STATE
{
	GPS_VALID_DATA = 0,   //��ȡ��Ч����
	GPS_INVALID_DATA,//��ȡ��Ч����
	GPS_DEV_NOTOPENED,  //GPS����δ��
	GPS_DEV_OPENED, //GPS�����Ѵ�
	GPS_NODATA//GPSδ�յ�����
};

//GPS���ݽṹ
typedef struct _GPSData
{
	char date[11] ; //Gps��������
	char time[9] ;  //Gps����ʱ��
	char latitude_type[2]; //γ�����ͣ���γ����γ
	char latitude[10] ; //γ��ֵ
	char longitude_type[2]; //�������ͣ�����������
	char longitude[11] ;//����ֵ
	char speed[6];//�ٶ�
	char starNum; //������Ŀ
}GPSData,*PGPSData;

class CGPS
{
public:
	CGPS(void);
	~CGPS(void);
public:
	//��GPS�豸
	BOOL Open(	   CWnd *pWnd , /*ӵ���ߴ��ھ��*/
				  UINT portNo	= 1,		/*���ں�*/
				  UINT baud		= 9600,	/*������*/
				  UINT parity	= NOPARITY, /*��żУ��*/
				  UINT databits	= 8,		/*����λ*/
				  UINT stopbits	= 0 );
	//�ر�GPS�豸
	void Close();
	//��ȡGPS�豸״̬
	GPSDEV_STATE GetGpsState();
	//�õ���ǰGPS����
	GPSData GetCurGpsData();

private:
	//�ڻ������в������ַ���
	int Pos(LPCSTR subString , CByteArray * pArray,int iPos);
	//�ж��Ƿ������ЧGPS����
	BOOL HaveValidGPSData(CByteArray * pArray,/*�����Ļ���������*/
		CString &outStr);
	//����GPS����
	PGPSData AnalyseGpsData(CString &aRecvStr);
private:
	//���ڽ������ݻص�����
	static void CALLBACK GpsOnSeriesRead(void* pOwner,BYTE* buf,DWORD dwBufLen);
private:
	//GPS���ݼ���̺߳���
	static DWORD WINAPI GpsCheckThreadFunc(LPVOID lparam);
private:
	GPSDEV_STATE m_gpsDev_State; //GPS��ǰ�豸״̬
	GPSData  m_gpsCurData;       // GPS��ǰ����
	GPSData m_gpsLastData;		//GPS��һ������
	CCESeries m_ceSeries;        //GPS����ͨѶ��
	CByteArray  m_aRecvBuf  ;   //���ջ�����
	CWnd *m_pWnd; //�洢��������
	HANDLE m_hThreadQuitEvent;	//�߳��˳��¼�
	HANDLE m_hGpsThread;		//GPS����߳̾��
	DWORD m_dwGpsThreadID;		//GPS����̱߳�ʶ
};
