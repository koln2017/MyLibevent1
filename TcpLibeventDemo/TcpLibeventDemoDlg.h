
// TcpLibeventDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "TcpLibBase.h"
#include "TcpLibServer.h"
#include "afxwin.h"

// CTcpLibeventDemoDlg �Ի���
class CTcpLibeventDemoDlg : public CDialog
{
// ����
public:
	CTcpLibeventDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCPLIBEVENTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTcpLibBase *m_pTcpLibSer;
	CTcpLibBase *m_pTcpLibClient;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString m_strRecv;
	CString m_strSend;
	CComboBox m_cmbType;
	CEdit m_editSend;
	CEdit m_editRecv;
};
