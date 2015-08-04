
// sFileDlg.h : 头文件
//

#pragma once


// CsFileDlg 对话框
class CsFileDlg : public CDialogEx
{
// 构造
public:
	CsFileDlg(CWnd* pParent = NULL);	// 标准构造函数
	void update(CString s);

// 对话框数据
	enum { IDD = IDD_SFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
		CEdit* show_edit;
		CEdit* send_edit;
		CEdit* port_edit;
		CButton* star_btn;
		CEdit* showmsg_edit;
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
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedBtnsend();
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
