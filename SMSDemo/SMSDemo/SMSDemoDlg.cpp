
// SMSDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMSDemo.h"
#include "SMSDemoDlg.h"
#include <atlconv.h>
#include <afx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GSM_7BIT		0
#define GSM_8BIT		4
#define GSM_UCS22		8

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


// CSMSDemoDlg �Ի���




CSMSDemoDlg::CSMSDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSDemoDlg::IDD, pParent)
	, num_to(_T(""))
	, num_from(_T("������"))
	, m_num(_T("+8613800290500"))
	, m_nums(_T("+8618710846259"))
	, m_hnum(_T("0"))
	, m_message(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMSDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SENDMESG, m_sendmsgs);
	DDX_Control(pDX, IDC_READMESG, m_readmsg);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_OPENPORT, m_open);
	DDX_Control(pDX, IDC_BCLOSE, m_close);
	DDX_Control(pDX, IDC_BQC, m_qc);
	DDX_Control(pDX, IDC_INITSMS, m_initmsg);
	DDX_Control(pDX, IDC_INITGPRS, m_initgprs);
	DDX_Control(pDX, IDC_BGD, m_gd);
	DDX_Control(pDX, IDC_BBT, m_bt);
	DDX_Control(pDX, IDC_BBH, m_bh);
	DDX_Text(pDX, IDC_EDIT1, num_to);
	DDX_Text(pDX, IDC_EDIT2, num_from);
	DDX_Text(pDX, IDC_EDIT3, m_num);
	DDX_Text(pDX, IDC_EDIT4, m_nums);
	DDX_Text(pDX, IDC_EDIT5, m_hnum);
	DDX_Text(pDX, IDC_MESSAGE, m_message);
}

BEGIN_MESSAGE_MAP(CSMSDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPENPORT, &CSMSDemoDlg::OnBnClickedOpenport)
	ON_BN_CLICKED(IDC_BCLOSE, &CSMSDemoDlg::OnBnClickedBclose)
	ON_BN_CLICKED(IDC_INITGPRS, &CSMSDemoDlg::OnBnClickedInitgprs)
	ON_BN_CLICKED(IDC_INITSMS, &CSMSDemoDlg::OnBnClickedInitsms)
	ON_BN_CLICKED(IDC_BREST, &CSMSDemoDlg::OnBnClickedBrest)
	ON_BN_CLICKED(IDC_BEXIT, &CSMSDemoDlg::OnBnClickedBexit)
	ON_BN_CLICKED(IDC_SENDMESG, &CSMSDemoDlg::OnBnClickedSendmesg)
	ON_BN_CLICKED(IDC_READMESG, &CSMSDemoDlg::OnBnClickedReadmesg)
	ON_BN_CLICKED(IDC_BBH, &CSMSDemoDlg::OnBnClickedBbh)
	ON_BN_CLICKED(IDC_BQC, &CSMSDemoDlg::OnBnClickedBqc)
	ON_BN_CLICKED(IDC_BBT, &CSMSDemoDlg::OnBnClickedBbt)
	ON_BN_CLICKED(IDC_BGD, &CSMSDemoDlg::OnBnClickedBgd)
	ON_BN_CLICKED(IDC_B1, &CSMSDemoDlg::OnBnClickedB1)
	ON_BN_CLICKED(IDC_B2, &CSMSDemoDlg::OnBnClickedB2)
	ON_BN_CLICKED(IDC_B3, &CSMSDemoDlg::OnBnClickedB3)
	ON_BN_CLICKED(IDC_B4, &CSMSDemoDlg::OnBnClickedB4)
	ON_BN_CLICKED(IDC_B5, &CSMSDemoDlg::OnBnClickedB5)
	ON_BN_CLICKED(IDC_B6, &CSMSDemoDlg::OnBnClickedB6)
	ON_BN_CLICKED(IDC_B7, &CSMSDemoDlg::OnBnClickedB7)
	ON_BN_CLICKED(IDC_B8, &CSMSDemoDlg::OnBnClickedB8)
	ON_BN_CLICKED(IDC_B9, &CSMSDemoDlg::OnBnClickedB9)
	ON_BN_CLICKED(IDC_BF1, &CSMSDemoDlg::OnBnClickedBf1)
	ON_BN_CLICKED(IDC_B0, &CSMSDemoDlg::OnBnClickedB0)
	ON_BN_CLICKED(IDC_BF2, &CSMSDemoDlg::OnBnClickedBf2)
	ON_LBN_DBLCLK(IDC_LIST1, &CSMSDemoDlg::OnLbnDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSMSDemoDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CSMSDemoDlg ��Ϣ�������

BOOL CSMSDemoDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_bt.EnableWindow(false);
	m_bh.EnableWindow(false);
	m_gd.EnableWindow(false);
	m_close.EnableWindow(false);
	m_initgprs.EnableWindow(false);
	m_qc.EnableWindow(true);
	m_initmsg.EnableWindow(false);
	m_sendmsgs.EnableWindow(false);
	pPortOwner=this;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSMSDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSMSDemoDlg::OnPaint()
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
HCURSOR CSMSDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSMSDemoDlg::OnBnClickedOpenport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL m_Open=serial.OpenPort(this,4,115200);//��ʼ���˿�
	if(!m_Open)	
	{
		MessageBox(L"���ڳ�ʼ��ʧ��!",L"ϵͳ��Ϣ",MB_OK);
		return;
	}
	MessageBox(L"���ڴ򿪳ɹ���",L"��ʾ��");
	serial.m_OnSeriesRead = m_OnSeriesReads;//���̻߳ص�����
	m_close.EnableWindow(true);
	m_initgprs.EnableWindow(true);
	m_qc.EnableWindow(true);
//	m_open.EnableWindow(false);
}

