
// GPRS_SMSDlg.h : ͷ�ļ�
//

#pragma once
#include "GPRS.h"
// CGPRS_SMSDlg �Ի���
class CGPRS_SMSDlg : public CDialog
{
// ����
public:
	CGPRS_SMSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GPRS_SMS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CGPRS m_GPRS;
public:
	afx_msg void OnBnClickedSetMsgcentrTelcode();
	afx_msg void OnBnClickedSendmsg();
};
