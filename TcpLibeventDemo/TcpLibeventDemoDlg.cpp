
// TcpLibeventDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TcpLibeventDemo.h"
#include "TcpLibeventDemoDlg.h"
#include<atlconv.h>

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


// CTcpLibeventDemoDlg �Ի���
CTcpLibeventDemoDlg::CTcpLibeventDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpLibeventDemoDlg::IDD, pParent)
	, m_strRecv(_T(""))
	, m_strSend(_T(""))
	, m_pTcpLibClient(NULL)
	, m_pTcpLibSer(NULL)
	, m_pBufEventSer(NULL)
	, m_pBufEventClient(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpLibeventDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRecv);
	DDX_Text(pDX, IDC_EDIT2, m_strSend);
	DDX_Control(pDX, IDC_COMBO1, m_cmbType);
	DDX_Control(pDX, IDC_EDIT2, m_editSend);
	DDX_Control(pDX, IDC_EDIT1, m_editRecv);
}

BEGIN_MESSAGE_MAP(CTcpLibeventDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTcpLibeventDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTcpLibeventDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTcpLibeventDemoDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CTcpLibeventDemoDlg ��Ϣ�������

BOOL CTcpLibeventDemoDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTcpLibeventDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTcpLibeventDemoDlg::OnPaint()
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
HCURSOR CTcpLibeventDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void TcpcbFun(void *argc, void *argv)
{
	CTcpLibeventDemoDlg *pDlg = (CTcpLibeventDemoDlg*)argc;
	char *pBuf = (char*)argv;

	USES_CONVERSION;
	CString str;
	pDlg->m_editRecv.GetWindowText(str);
	if (_T("") != str)
	{	
		str.AppendFormat(_T("\r\n"));
	}
	str.AppendFormat(A2W(pBuf));
	pDlg->m_editRecv.SetWindowText(str);
}

void CTcpLibeventDemoDlg::OnBnClickedButton1()
{
	CBFUN_PARAM_T param;
	param.pThis = this;
	param.cbFun = TcpcbFun;
	int nSel = m_cmbType.GetCurSel();
	switch (nSel)
	{
	case 0:
		{
			//�����_bufferevent
			m_pBufEventSer = NewTcpLib(0, param);
			m_pBufEventSer->Init("", 8888);
			m_pBufEventSer->Start();
		}
		break;
	case 1:
		{
			//�����_event
			m_pTcpLibSer = NewTcpLib(1, param);
			m_pTcpLibSer->Init("", 8888);
			m_pTcpLibSer->Start();
		}
		break;
	case 2:
		{
			//�ͻ���_bufferevent
			m_pBufEventClient = NewTcpLib(2, param);
			m_pBufEventClient->Init("10.21.38.21", 8888);
			m_pBufEventClient->Start();
		}
		break;
	case 3:
		{
			//�ͻ���_event
			m_pTcpLibClient= NewTcpLib(3, param);
			m_pTcpLibClient->Init("10.21.38.21", 8888);
			m_pTcpLibClient->Start();
		}
		break;
	default:
		break;
	}
	m_cmbType.EnableWindow(FALSE);
}

void CTcpLibeventDemoDlg::OnBnClickedButton2()
{
	int nSel = m_cmbType.GetCurSel();
	switch (nSel)
	{
	case 0:
		{
			//�����_bufferevent
			m_pBufEventSer->Stop();
		}
		break;
	case 1:
		{
			//�����_event
			m_pTcpLibSer->Stop();
		}
		break;
	case 2:
		{
			//�ͻ���_bufferevent
			m_pBufEventClient->Stop();
		}
		break;
	case 3:
		{
			//�ͻ���_event
			m_pTcpLibClient->Stop();
		}
		break;
	default:
		break;
	}
	m_cmbType.EnableWindow(TRUE);
}

void CTcpLibeventDemoDlg::OnBnClickedButton3()
{
	USES_CONVERSION;
	CString str;
	m_editSend.GetWindowText(str);
	int nSel = m_cmbType.GetCurSel();
	switch (nSel)
	{
	case 0:
		{
			//�����_bufferevent
			TCHAR *pBuf = (LPTSTR)(LPCTSTR)str;
			m_pBufEventSer->Send((const unsigned char*)W2A(pBuf), str.GetLength());
		}
		break;
	case 1:
		{
			//�����_event
			TCHAR *pBuf = (LPTSTR)(LPCTSTR)str;
			m_pTcpLibSer->Send((const unsigned char*)W2A(pBuf), str.GetLength());
		}
		break;
	case 2:
		{
			//�ͻ���_bufferevent
			TCHAR *pBuf = (LPTSTR)(LPCTSTR)str;
			m_pBufEventClient->Send((const unsigned char*)W2A(pBuf), str.GetLength());
		}
		break;
	case 3:
		{
			//�ͻ���_event
			TCHAR *pBuf = (LPTSTR)(LPCTSTR)str;
			m_pTcpLibClient->Send((const unsigned char*)W2A(pBuf), str.GetLength());
		}
		break;
	default:
		break;
	}
}
