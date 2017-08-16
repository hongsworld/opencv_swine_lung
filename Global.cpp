
#include "stdafx.h"
#include "Global.h"


//						//
ofstream				G_xLogFile;
string					G_sMsg;
ostringstream			G_xOstrStream;
int						G_i;




bool typePair_IF_less(typePair_IF& elem1, typePair_IF& elem2) {
	return elem1.second < elem2.second;
}
bool typePair_IF_greater(typePair_IF& elem1, typePair_IF& elem2) {
	return elem1.second > elem2.second;
}
bool typePair_IF_Igreater(typePair_IF& elem1, typePair_IF& elem2) {
	return elem1.first > elem2.first;
}



bool typePair_IX_less(typePair_IX& elem1, typePair_IX& elem2) {
	return elem1.second.second < elem2.second.second;
}
bool typePair_IX_greater(typePair_IX& elem1, typePair_IX& elem2) {
	return elem1.second.first > elem2.second.first;
}
bool typePair_IX_greater2(typePair_IX& elem1, typePair_IX& elem2) {
	return elem1.second.second > elem2.second.second;
}



inline float
SUmatchStat::getMatchMetric()
{
	// 1
	//return	getDistMin() * getDistMean() * getMatchCount();
	// 2
	//return	log(( 1.0 / (getMatchCount() / getPopRatio()) ) + 1) * getDistMean();
	// 3
	//return	log((1.0 / (getMatchCount() / getPopRatio() )) + 1) * getDistMean() * getDistMin();
	// 4
	//return	log((1.0 / getMatchCount()) + 1) * getDistMean() * getDistMin();
	// 5
	//return	log((1.0 / getMatchCount()) + 1) * getDistMean();
	// 6
	//return	getMatchCount();
	// 7
	//return	getDistMean();
	// 8
	//return	getMatchCount() * (getDistMean() - getDistMin());
	// 9
	return	log((1.0 / getMatchCount()) + 1);
}

bool SUmatchStat_less(SUmatchStat& e1, SUmatchStat& e2) {
	return e1.getMatchMetric() < e2.getMatchMetric();
}
bool SUmatchStat_greater(SUmatchStat& e1, SUmatchStat& e2) {
	return e1.getMatchMetric() > e2.getMatchMetric();
}



////////////////////////////////////////////////////////////////////
// Utility

string
getNowS()
{
	time_t			nNow = time(0);
	char			szNow[SIZE_ENOUGH_1];
	ctime_s(szNow, sizeof(szNow), &nNow);

	//>>>exception<<<
	return	string(szNow);
}

string
getNowS_1()
{
	time_t			nNowSec = time(0);
	struct tm		xNow;
	char			szNow[SIZE_ENOUGH_1];

	localtime_s(&xNow, &nNowSec);
	std::strftime(szNow, sizeof(szNow), "%y%m%d_%H%M_%S", &xNow);

	//>>>exception<<<
	return	string(szNow);
}


string
string_clean(const string cs)
{
	int				iBlackFront = cs.find_first_not_of(" ");
	int				iBlackBack = cs.find_last_not_of(" ");
	//return	cs.substr(iBlackFront, iBlackBack - iBlackFront + 1);
	string			sTemp = cs.substr(iBlackFront, iBlackBack - iBlackFront + 1);
	size_t			iHyphen = sTemp.find('-');
	return			sTemp.substr(0, iHyphen);
}



void
extractLabels(string csFile)
{
	list<string>	ls;
	string			sSep1 = "_", sSep2 = "+", sSep3 = ".";
	int				iSep1Pos = csFile.rfind(sSep1, string::npos);
	int				iSep3Pos = csFile.rfind(sSep3, string::npos);

	cout << "string length = " << csFile.length() << endl;
	cout << "iSep1Pos = " << iSep1Pos << endl;

	//	cout << "sSep1 = " << "(" << csFile[iSep1Pos] << ")" << endl;
	cout << "mainablem = >" << csFile.substr(iSep1Pos + 1, iSep3Pos - iSep1Pos - 1) << "<" << endl;

	//string		sCleanLabel = string_clean(csFile.substr(iSep1Pos + 1, iSep3Pos - iSep1Pos + 1));
	string		sCleanLabel = csFile.substr(iSep1Pos + 1, iSep3Pos - iSep1Pos - 1);
	cout << "cleaned label part = >" << sCleanLabel << "<" << endl;
	try {
		if (iSep1Pos == string::npos || iSep3Pos == string::npos || iSep1Pos >= iSep3Pos || !sCleanLabel.length())
			throw("Data file name is not well formed.");
		cout << "whawwrew" << endl;
	}
	catch (const char *csz) {
		cerr << csz << endl;
	}

	string parsed, input = sCleanLabel; // "text to be parsed";
	stringstream input_stringstream(input);

	while (getline(input_stringstream, parsed, '+')) {
		cout << ">" << string_clean(parsed) << "<" << endl;
		//cout <<">"<< parsed <<"<"<< endl;
	}
}

