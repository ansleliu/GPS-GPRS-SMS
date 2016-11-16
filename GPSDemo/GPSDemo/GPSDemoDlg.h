// GPSDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "gps.h"
// CGPSDemoDlg �Ի���
class CGPSDemoDlg : public CDialog
{
// ����
public:
	CGPSDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GPSDEMO_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	// ��ʾGPS���ڽ��յ�����
	afx_msg LONG OnRecvSerialData(WPARAM wParam,LPARAM lParam);
	// ��ʾGPS״̬�仯
	afx_msg LONG OnGpsStateChange(WPARAM wParam,LPARAM lParam);
	// ��ʾGPS��Ч����
	afx_msg LONG OnRecvValidGps(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CGPS *m_pGps;  //GPS�����
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnClear();
};
