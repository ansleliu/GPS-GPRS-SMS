
// GPRS_SMSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GPRS_SMS.h"
#include "GPRS_SMSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGPRS_SMSDlg �Ի���




CGPRS_SMSDlg::CGPRS_SMSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGPRS_SMSDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGPRS_SMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGPRS_SMSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SET_MSGCENTR_TELCODE, &CGPRS_SMSDlg::OnBnClickedSetMsgcentrTelcode)
	ON_BN_CLICKED(IDC_SENDMSG, &CGPRS_SMSDlg::OnBnClickedSendmsg)
END_MESSAGE_MAP()


// CGPRS_SMSDlg ��Ϣ�������

BOOL CGPRS_SMSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemText(IDC_SENDMSG_TELCODE, _T("18710846798"));		
	SetDlgItemText(IDC_EDIT_SENDMSG, _T("ansleliu"));
	BOOL ret = m_GPRS.GPRS_Init(_T("COM4:"), 115200, (DWORD)this);
	if (ret == FALSE)
	{
		MessageBox(_T("GPRS��ʼ��ʧ��, �����Ƿ�װ��ȷ."));
	}	
	SetDlgItemText(IDC_MSGCENTR_TELCODE, _T("+8613800290500"));
	m_GPRS.GPRS_DeleteShortMsg(1);		/* ɾ���� 1 ������*/
	SetTimer(1, 1000, NULL);			/* ÿ 1 ���ȡһ�ζ��� */

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGPRS_SMSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGPRS_SMSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGPRS_SMSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGPRS_SMSDlg::OnTimer(UINT nIDEvent) 
{
	BOOL ret;
	CString strTelCode, strMsg;
	ret = m_GPRS.GPRS_ReadShortMessage(1, &strTelCode, &strMsg);// ��ȡ�� 0 ������
	if (ret == TRUE)
	{
		for (int i = 0; i < strMsg.GetLength(); i++)
		{	
			if ((char)strMsg.GetAt(i) == '\n')	//�й����ַ� 
			if ((char)strMsg.GetAt(i - 1) != '\r')	// ��û�лس��ַ� 
			{
				strMsg.Insert(i, '\r');//����س���
			}
		}
		SetDlgItemText(IDC_RECVMSG_TELCODE, strTelCode);//��ʾ�绰���� 
		SetDlgItemText(IDC_DISP_RECVMSG, strMsg);	// ��ʾ��������
		m_GPRS.GPRS_DeleteShortMsg(1);	// ɾ������
	}
	
	SetTimer(1, 1000, NULL);	//ÿ 1 ���ȡһ�ζ��� 
	CDialog::OnTimer(nIDEvent);
}
void CGPRS_SMSDlg::OnBnClickedSetMsgcentrTelcode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strCode;
	GetDlgItemText(IDC_MSGCENTR_TELCODE, strCode);
	BOOL ret = m_GPRS.GPRS_SetShortMSGCenterTel(strCode);	//���ö����к��� 
	if (ret == TRUE)
		MessageBox(_T("���ö������ĺ���ɹ�."));
	else
		MessageBox(_T("���ö������ĺ���ʧ��."));
}

void CGPRS_SMSDlg::OnBnClickedSendmsg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTelCode, strMsg;

	GetDlgItemText(IDC_SENDMSG_TELCODE, strTelCode);//��ȡ���Ͷ��ŵ绰���뼰����	
	GetDlgItemText(IDC_EDIT_SENDMSG, strMsg);	

	if ((strTelCode == "") || (strMsg == ""))
	{	//�ж����������Ƿ���ȷ 
		MessageBox(_T("�绰�����������ݲ���Ϊ��."));
		return;
	}

	BOOL ret = m_GPRS.GPRS_SendShortMessage(strTelCode, strMsg);//���Ͷ���
	if (ret == TRUE)
		MessageBox(_T("���ŷ��ͳɹ�."));
	else
		MessageBox(_T("���ŷ���ʧ��."));
}
