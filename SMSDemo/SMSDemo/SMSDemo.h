
// SMSDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSMSDemoApp:
// �йش����ʵ�֣������ SMSDemo.cpp
//

class CSMSDemoApp : public CWinAppEx
{
public:
	CSMSDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSMSDemoApp theApp;