void CSMSDemoDlg::OnBnClickedBclose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	serial.ClosePort();	
	m_open.EnableWindow(true);
	m_initgprs.EnableWindow(false);
	m_qc.EnableWindow(false);
}

void CSMSDemoDlg::OnBnClickedInitgprs()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CString at="AT\r\n";  
//	m_bInitGSM=serial.WriteChar(at);
	char * buf  =NULL;  //���巢�ͻ�����
	DWORD dwBufLen = 0;   //���巢�ͻ���������
	CString strSend = L"AT\r\n";
	//�������͵��ַ���ת���ɵ��ֽڣ����з���
	buf = new char[strSend.GetLength()*2+1];
	ZeroMemory(buf,strSend.GetLength()*2+1);
	//ת���ɵ��ֽڽ��з���	
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strSend.GetBuffer(strSend.GetLength())
	   ,strSend.GetLength(),buf,strSend.GetLength()*2,NULL,NULL);
	dwBufLen = strlen(buf) + 1;
	//�����ַ���
	m_bInitGSM=serial.WriteSyncPort((BYTE*)buf,dwBufLen);
	if(m_bInitGSM==false)
	{
		MessageBox(L"modem�޷�����!",L"ϵͳ��Ϣ",MB_OK);
		serial.ClosePort();
		delete[] buf;
		buf = NULL;
		return;
	}
	//�ͷ��ڴ�
	delete[] buf;
	buf = NULL;
	m_bh.EnableWindow(true);
	m_initmsg.EnableWindow(true);
}

void CSMSDemoDlg::OnBnClickedInitsms()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString ATE=L"ATE0\r\n";                          //����������
	CString CMGF=L"AT+CMGF=0\r\n";						//ѡ�����Ϣ���͸�ʽ��Ϊ0��PDU��Ϊ1ΪTEXT��ʽ
	CString CNMI=L"AT+CNMI=2,1,0,0,0\r\n";               //��ʾ���յ��Ķ���
	serial.WriteChar(ATE);
	serial.WriteChar(CMGF);
	serial.WriteChar(CNMI);
	m_sendmsgs.EnableWindow(true);
}

void CSMSDemoDlg::OnBnClickedBrest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	serial.m_OnSeriesRead=m_OnSeriesReads;
	m_open.EnableWindow(true);
	m_bt.EnableWindow(false);
	m_bh.EnableWindow(false);
	m_gd.EnableWindow(false);
	m_close.EnableWindow(false);
	m_initgprs.EnableWindow(false);
	m_qc.EnableWindow(false);
	m_initmsg.EnableWindow(false);
	Reset();	
	UpdateData(false);
}

/*��λ����*/
void CSMSDemoDlg::Reset()
{
	serial.ClosePort();
	sendmsg = FALSE;
	m_bInitGSM=FALSE;
	m_bReadMsg=FALSE;
	m_strMsg="";
	num_to="";
	num_from="������";
	UpdateData(false);
}
void CSMSDemoDlg::OnBnClickedBexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(AfxMessageBox(L"�˳�ϵͳ��", MB_YESNO) == IDYES)
	{
		MessageBox(L"�˳�ϵͳ��",L"��ʾ��",MB_OK|MB_ICONASTERISK);
		SetEvent(m_hReadCloseEvent);
		Reset();
		serial.ClosePort();
		Sleep(500);
		AfxGetMainWnd()->PostMessage(WM_QUIT);
		CDialog::OnCancel();
	}
}

