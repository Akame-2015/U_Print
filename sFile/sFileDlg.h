
// sFileDlg.h : ͷ�ļ�
//

#pragma once


// CsFileDlg �Ի���
class CsFileDlg : public CDialogEx
{
// ����
public:
	CsFileDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void update(CString s);

// �Ի�������
	enum { IDD = IDD_SFILE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
		CEdit* show_edit;
		CEdit* send_edit;
		CEdit* port_edit;
		CButton* star_btn;
		CEdit* showmsg_edit;
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
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedBtnsend();
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
