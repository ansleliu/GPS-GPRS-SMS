// GPSDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GPSDemo.h"
#include "GPSDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//����GPS���ں�
#define GPS_COM_NO 8
// CGPSDemoDlg �Ի���

CGPSDemoDlg::CGPSDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pGps = NULL;  //GPS����� 
}

void CGPSDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGPSDemoDlg, CDialog)

	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GPS_RECV_BUF,OnRecvSerialData)
	ON_MESSAGE(WM_GPS_STATE_CHANGE_MESSAGE,OnGpsStateChange)
	ON_MESSAGE(WM_GPS_RECV_VALID_LONGLAT,OnRecvValidGps)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CGPSDemoDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CGPSDemoDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CGPSDemoDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CGPSDemoDlg ��Ϣ�������

BOOL CGPSDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


// ��ʾGPS���ڽ��յ�����
LONG CGPSDemoDlg::OnRecvSerialData(WPARAM wParam,LPARAM lParam)
{
	//�õ�ԭʼ����
	char * aRecv = (char *)wParam;
	
	CString strRecv = L"";
	CString strRecv1 = L"";
	//
	CEdit *pEdtRecv = (CEdit*)GetDlgItem(IDC_EDT_RECV);

	//�õ����տ�ԭ�����ı�
	pEdtRecv->GetWindowTextW(strRecv1);
	strRecv = CString(aRecv);
	//׷���½��յ����ı�
	pEdtRecv->SetWindowTextW(strRecv1+strRecv);
	
	//�ͷ��ڴ�
	delete[] aRecv;
	aRecv = NULL;
	return 0;

}

// ��ʾGPS״̬�仯
LONG CGPSDemoDlg::OnGpsStateChange(WPARAM wParam,LPARAM lParam)
{
	//GPS״̬
	GPSDEV_STATE gpsStatus = (GPSDEV_STATE)wParam;
	CString strHint = L"";
	//
	CStatic *pLblStatus = (CStatic*)GetDlgItem(IDC_LBL_GPS_STATUS);

	switch (gpsStatus)
	{
		//GPS��Ч����
	case GPS_VALID_DATA:
		{
			strHint = L"������Ч";
		}
		break;
		//GPS��Ч����
	case GPS_INVALID_DATA:
		{
			strHint = L"������Ч";
		}
		break;
		//GPS�����豸δ��
	case GPS_DEV_NOTOPENED:
		{
			strHint = L"����δ��";
		}
		break;
		//GPS�����豸��
	case GPS_DEV_OPENED:
		{
			strHint = L"���ڴ�";
		}
		break;
		//GPS������
	case GPS_NODATA:
		{
			strHint = L"������";
		}
		break;
	default:
		break;
	}

	//����GPS״̬����
	pLblStatus->SetWindowTextW(strHint);
	return 0;
}

// ��ʾGPS��Ч����
LONG CGPSDemoDlg::OnRecvValidGps(WPARAM wParam,LPARAM lParam)
{
	//�õ�GPS����
	GPSData *pGps = (GPSData*)wParam;

	CEdit *pEdtLong = (CEdit*)GetDlgItem(IDC_EDT_LONG);
	CEdit *pEdtLat = (CEdit*)GetDlgItem(IDC_EDT_LAT);

	CString strLong = CString(pGps->longitude);  //����
	CString strLat = CString(pGps->latitude);	 //γ��
 
	//��ʾ��γ����Ϣ
	pEdtLong->SetWindowTextW(strLong);
	pEdtLat->SetWindowTextW(strLat);

	//�ͷ��ڴ�
	delete pGps;
	pGps = NULL;
	return 0;
}
//��
void CGPSDemoDlg::OnBnClickedBtnOpen()
{
	CString strTmp;
	//�ȼ���Ƿ��ͷ�
	if (m_pGps != NULL)
	{
		m_pGps->Close();
		delete m_pGps;
		m_pGps = NULL;
	}
	//����GPS�߼���
	m_pGps = new CGPS();
	//
	if (m_pGps->Open(this,GPS_COM_NO,115200))
	{
		AfxMessageBox(L"GPS�豸�򿪳ɹ�");
	}
	else
	{
		AfxMessageBox(L"GPS �豸��ʧ��");
		return ;
	}
}

//�ر�
void CGPSDemoDlg::OnBnClickedBtnClose()
{
	//�ͷ�GPS����
	if (m_pGps != NULL)
	{
		m_pGps->Close();
		delete m_pGps;
		m_pGps = NULL;
	}
}
//���
void CGPSDemoDlg::OnBnClickedBtnClear()
{
	//
	CEdit *pEdtRecv = (CEdit*)GetDlgItem(IDC_EDT_RECV);

	pEdtRecv->SetWindowTextW(L"");

}