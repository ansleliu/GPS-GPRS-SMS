
// SMSDemoDlg.h : ͷ�ļ�
//

#pragma once
#include"CESeries.h"
#include"Encode.h"
#include "afxwin.h"

typedef struct {
	int len;
	char data[16384];
} SM_BUFF;

// CSMSDemoDlg �Ի���
class CSMSDemoDlg : public CDialog
{
// ����
public:
	CSMSDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SMSDEMO_DIALOG };
	CButton m_sendmsgs;
	CButton m_readmsg;
	CListBox m_list;
	CButton m_open;
	CButton m_close;
	CButton m_qc;
	CButton m_initmsg;
	CButton m_initgprs;
	CButton m_gd;
	CButton m_bt;
	CButton m_bh;
	CString num_to;
	CString num_from;
	CString m_num;
	CString m_nums;
	CString m_hnum;
	CString m_message;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
    BOOL ReadMessage();//��ȡ����
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOpenport();
	afx_msg void OnBnClickedBclose();
	afx_msg void OnBnClickedInitgprs();
	afx_msg void OnBnClickedInitsms();
	afx_msg void OnBnClickedBrest();
	afx_msg void OnBnClickedBexit();
	afx_msg void OnBnClickedSendmesg();
	afx_msg void OnBnClickedReadmesg();
	afx_msg void OnBnClickedBbh();
	afx_msg void OnBnClickedBqc();
	afx_msg void OnBnClickedBbt();
	afx_msg void OnBnClickedBgd();
	afx_msg void OnBnClickedB1();
	afx_msg void OnBnClickedB2();
	afx_msg void OnBnClickedB3();
	afx_msg void OnBnClickedB4();
	afx_msg void OnBnClickedB5();
	afx_msg void OnBnClickedB6();
	afx_msg void OnBnClickedB7();
	afx_msg void OnBnClickedB8();
	afx_msg void OnBnClickedB9();
	afx_msg void OnBnClickedBf1();
	afx_msg void OnBnClickedB0();
	afx_msg void OnBnClickedBf2();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void GetNum(CString str);
	DECLARE_MESSAGE_MAP()

public:
	BOOL waitok();
//	typedef void (CALLBACK* ONSERIESREAD)(CWnd* pPortOwner,BYTE* buf,int bufLen);
//	ONSERIESREAD m_OnSeriesRead; 
	CWnd* pPortOwner;//������ָ��
	CEncode encode;			//������ָ��
	CCESeries serial;   //���������
	BOOL sendmsg;         //�����ж�
	BOOL m_bInitGSM;        //GPRS״̬
	BOOL m_bReadMsg;        //���Ž���״̬��
	BOOL bSendATCmd;		//���ŷ���״̬
	CString strNum;          //��ȡ����������
	CString m_strMsg;   //��������
	CListCtrl m_ListMsg;   //����ָ��
	char pdu[512];          //PDU�ַ�
	void Reset(void);
	void EnableBtn(bool bvalue);
	
private:
    int FindMsgPos(int *posnum, int *numlen, int *posmsg, int *msglen,CString str);//�ص�����
	HANDLE m_hReadCloseEvent;
	static void CALLBACK m_OnSeriesReads(void* pPortOwner,BYTE* buf,DWORD bufLen); 
};
