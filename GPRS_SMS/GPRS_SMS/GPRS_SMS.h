
// GPRS_SMS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGPRS_SMSApp:
// �йش����ʵ�֣������ GPRS_SMS.cpp
//

class CGPRS_SMSApp : public CWinAppEx
{
public:
	CGPRS_SMSApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGPRS_SMSApp theApp;