/*�������ݻص�����*/
void CALLBACK CSMSDemoDlg::m_OnSeriesReads(void* pPortOwner,BYTE* buf,DWORD bufLen)
{	
	CSMSDemoDlg *pDlg = (CSMSDemoDlg *)pPortOwner;
	pDlg->UpdateData(TRUE);
	pDlg->strNum=_T("");
	LPTSTR pStr = pDlg->strNum.GetBuffer(bufLen);
	MultiByteToWideChar(CP_ACP, 0, (char *)buf, bufLen, pStr, bufLen);//ת��ΪUnicode����
	pDlg->strNum.ReleaseBuffer();
	if ((pDlg->strNum.Find(_T("RING")) >= 0) || 
		(pDlg->strNum.Find(_T("ring")) >= 0))
	{													//�е绰����� 		
//		pDlg->GetNum(pDlg->strNum);
		pDlg->m_bt.EnableWindow(true);
	}
	else
	if ((pDlg->strNum.Find(_T("NO CARRIER")) >= 0) || 
	(pDlg->strNum.Find(_T("no carrier")) >= 0))				
	{													// �����, �Է��һ� 
		CString st=L"AT+CHUP\r\n";
		pDlg->serial.WriteChar(st);
		pDlg->MessageBox(L"�Է��ѹһ������ڷ��͹һ�����",L"ϵͳ��Ϣ");

	}
	else
	if ((pDlg->strNum.Find(_T("ERROR"), 0) >= 0) ||
		(pDlg->strNum.Find(_T("error"), 0) >= 0))
	{													// ��Ӧ����"ERROR" 
		pDlg->MessageBox(L"����ʧ��",L"ϵͳ��Ϣ");	
	}
	if ((pDlg->strNum.Find(_T("OK"), 0) >= 0) ||
		(pDlg->strNum.Find(_T("ok"), 0) >= 0))
	{	
	//	pDlg->MessageBox(L"�ҵ�OK",L"ϵͳ��Ϣ");
	// ��Ӧ����"OK" 
	}
	else
	if((pDlg->strNum.Find(_T("+CMGL:")),0)||(pDlg->strNum.Find(_T("+cmgl:")),0))
	{
			int posnum, numlen, posmsg, msglen;	
			int pos=pDlg->FindMsgPos(&posnum, &numlen, &posmsg, &msglen,pDlg->strNum);
	}
	else
	if((pDlg->strNum.Find(_T("+CMS ERROR")),0)||(pDlg->strNum.Find(_T("+cms error")),0))
	{
		pDlg->MessageBox(L"�����Ŵ���",L"ϵͳ��Ϣ");
	}
	else
	if((pDlg->strNum.Find(_T("CMTI")),0)||(pDlg->strNum.Find(_T("cmti")),0))
	{
		pDlg->MessageBox(L"���¶���Ϣ",L"ϵͳ��Ϣ");
		int i=pDlg->strNum.GetLength();
//		char cmd[16];
		int index=(int)pDlg->strNum[i];
		if(AfxMessageBox(L"�Ƿ��ȡ����Ϣ��", MB_YESNO) == IDYES)
		{
			CString cmd;
//			sprintf(cmd,format, index);	// ��������
			cmd.Format(_T("AT+CMGR=%d\r"),index);
			pDlg->serial.WriteChar(cmd);
			Sleep(100);
		}
	}
	else
	if(pDlg->strNum.Find(_T("CMGR"), 0))
	{
	//���������Ϣ
		SM_PARAM *pMsg = NULL;
		char* ptr;
		SM_BUFF *pBuff = NULL;
		CString str=L"����ID:";
		CString str1=L"��������:";
		ptr=pBuff->data;
		while((ptr = strstr(ptr,"+CMGL:")) != NULL)
		{
			ptr += 6;		// ����"+CMGL:", ��λ�����
			sscanf(ptr, "%d", &pMsg->index);	// ��ȡ���
//			str+=&pMsg->index;
			ptr = strstr(ptr, "\r\n");	// ����һ��
			if (ptr != NULL)
			{
				str+=str1;
				ptr += 2;		// ����"\r\n", ��λ��PDU		
				pDlg->encode.gsmDecodePdu(ptr, pMsg);	// PDU������
//				str+=&pMsg->TPA;
				str+=" ���ŷ���ʱ��:";
//				str+=&pMsg->TP_SCTS;
			//	m_list.AddString(str);
//				m_message="";
//				m_message+=str;
			}
		}

	}
	else
	if((pDlg->strNum.Find(_T("CMGL")),0)||(pDlg->strNum.Find(_T("CMGL")),0))
	{
		SM_PARAM *pMsg;
		char* ptr;
		SM_BUFF *pBuff;
		CString str=L"����ID:";
		CString str1=L"��������:";
		ptr=pBuff->data;
		while((ptr = strstr(ptr, "+CMGL:")) != NULL)
		{
			ptr += 6;		// ����"+CMGL:", ��λ�����
			sscanf(ptr, "%d", &pMsg->index);	// ��ȡ���
//			str+=&pMsg->index;
			ptr = strstr(ptr, "\r\n");	// ����һ��
			if (ptr != NULL)
			{
				str+=str1;
				ptr += 2;		// ����"\r\n", ��λ��PDU		
				pDlg->encode.gsmDecodePdu(ptr, pMsg);	// PDU������
//				str+=&pMsg->TPA;
				str+=L" ���ŷ���ʱ��:";
//				str+=&pMsg->TP_SCTS;
//				m_list.AddString(str);
			}
		}
	}
	else
	{
		for(DWORD i=0;i<bufLen;i++)
			pDlg->num_from.Format(L"%s %c",pDlg->num_from,buf[i]);
	}
	pDlg->UpdateData(FALSE);
	delete[] buf;
}

