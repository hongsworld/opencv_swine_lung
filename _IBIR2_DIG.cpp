
// _IBIR_2Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "_IBIR_2.h"
#include "_IBIR_2Dlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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





// C_IBIR_2Dlg 대화 상자




C_IBIR_2Dlg::C_IBIR_2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C_IBIR_2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pxApp = (C_IBIR_2App*) AfxGetApp();
}

void C_IBIR_2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DATASETPATH_TRAIN, m_xEdit_datasetpath_train);
	DDX_Control(pDX, IDC_EDIT_IMAGE_SELECT, m_xEdit_image_select);
	DDX_Control(pDX, IDC_EDIT_MSG_1, m_xEdit_msg_1);
	DDX_Control(pDX, IDC_EDIT_DATASETPATH_TEST, m_xEdit_datasetpath_test);
}

BEGIN_MESSAGE_MAP(C_IBIR_2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_02, &C_IBIR_2Dlg::OnBnClick_02)
	ON_BN_CLICKED(IDC_BTN_04, &C_IBIR_2Dlg::OnBnClick_04)
	ON_BN_CLICKED(IDC_BTN_IMAGE_SELECT, &C_IBIR_2Dlg::OnBnClickImageSelect)
	ON_BN_CLICKED(IDC_BTN_CLASSIFY, &C_IBIR_2Dlg::OnBnClickClassify)
	ON_BN_CLICKED(IDC_BTN_06, &C_IBIR_2Dlg::OnBnClick_06)
	ON_BN_CLICKED(IDC_BTN_08, &C_IBIR_2Dlg::OnBnClick_08)
END_MESSAGE_MAP()



CFont *myFont = new CFont();



// C_IBIR_2Dlg 메시지 처리기

BOOL C_IBIR_2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	m_xEdit_datasetpath_train.SetWindowTextW(_T("D:\\UDA\\_SNUP_1_dataset\\dataset_02_dodram14_subset\\original\\testing\\"));

	myFont->CreateFont(20, 0, 0, 0, FW_HEAVY, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Courier New"));

	m_xEdit_msg_1.SetFont(myFont);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void C_IBIR_2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void C_IBIR_2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}



// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR C_IBIR_2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// MFC
//////////////////////////////////////////////////////////////////////////////////////////////////////////









//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// BoVW interface
//						//
void C_IBIR_2Dlg::OnBnClick_02()
{
	// TODO: Add your control notification handler code here
	CString				sSelectedPath;
	m_xEdit_datasetpath_train.GetWindowText(sSelectedPath);


	if ( !exists_path( string((CStringA) sSelectedPath).c_str()) ) {
		AfxMessageBox(_T("Training dataset path is not valid"));
		return;
	}

//	m_pxApp->app_index_build((string)(CStringA)sSelectedPath + "\\", "composite");
	m_pxApp->app_fx_getBoVW((string)(CStringA)sSelectedPath + "\\");
}


void C_IBIR_2Dlg::OnBnClick_04()
{
//	// TODO: Add your control notification handler code here
//	//AfxMessageBox(_T("Currently index is automatically saved on building\n to the default directory"));
//
//	m_pxApp->app_test();
}


void C_IBIR_2Dlg::OnBnClick_06()
{
//	// TODO: Add your control notification handler code here
//
//	CFileDialog			xFileDialog(TRUE, _T("*.*"), NULL);		//, OFN_HIDEREADONLY ); //, _T("Text Files: (*.txt)|*.txt||") );
//	CString				sSelectedPath;
//	if (xFileDialog.DoModal() == IDOK) {
//		sSelectedPath = xFileDialog.GetPathName();
//		//m_xEdit_image_select.SetWindowTextW(sSelectedPath);
//	}
//	else
//		return;
//
//	if (!m_pxApp->app_index_load(string((CStringA)sSelectedPath)))
//		AfxMessageBox(sSelectedPath + _T("\n Loading index failed"));
//	else
//		AfxMessageBox(_T("Index loaded"));
}


void C_IBIR_2Dlg::OnBnClick_08()
{
//	// TODO: Add your control notification handler code here
//
//	CString				sSelectedPath;
//	m_xEdit_datasetpath_test.GetWindowText(sSelectedPath);
//
//	if (!exists_path(string((CStringA)sSelectedPath).c_str())) {
//		AfxMessageBox(_T("Testing dataset path is not valid"));
//		return;
//	}
//
//	m_pxApp->app_index_test((string)(CStringA)sSelectedPath + "\\");
}




//						//
void C_IBIR_2Dlg::OnBnClickImageSelect()
{
//	// TODO: Add your control notification handler code here
//
//	m_xEdit_msg_1.SetWindowTextW(_T(""));
//
//	CFileDialog			xFileDialog(TRUE, _T("*.*"), NULL);		//, OFN_HIDEREADONLY ); //, _T("Text Files: (*.txt)|*.txt||") );
//	CString				sSelectedPath;
//	if (xFileDialog.DoModal() == IDOK) {
//		sSelectedPath = xFileDialog.GetPathName();
//		m_xEdit_image_select.SetWindowTextW(sSelectedPath);
//	}
//	else
//		return;
//
//	// Read in the selected image
//	std::string			sImgPath((CStringA)sSelectedPath);
//	cv::Mat				mSelectedImg;
//	mSelectedImg = cv::imread(sImgPath, 1);
//
//	char* source_window = "Source";
//	cv::namedWindow(source_window, CV_WINDOW_NORMAL); //CV_WINDOW_AUTOSIZE);
//	cv::imshow(source_window, mSelectedImg);
}

						//