string
getExt(const string& sFilePath)
{
	size_t				iExtDelim = sFilePath.find_last_of(".");
	if (iExtDelim == string::npos)	return false;

	string				sExt = sFilePath.substr(iExtDelim + 1, string::npos);

	_strupr_s((char*) sExt.c_str(), sExt.size()+1);
	return	sExt;
}


bool
isValidImgFile(const string& sFilePath)
{
	size_t				iExtDelim = sFilePath.find_last_of(".");
	if (iExtDelim == string::npos)	return false;

	string				sExt = sFilePath.substr(iExtDelim + 1, string::npos);

	_strupr_s((char*) sExt.c_str(), sExt.size()+1);

	if (sExt == "JPG" || sExt == "JPEG")
		return	true;
	else if (sExt == "BMP")
		return	true;
	else
		return	false;
}

bool
exists_path(const char* szDirName, bool bToCheckDir)
{
	DWORD iAttribs = ::GetFileAttributesA(szDirName);
	if (iAttribs == INVALID_FILE_ATTRIBUTES)
		return	false;
	if (bToCheckDir)
		return	(iAttribs & FILE_ATTRIBUTE_DIRECTORY);
	else
		return	!(iAttribs & FILE_ATTRIBUTE_DIRECTORY);
}




////////////////////////////////////////////////////////////////////
// CU_util_MFCconsole
//                      //
CU_util_MFCconsole::CU_util_MFCconsole()
{
	// 초기화 콘솔을 열고 stdout, stdin 설정후 콘솔을 닫음 
	AllocConsole();
	int					hCrtout = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	FILE				*hout = _fdopen(hCrtout, "w");
	*stdout = *hout;
	int					hCrtin = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
	FILE *hin = _fdopen(hCrtin, "r");
	*stdin = *hin;
	setvbuf(stdin, NULL, _IONBF, 0);
	FreeConsole();
}

CU_util_MFCconsole::~CU_util_MFCconsole()
{
	_fcloseall();
}

void
CU_util_MFCconsole::open()
{
	//콘솔을 열고 타이틀을 설정함. 열린 콘솔이 있을경우 무시됨.
	if (AllocConsole()) {
		SetConsoleTitle(_T("Console"));
		printf("huck");
		cout << "HUCK" << endl;
	}
	else
		AfxMessageBox(_T("Another console is already open"));
}
void
CU_util_MFCconsole::close()
{
	// 콘솔을 닫음. 열린 콘솔이 없을경우 무시.
	FreeConsole();
}








////////////////////////////////////////////////////////////////////
// set/get data to/from file
//                      //
void
setLinesToFile(const string sFilePath, vector<int>& ai)
{
	std::ofstream		xOutFile(sFilePath);
	for (int i = 0; i < ai.size(); i++)
		xOutFile << ai[i] << endl;
	xOutFile.close();
}

void
getLinesFromFile(const string sFilePath, vector<int>& ai)
{
	std::ifstream		xInFile(sFilePath);
	string				sLine;
	while (std::getline(xInFile, sLine))
		ai.push_back(std::stoi(sLine));
	xInFile.close();
}

void
setLinesToFile(const string sFilePath, vector<string>& as)
{
	std::ofstream		xOutFile(sFilePath);
	for (int i = 0; i < as.size(); i++)
		xOutFile << as[i] << endl;
	xOutFile.close();
}
void
getLinesFromFile(const string sFilePath, vector<string>& as)
{
	std::ifstream		xInFile(sFilePath);
	string				sLine;
	while (std::getline(xInFile, sLine))
		as.push_back(sLine);
	xInFile.close();
}