/*�ı�ģʽ�¶�ȡ����Ϣ*/
BOOL CSMSDemoDlg::ReadMessage()
{
	CString strTel,Msg;
	int posnum, numlen, posmsg, msglen;	
	int pos = FindMsgPos(&posnum, &numlen, &posmsg, &msglen,strNum);	// Ѱ�Ҷ���λ��
	if (pos > 0)
	{
		strTel=strNum.Mid(posnum, numlen);   //���ͷ��ֻ���
		Msg = strNum.Mid(posmsg, msglen);				// ȡ���������� 
	}
	num_from="�������:";
	num_from+=strTel;
	num_from+="\n������Ϣ:";
	num_from+=Msg;
	return TRUE;
}
/*�ı�ģʽ��Ѱ�Ҷ���λ��*/
int CSMSDemoDlg::FindMsgPos(int *posnum, int *numlen, int *posmsg, int *msglen,CString strs)
{
	// ���ſ�ʼλ��
	int posStart = strs.Find(_T("+CMGR:"), 0);					
	if (posStart < 0)
		return -1;

	// �绰���뿪ʼλ��
	*posnum = strs.Find(_T("\"REC READ\",\""), posStart);	
	if (*posnum < 0)
		return -1;

	*posnum = *posnum + wcslen(_T("\"REC READ\",\""));			

	// �绰�������λ��
	int posnumend = strs.Find(_T("\""), *posnum + 1);
	if (posnumend < 0)
		return -1;

	// �绰���볤��
	*numlen = posnumend - *posnum;				
	
	// ���ſ�ʼλ��
	*posmsg = strs.Find(_T("\r\n"), posStart);				
	if (*posmsg < 0)
		return -1;

	*posmsg = *posmsg + wcslen(_T("\r\n"));	
	
	// ���Ž���λ��
	int posmsgend = strs.Find(_T("\r\n\r\nOK"), *posmsg);
	*msglen = posmsgend - *posmsg;	
	ReadMessage();
	return 1;
}

