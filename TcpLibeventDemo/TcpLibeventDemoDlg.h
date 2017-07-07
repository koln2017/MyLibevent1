
// TcpLibeventDemoDlg.h : 头文件
//

#pragma once
#include "TcpLibBase.h"
#include "TcpLibServer.h"
#include "afxwin.h"

// CTcpLibeventDemoDlg 对话框
class CTcpLibeventDemoDlg : public CDialog
{
// 构造
public:
	CTcpLibeventDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPLIBEVENTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
