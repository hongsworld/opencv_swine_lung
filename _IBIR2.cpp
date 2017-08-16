
// _IBIR_2.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "_IBIR_2.h"
#include "_IBIR_2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C_IBIR_2App

BEGIN_MESSAGE_MAP(C_IBIR_2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// C_IBIR_2App 생성

C_IBIR_2App::C_IBIR_2App()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 C_IBIR_2App 개체입니다.

C_IBIR_2App theApp;


// C_IBIR_2App 초기화

BOOL C_IBIR_2App::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	C_IBIR_2Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}


	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}





//						//
/////////////////////////////////////////////////////
//

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// BoVW interafce

void
C_IBIR_2App::app_fx_getBoVW(const string& sTrainDatasetPath)
{
	m_gxDataset4training.load(
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\training\\",
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\training\\");
//		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\04\\training\\",
//		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\04\\training\\");


//	m_xAppBoVW.test(&m_xAppTrainImgSet);
	//m_xAppBoVW.test(&m_gxDataset4training);
	m_xAppBoVW.test3(&m_gxDataset4training);
}

// BoVW interafce
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



void
extractClassLabels(string sFile, string sSep1, char cSep2, string sSep3, list<string>& lsLabel);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// classifier interface

//						//
void
C_IBIR_2App::app_classifier_train()
{

}

string
C_IBIR_2App::app_classifier_classify(const string& sImgPath)
{
	if (!m_xAppImgIndex.isIndexReady()) {
		AfxMessageBox(_T("Index is not ready.\nPlease build or load index"));
		return	"";
	}

	G_xLogFile.open("..\\log_classify_" + getNowS_1() + ".txt");
	G_xLogFile << "image file: " << sImgPath << endl << endl;
	string				sResult;
	m_xAppImgIndex.classify(NULL, sImgPath, sResult);
	G_xLogFile.close();

	list<string>		lsLabel;
	extractClassLabels(sImgPath, "_", '+', ".", lsLabel);
	string				sMsg;

	sMsg = "이미지 병명: ";
	if (lsLabel.size() == 1) {
		sMsg = sMsg + lsLabel.front();
		sMsg += "\r\n\r\n";
	}
	else {
		sMsg += "\r\n";
		for (list<string>::iterator i = lsLabel.begin(); i != lsLabel.end(); i++)
			sMsg = sMsg + "\t" + (*i) + "\r\n";
	}
	sMsg = sMsg + "\r\n";
	sMsg = sMsg + "검색된 병명: " + sResult;

	return	sMsg;
}


// classifier interface
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// kNN index interface

void
C_IBIR_2App::app_index_build(const string& sTrainDatasetPath, const string& sIndexType)
{
	// Set up training dataset
	m_xAppTrainImgSet.load(sTrainDatasetPath + "\\");

	// Build index on training dataset
	if (!m_xAppImgIndex.buildIndexOn(&m_xAppTrainImgSet, sIndexType))
		AfxMessageBox(_T("Building index failed"));
	else
		AfxMessageBox(_T("Building index succeeded"));
}
void
C_IBIR_2App::app_index_save(const string& sNNindexPath)
{
	m_xAppImgIndex.saveIndexTo(sNNindexPath);
}
bool
C_IBIR_2App::app_index_load(const string& sNNindexPath)
{
	return	m_xAppImgIndex.loadIndexFrom(sNNindexPath);
}
void
C_IBIR_2App::app_index_test(const string& sTestDatasetPath)
{

	if (!m_xAppImgIndex.isIndexReady()) {
		AfxMessageBox(_T("Index is not ready.\nPlease build or load index"));
		return;
	}

	// Set up testing dataset
	m_xAppTestImgSet.load(sTestDatasetPath, false);
	m_xAppTestImgSet.listClassStat();

	// Test index on testing dataset
	G_xLogFile.open("..\\log_test_" + getNowS_1() + ".txt");

	m_xAppImgIndex.testIndexOn(&m_xAppTestImgSet);

	G_xLogFile.close();
}

//string	C_IBIR_2App::app_classifier_classify(const string& sImgPath)

//{
//	if (!m_xAppImgIndex.isIndexReady()) {
//		AfxMessageBox(_T("Index is not ready.\nPlease build or load index"));
//		return	"";
//	}
//
//	G_xLogFile.open("..\\log_classify_" + getNowS_1() + ".txt");
//	G_xLogFile << "image file: " << sImgPath << endl << endl;
//	string				sResult;
//	m_xAppImgIndex.classify(NULL, sImgPath, sResult);
//	G_xLogFile.close();
//
//	list<string>		lsLabel;
//	extractClassLabels(sImgPath, "_", '+', ".", lsLabel);
//	string				sMsg;
//
//	sMsg = "이미지 병명: ";
//	if (lsLabel.size() == 1) {
//		sMsg = sMsg + lsLabel.front();
//		sMsg += "\r\n\r\n";
//	}
//	else {
//		sMsg += "\r\n";
//		for (list<string>::iterator i = lsLabel.begin(); i != lsLabel.end(); i++)
//			sMsg = sMsg + "\t" + (*i) + "\r\n";
//	}
//	sMsg = sMsg + "\r\n";
//	sMsg = sMsg + "검색된 병명: " + sResult;
//
//	return	sMsg;
//}


// kNN index interface
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



void
extractClassLabels(string sFile, string sSep1, char cSep2, string sSep3, list<string>& lsLabel)
{
	int			iSep1Pos = sFile.rfind(sSep1, string::npos);
	int			iSep3Pos = sFile.rfind(sSep3, string::npos);
	string		sMainamble = sFile.substr(iSep1Pos + 1, iSep3Pos - iSep1Pos - 1);

	try {
		if (iSep1Pos == string::npos || iSep3Pos == string::npos || iSep1Pos >= iSep3Pos || !sMainamble.length())
			throw(EUdataFileName());
	}
	catch (EUdataFileName e) {
		cerr << e.what() << endl;
	}

	string		sParse, sInput = sMainamble;
	stringstream
		input_stringstream(sInput);
	while (getline(input_stringstream, sParse, cSep2)) {
		//cout << ">" << string_clean(sParse) << "<" << endl;
		lsLabel.push_back(string_clean(sParse));
	}
}

void
C_IBIR_2App::app_test()
{
	//m_xAppTrainImgSet.load("D:\UDA\\_SNUP_1_dataset\\dataset_01_dodram14\\training\\");
	m_xAppTrainImgSet.load("D:\\UDA\\_SNUP_1_dataset\\dataset_02_dodram14_subset\\original\\training\\");
	//m_xAppTrainImgSet.load("D:\\UDA\\_SNUP_1_dataset\\dataset_02_dodram14_subset\\original\\testing\\");

	m_xAppImgIndex.test(&m_xAppTrainImgSet);
}