BOOL CSMSDemoDlg::waitok()
{
	return false;
}
void CSMSDemoDlg::GetNum(CString str)
{
	CString strTum=L"";
	int pos1 =str.Find(_T("+CLIP: \""), 0);
	if (pos1 < 0)
		MessageBox(L"�Ҳ����������",L"ϵͳ��Ϣ");	
	else
		pos1 = pos1 + wcslen(_T("+CLIP: \""));
	int pos2 =str.Find(_T("\""), pos1);
	if (pos2 < 0)
		MessageBox(L"�Ҳ����������",L"ϵͳ��Ϣ");
	strTum = str.Mid(pos1, pos2-pos1);				  /* ȡ�õ绰���� */
	num_from=strTum;
	UpdateData(false);
//	return strNum;
}
void CSMSDemoDlg::OnBnClickedSendmesg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strSmsc=m_num;
	CString strNumber=m_nums;
	CString strContent=m_message;
	if(AfxMessageBox(L"ȷ��������", MB_YESNO) == IDYES)	  //IDYES   The Yes button was selected.IDYES=6
	{
		SM_PARAM SmParam;
		memset(&SmParam, 0, sizeof(SM_PARAM));  //void int size_t(count) ָ�� ���õ�λ�� �����ַ�

		// ȥ������ǰ��"+"
		if(strSmsc[0] == '+')  strSmsc = strSmsc.Mid(1);   //�ӵ�һ����ʼ��ȡ�������ǵ�0��
		if(strNumber[0] == '+')  strNumber = strNumber.Mid(1);
		CString strtmp=_T("86");


		// �ں���ǰ��"86"
		if(strSmsc.Left(2) != "86")     //������nCount Ҫ�����CString��������ȡ���ַ���Ϊ2�� 
		{
			strtmp+=strSmsc;//strSmsc = "86" + strSmsc;
			strSmsc=strtmp;
		}
		strtmp=_T("86");
		if(strNumber.Left(2) != "86") 
		{
			strtmp+=strNumber;
			strNumber=strtmp;
		}

		// ������Ϣ�ṹ
		USES_CONVERSION;          //����������
		char* pStr=W2A(strSmsc);  //
		strcpy(SmParam.SCA, pStr);//strSmsc);   ����Ϣ�������ĺ���(SMSC��ַ)
		
		pStr=W2A(strNumber);
		strcpy(SmParam.TPA, pStr);//strNumber);   Ŀ������ظ�����(TP-DA��TP-RA)
		
		pStr=W2A(strContent);
		strcpy(SmParam.TP_UD, pStr);        //  ԭʼ�û���Ϣ(����ǰ�������TP-UD)
		SmParam.TP_PID=0;                 //  �û���ϢЭ���ʶ(TP-PID)

		SmParam.TP_DCS=GSM_UCS22;         //  �û���Ϣ���뷽ʽ(TP-DCS)

		int nPduLength;		// PDU������
		unsigned char nSmscLength;	// SMSC������
//		char cmd[16];		// ���

		nPduLength=encode.gsmEncodePdu(&SmParam,pdu);
		strcat(pdu, "\x01a");

		encode.gsmString2Bytes(pdu, &nSmscLength, 2);	// ȡPDU���е�SMSC��Ϣ����
		nSmscLength++;		// ���ϳ����ֽڱ���
		// �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�
//		sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength);	// ��������
		CString cmd;
		cmd.Format(_T("AT+CMGS=%d\r"),nPduLength / 2 - nSmscLength);
		serial.WriteChar(cmd);
	}
}

void CSMSDemoDlg::OnBnClickedReadmesg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString CMGL=L"AT+CMGL=\"ALL\"\r\n";
	serial.WriteChar(CMGL);
}

void CSMSDemoDlg::OnBnClickedBbh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL ret;
	CString num=L"ATD ";
	num+=num_to;
	num+=L";\r\n";
	ret=serial.WriteChar(num);
	if(ret==false)
	{
		MessageBox(L"����ʧ��",L"��ʾ��");
		return;
	}
	MessageBox(L"���ųɹ�",L"��ʾ��");
	m_gd.EnableWindow(true);
}

void CSMSDemoDlg::OnBnClickedBqc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=L"";
	UpdateData(false);
}

void CSMSDemoDlg::OnBnClickedBbt()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL ret;
	CString ata=L"ATA\r\n";
	ret=serial.WriteChar(ata);
	if(ret==false)
	{
		MessageBox(L"����ʧ��",L"��ʾ��");
		return;
	}
	m_gd.EnableWindow(true);
}

void CSMSDemoDlg::OnBnClickedBgd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL ret;
	CString ath=L"ATH\r\n";
	ret=serial.WriteChar(ath);
	if(ret==false)
	{
		MessageBox(L"�Ҷ�ʧ��",L"��ʾ��");
		return;	
	}	
	MessageBox(L"�Ҷϳɹ�",L"��ʾ��");	
	m_gd.EnableWindow(false);
	return;
}

void CSMSDemoDlg::OnBnClickedB1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"1";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"2";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"3";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"4";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"5";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"6";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"7";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"8";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"9";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedBf1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"*";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedB0()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"0";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnBnClickedBf2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	num_to=num_to+L"#";	
	UpdateData(false);	
}

void CSMSDemoDlg::OnLbnDblclkList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CSMSDemoDlg::OnLbnSelchangeList1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
