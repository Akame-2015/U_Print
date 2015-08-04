
// sFileDlg.cpp : ʵ���ļ�
//
#include <winsock2.h> 
#include "stdafx.h"
#include "sFile.h"
#include "sFileDlg.h"
#include "afxdialogex.h"
#pragma comment(lib, "WS2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
UINT server_thd(LPVOID p); //�����̺߳��� 
SOCKET listen_sock;//����һ��ȫ�ֵļ���soket 
SOCKET sock;//����һ��soket 
CString showeditmsg; //����ȫ�ֵ���ʾ��Ϣ���ַ���

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsFileDlg �Ի���



CsFileDlg::CsFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Btnsend, &CsFileDlg::OnBnClickedBtnsend)
	ON_BN_CLICKED(IDC_BUTTON2, &CsFileDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CsFileDlg ��Ϣ�������

BOOL CsFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	show_edit = (CEdit *)GetDlgItem(IDC_EDIT1);
	send_edit = (CEdit *)GetDlgItem(IDC_EDIT2);
	port_edit = (CEdit *)GetDlgItem(IDC_EDIT3);
	star_btn = (CButton *)GetDlgItem(IDC_BUTTON2);
	showmsg_edit = (CEdit *)GetDlgItem(IDC_EDIT4);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CsFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsFileDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CsFileDlg::update(CString s)
{
	showeditmsg += s;
	show_edit->SetWindowText(showeditmsg);
}


UINT server_thd(LPVOID p)
{
	WSADATA wsaData;
	WORD wVersion;
	wVersion = MAKEWORD(2, 2);
	WSAStartup(wVersion, &wsaData);

	SOCKADDR_IN local_addr;
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	int res;
	char msg[1024];
	CString port;
	CsFileDlg * dlg = (CsFileDlg *)AfxGetApp()->GetMainWnd(); //�õ�Ӧ�ó��������ڵ�ָ�� 
	dlg->port_edit->GetWindowText(port);//��ö˿ڵ�ַ 
	if (port == "")
	{
		AfxMessageBox("������˿ں�");
		return 0;
	}
	//Ϊlocal_addr��ֵ������soket 
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(atoi(port)); //atoi�� ���ַ���ת���������� 
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//��������listen_sock 
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		dlg->update("��������ʧ��\r\n");
		return 0;
	}
	//�� 
	if (bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)))
	{
		dlg->update("�󶨴���,��һ���˿����԰�~\r\n");
		return 0;
	}

	listen(listen_sock, 1); //��ʼ������������������Ϊ1 
	dlg->star_btn->EnableWindow(FALSE);  //������������ť�һ� 
	dlg->showmsg_edit->ShowWindow(SW_SHOW); //����Ϣedit��ʾ 
	dlg->update("�ѳɹ�����....\r\n");

	//������������ 
	if ((sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)
	{
		dlg->update("������������ʧ��\r\n");
		return 0;
	}
	else
	{
		CString port;
		port.Format("%d", int(ntohs(client_addr.sin_port)));
		dlg->update("���������ԣ�" + CString(inet_ntoa(client_addr.sin_addr)) + "  �˿ڣ�" +
			port + "\r\n");
	}

	////////////�������� 
	while (1)
	{

		if ((res = recv(sock, msg, 1024, 0)) == -1) //�������ݣ��ж��Ƿ���ճɹ� 
		{
			dlg->update("ʧȥ����\r\n");
			dlg->star_btn->EnableWindow(TRUE);
			break;
		}
		else
		{
			dlg->update("�ͻ���:" + (CString(msg)).Mid(0, res) + "\r\n");
		}

	}
	return 0;
}


void CsFileDlg::OnBnClickedBtnsend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString s;
	char * msg;
	send_edit->GetWindowText(s);
	msg = (char*)s.GetBuffer(s.GetLength()); //��ȡ���͵����ݵĻ�������ָ��

	if (s == "")
	{
		MessageBox("��������Ϣ");
	}
	else if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR) //�������ݣ����ж��Ƿ�ɹ� 
	{
		update("����ʧ��\r\n");
	}
	else
	{
		update("��:" + s + "\r\n");//��Ϣ���� 
		send_edit->SetWindowText("");//������� 
		send_edit->SetFocus();//���ػ񽹵� 
	}
}


void CsFileDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);

	char name[80];
	CString IP;
	CString P;
	hostent* pHost;
	gethostname(name, sizeof(name));//��������� 
	pHost = gethostbyname(name);//��������ṹ 
	IP = inet_ntoa(*(struct in_addr *)pHost->h_addr_list[0]);//��ȡ����ip��ַ 
	showmsg_edit->SetWindowText("��IP��ַ��" + IP);
	AfxBeginThread(&server_thd, 0); //�������̴߳��� 
}


BOOL CsFileDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	//����س���Ϣ 
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_RETURN)
	{
		//��Ӧ�س�Ϊtab�� 
		pMsg->wParam = VK_TAB;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