void C_IBIR_2Dlg::OnBnClickClassify()
{
//	// TODO: Add your control notification handler code here
//
//	CString				sSelectedImgPath;
//	m_xEdit_image_select.GetWindowText(sSelectedImgPath);
//
//	if (!exists_path(string((CStringA)sSelectedImgPath).c_str(), false)) {
//		AfxMessageBox(_T("Image file path is not valid"));
//		return;
//	}
//
//	string				sRetMsg;
//	sRetMsg = m_pxApp->app_classifier_classify((string)(CStringA)sSelectedImgPath);
//
//	// Display the result
//	m_xEdit_msg_1.SetWindowTextW(CString(sRetMsg.c_str()));
}


// BoVW interface
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// k-NN index interface
//						//
//void C_IBIR_2Dlg::OnBnClick_02()
//{
//	// TODO: Add your control notification handler code here
//	CString				sSelectedPath;
//	m_xEdit_datasetpath_train.GetWindowText(sSelectedPath);
//
//
//	if ( !exists_path( string((CStringA) sSelectedPath).c_str()) ) {
//		AfxMessageBox(_T("Training dataset path is not valid"));
//		return;
//	}
//
//	m_pxApp->app_index_build((string)(CStringA)sSelectedPath + "\\", "composite");
//}
//
//
//void C_IBIR_2Dlg::OnBnClick_04()
//{
//	// TODO: Add your control notification handler code here
//	//AfxMessageBox(_T("Currently index is automatically saved on building\n to the default directory"));
//
//	m_pxApp->app_test();
//}
//
//void C_IBIR_2Dlg::OnBnClick_06()
//{
//	// TODO: Add your control notification handler code here
//
//	CFileDialog			xFileDialog(TRUE, _T("*.*"), NULL);		//, OFN_HIDEREADONLY ); //, _T("Text Files: (*.txt)|*.txt||") );
//	CString				sSelectedPath;
//	if (xFileDialog.DoModal() == IDOK) {
//		sSelectedPath = xFileDialog.GetPathName();
//		//m_xEdit_image_select.SetWindowTextW(sSelectedPath);
//	}
//	else
//		return;
//
//	if (!m_pxApp->app_index_load(string((CStringA)sSelectedPath)))
//		AfxMessageBox(sSelectedPath + _T("\n Loading index failed"));
//	else
//		AfxMessageBox(_T("Index loaded"));
//}
//
//
//void C_IBIR_2Dlg::OnBnClick_08()
//{
//	// TODO: Add your control notification handler code here
//
//	CString				sSelectedPath;
//	m_xEdit_datasetpath_test.GetWindowText(sSelectedPath);
//
//	if (!exists_path(string((CStringA)sSelectedPath).c_str())) {
//		AfxMessageBox(_T("Testing dataset path is not valid"));
//		return;
//	}
//
//	m_pxApp->app_index_test((string)(CStringA)sSelectedPath + "\\");
//}
//
//
//
//
////						//
//void C_IBIR_2Dlg::OnBnClickImageSelect()
//{
//	// TODO: Add your control notification handler code here
//
//	m_xEdit_msg_1.SetWindowTextW(_T(""));
//
//	CFileDialog			xFileDialog(TRUE, _T("*.*"), NULL);		//, OFN_HIDEREADONLY ); //, _T("Text Files: (*.txt)|*.txt||") );
//	CString				sSelectedPath;
//	if (xFileDialog.DoModal() == IDOK) {
//		sSelectedPath = xFileDialog.GetPathName();
//		m_xEdit_image_select.SetWindowTextW(sSelectedPath);
//	}
//	else
//		return;
//
//	// Read in the selected image
//	std::string			sImgPath((CStringA)sSelectedPath);
//	cv::Mat				mSelectedImg;
//	mSelectedImg = cv::imread(sImgPath, 1);
//
//	char* source_window = "Source";
//	cv::namedWindow(source_window, CV_WINDOW_NORMAL); //CV_WINDOW_AUTOSIZE);
//	cv::imshow(source_window, mSelectedImg);
//}
//
//						//
//void C_IBIR_2Dlg::OnBnClickClassify()
//{
//	// TODO: Add your control notification handler code here
//
//	CString				sSelectedImgPath;
//	m_xEdit_image_select.GetWindowText(sSelectedImgPath);
//
//	if (!exists_path(string((CStringA)sSelectedImgPath).c_str(), false)) {
//		AfxMessageBox(_T("Image file path is not valid"));
//		return;
//	}
//
//	string				sRetMsg;
//	sRetMsg = m_pxApp->app_classifier_classify((string)(CStringA)sSelectedImgPath);
//
//	// Display the result
//	m_xEdit_msg_1.SetWindowTextW(CString(sRetMsg.c_str()));
//}


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


