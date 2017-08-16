#include "stdafx.h"
#include "Echelon_1.h"

#include "Echelon_3.h"



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
//
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

cv::Ptr<cv::Feature2D>	xxxCUsetDataImg::m_pxF2dSIFT = cv::xfeatures2d::SIFT::create();

cv::Ptr<cv::Feature2D>	CUdatumImage::m_pxF2dSIFT = cv::xfeatures2d::SIFT::create();


//						//
////////////////////////////////////////////////////////////////////
// Utility

void
saveMat(const cv::Mat& mData, const string sDataName, const string& sFilePath)
{
	cv::FileStorage		xOutFS(sFilePath, cv::FileStorage::WRITE);
	xOutFS << sDataName << mData;
	xOutFS.release();
}

void
loadMat(cv::Mat& mData, const string sDataName, const string& sFilePath)
{
	cv::FileStorage xInFS(sFilePath, cv::FileStorage::READ);
	xInFS[sDataName] >> mData;
	xInFS.release();
}

float
dist(cv::InputArray mVec1, cv::InputArray mVec2)
{
	//))SE: Make sure the 2 input arrays have same size and type
	cv::Mat mTemp(1, mVec1.size().width, CV_32F);
	cv::absdiff(mVec1, mVec2, mTemp);
	cv::pow(mTemp, 2, mTemp);

	return	sqrt(cv::sum(mTemp)[0]);
}


void
writeKeyptImage(const string& sFilePath, std::vector<cv::KeyPoint>& axKeypoint)
{
	static cv::Ptr<cv::Feature2D> pxF2dSIFT = cv::xfeatures2d::SIFT::create();

	//const cv::Mat	xmImg_1 = cv::imread(sFilePath, CV_LOAD_IMAGE_GRAYSCALE);
	const cv::Mat	xmImg_1 = cv::imread(sFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat			xmImg_2; //= xmImg_1.clone();

	int				nRow = 1200;
	int				nCol = 1600;
	cv::resize(xmImg_1, xmImg_2, cv::Size(nCol, nRow));

	// Detect image's keypoints
//	std::vector<cv::KeyPoint> vxKeypoint;
//	pxF2dSIFT->detect(xmImg_2, vxKeypoint);

	// Draw keypoints to image
	cv::Mat	xmImgOut;
	cv::drawKeypoints(xmImg_1, axKeypoint, xmImgOut, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	string			sFilePath2 = sFilePath + "_keypoint.JPG";
	cv::imwrite(sFilePath2, xmImgOut);
}




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUitem
//					//
void
CUitem::streamIn(stringstream& xIn)
{
	string			sParse;
	std::getline(xIn, sParse, '\t');
	std::getline(xIn, sParse, '\t');
	m_iID = std::stoi(sParse);

	std::getline(xIn, sParse, '\t');
	m_sLabel = sParse;
}

// CUitem
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUclass
//					//
void
CUclass::streamOut(ostream& xOut)
{
	CUitem::streamOut(xOut);
	xOut << "\t" << m_nPop;
}
void
CUclass::streamIn(stringstream& xIn)
{
	CUitem::streamIn(xIn);

	string			sParse;
	std::getline(xIn, sParse, '\t');
	//m_nPop = std::stoi(sParse);
}

// CUclass
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<






//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUdatum

void
CUdatum::streamOut(ostream& xOut)
{
	CUitem::streamOut(xOut);
//	xOut << "\t" << m_iID << "\t" << m_sLabel;
}



string
CUdatum::getFilePath()
{
	cout << "kkk = " << m_pxDataset << endl;

	return	((CUsetOfData*) m_pxDataset)->getDatasetRoot()
		+ "\\" + ((CUsetOfData*) m_pxDataset)->getRelaPath(m_iRelaPath)
		+ "\\" + m_sLabel;

	//xxxCUsetFile*		p = 0; //(xxxCUsetFile*)m_pxDataFileSet;
	//string				sGroup = p->getGroupLabel(m_iRelaPath);
	//xxxCUsetData*			pp = (xxxCUsetData*)p->m_pxDataSet;
	//string				sDatasetRoot = pp->getDataSetPath();

	//return	sDatasetRoot + "\\" + sGroup + "\\" + m_sLabel;
}

// CUdatum
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

////////////////////////////////////////////////////////////////////
// CUdatumImage

void
CUdatumImage::normalizeSize(const cv::Mat& mImg1, cv::Mat& mImg2)
{
	float	fNormalznRatio = (float)STD_IMG_SIZE_ROW / mImg1.rows;
	int		nNewRow = ceil(mImg1.rows * fNormalznRatio);
	int		nNewCol = ceil(mImg1.cols *fNormalznRatio);

	cout << "image size = " << mImg1.rows << " x " << mImg1.cols << endl;

	cv::resize(mImg1, mImg2, cv::Size(nNewCol, nNewRow));    // Size = [width (cols), height (rows)]
}

//						//
void
CUdatumImage::extractFeatures(
						string& sFilePath, 
						cv::Mat& m2dfDesc,
						vector<int>& paiFile,
						std::vector<cv::KeyPoint>& axKeypoint)
{
	cv::Ptr<cv::Feature2D>
						pxSIFT = xxxCUsetDataImg::m_pxF2dSIFT;
	// Load image
	// >>>exception<<<
	cout << "File path = " << sFilePath << endl << endl;

	const cv::Mat		mImg1 = cv::imread(sFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat				mImg2; //= mImg_1.clone();
	// Normalize image's size (resolution)
	normalizeSize(mImg1, mImg2);

	// Detect image's keypoints
	pxSIFT->detect(mImg2, axKeypoint);
	// Extract descriptor (feature) vector for each keypoint
	cv::Mat				mDescriptorVec;
	pxSIFT->compute(mImg2, axKeypoint, mDescriptorVec);
	// Accumulate the descriptor vectors
	m2dfDesc.push_back(mDescriptorVec);
	// Accumulate the file IDs
	for (int i = 0; i < mDescriptorVec.rows; i++)
		paiFile.push_back(m_iID);
}




void
CUdatumImage::extractFeatures(string& sFilePath, cv::Mat& m2dfDesc, vector<int>& paiFile)
{
	cv::Ptr<cv::Feature2D>
		pxSIFT = xxxCUsetDataImg::m_pxF2dSIFT;
	// Load image
	// >>>exception<<<
	cout << "File path = " << sFilePath << endl << endl;


	const cv::Mat		mImg1 = cv::imread(sFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat				mImg2; //= mImg_1.clone();
	// Normalize image's size (resolution)
	normalizeSize(mImg1, mImg2);


	// Detect image's keypoints
	std::vector<cv::KeyPoint>
		axKeypoint;
	pxSIFT->detect(mImg2, axKeypoint);
	// Extract descriptor (feature) vector for each keypoint
	cv::Mat				mDescriptorVec;
	pxSIFT->compute(mImg2, axKeypoint, mDescriptorVec);
	// Accumulate the descriptor vectors
	m2dfDesc.push_back(mDescriptorVec);
	// Accumulate the file IDs
	for (int i = 0; i < mDescriptorVec.rows; i++)
		paiFile.push_back(m_iID);
}

void
CUdatumImage::getBoVW(cv::Mat& mfBoVW, void* pxBoVW)
{
	string				sFilePath = getFilePath();
	cout << "\t" << m_iID << "\t" << sFilePath << endl;

	cv::Mat				afBoVW;
	((CUfxBoVW*) pxBoVW)->getBoVW(sFilePath, afBoVW);
	// Accumulate BoVW
	mfBoVW.push_back(afBoVW);
}

void
CUdatumImage::extractFeatures(cv::Mat& m2dfDesc, vector<int>* paiFile)
{
	string				sFilePath = getFilePath();
	cout << "\t" << m_iID << "\t" << sFilePath << endl;

	// Load image
	// >>>exception<<<
	const cv::Mat		mImg1 = cv::imread(sFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat				mImg2; //= mImg_1.clone();
	// Normalize image's size (resolution)
	normalizeSize(mImg1, mImg2);
	// Detect image's keypoints
	std::vector<cv::KeyPoint>
						axKeypoint;
	m_pxF2dSIFT->detect(mImg2, axKeypoint);
	// Extract descriptor (feature) vector for each keypoint
	cv::Mat				mDescriptorVec;
	m_pxF2dSIFT->compute(mImg2, axKeypoint, mDescriptorVec);
	// Accumulate the descriptor vectors
	m2dfDesc.push_back(mDescriptorVec);
	// Accumulate the file IDs
	if (paiFile)
		for (int i = 0; i < mDescriptorVec.rows; i++)
			paiFile->push_back(m_iID);

	cout << "mFvec.rows = " << m2dfDesc.rows << "\tmDescVec.rows = " << mDescriptorVec.rows << endl;
}



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUsetOfItem
//						//

CUsetOfItem::~CUsetOfItem()
{
	for (int i = 0; i < size(); i++)
		delete (*this)[i];
	clear();
}


int
CUsetOfItem::getId(const string& sItem)
{
	auto				px = m_xMapLabel2id.find(sItem);
	if (px != m_xMapLabel2id.end())
		return	px->second;
	return	-1;
}

bool
CUsetOfItem::insert(const string& sItem, int iRelaPath, void* pxDataset)
{
	if (itemExists(sItem))
		return	false;

	int					iItem = size();
	push_back(makeNew(iItem, sItem, iRelaPath, pxDataset));

	return	m_xMapLabel2id.insert(make_pair(sItem, iItem)).second;
}


void
CUsetOfItem::streamOut(const string& sTitle, ostream& xOut)
{
	xOut << endl;
	xOut << "*************************" << endl;
	xOut << sTitle << endl;
	xOut << "*************************" << endl;
	xOut << "\tID\tlabel" << endl;
	xOut << "\t==\t=====" << endl;

	for (int i=0; i<size(); i++) {
		(*this)[i]->streamOut(xOut);
		xOut << endl;
	}
}

bool
CUsetOfItem::save(const string& sSavePath)
{
	if (!exists_path(sSavePath.c_str())) {
		CString			sMsg = CString("Class-set save path is not valid:\n") + sSavePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}
	std::ofstream		xOutFile(sSavePath + "\\_class-set.txt");
	streamOut(sSavePath, xOutFile);
	xOutFile.close();

	return	true;
}

bool
CUsetOfItem::load(const string& sLoadFilePath, void* pxDataset)
{
	if (!exists_path(sLoadFilePath.c_str(), false)) {
		CString			sMsg = CString("Item-set load path is not valid:\n") + sLoadFilePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}

	std::ifstream		xInFile(sLoadFilePath);
	string				sLine;

	// Drain pre-amble lines
	drainPreamble(xInFile);
	while (std::getline(xInFile, sLine)) {
		if (sLine.empty())	continue;

		stringstream	xInStream(sLine);
		push_back(makeNew(-1, "", -1, pxDataset));
		back()->streamIn(xInStream);

		string			sItem = back()->getLabel();
		int				iItem = back()->getID();
		m_xMapLabel2id.insert(make_pair(sItem, iItem));
	}

	xInFile.close();
	return	true;
}

// CUsetOfItem
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUsetOfClass
//						//

void
CUsetOfClass::streamOut(const string& sTitle, ostream& xOut)
{
	xOut << endl;
	xOut << "*************************" << endl;
	xOut << sTitle << endl;
	xOut << "*************************" << endl;
	xOut << "All-population: " << getPop() << endl;
	xOut << "\tID\tlabel\tpopulation" << endl;
	xOut << "\t==\t=====\t==========" << endl;

	for (int i=0; i<size(); i++) {
		(*this)[i]->streamOut(xOut);
		xOut << endl;
	}
}

void
CUsetOfClass::streamOut(ofstream* pxOut, const string& sTile, EnumClassListingOpt eOpt)
{
//	xxxCUsetItem::streamOut();

	if (pxOut) {
		*pxOut << endl << endl;
		*pxOut << "*************************" << endl;
		*pxOut << "* CLASS STAT" << endl;
		*pxOut << "*************************" << endl;
	}

	cout << endl << endl;
	cout << "*************************" << endl;
	cout << "* CLASS STAT" << endl;
	cout << "*************************" << endl;

	for (int i = 0; i < size(); i++) {
		CUclass*		pxThisClass = (CUclass*) (*this)[i];
		// class-id
		cout << "\t" << pxThisClass->getID();
		if (pxOut)	*pxOut << "\t" << pxThisClass->getID();
		// label
		string	sLabel = pxThisClass->getLabel();
		cout << "\t" << sLabel;
		if (pxOut)	*pxOut << "\t" << sLabel;
		if (sLabel.length() <= 6) {
			cout << "\t";
			if (pxOut)	*pxOut << "\t";
		}
		// pop
		cout << "\t" << pxThisClass->getPop();
		if (pxOut)	*pxOut << "\t" << pxThisClass->getPop();
			// match
		if (eOpt == EnumClassListingOpt::showMatch) {
			cout << "\t" << pxThisClass->getMatchCount();
			cout << "\t" << (float) (100.0 * pxThisClass->getMatchCount() / pxThisClass->getPop()) << "%";
			if (pxOut) {
				*pxOut << "\t" << pxThisClass->getMatchCount();
				*pxOut << "\t" << (float)(100.0 * pxThisClass->getMatchCount() / pxThisClass->getPop()) << "%";
			}
		}
		cout << endl;
		if (pxOut)	*pxOut << endl;
	}
}

int
CUsetOfClass::getPop()
{
	int					nPopCum = 0;
	for (int i=0; i<size(); i++)
		nPopCum += ((CUclass*) (*this)[i])->getPop();
	return	nPopCum;
}


int
CUsetOfClass::getClassPop(int iClass)
{
	if (isValid(iClass))
		return	((CUclass*) (*this)[iClass])->getPop();
	return	-1;
}

string
CUsetOfClass::getClassLabel(int iClass)
{
	if (isValid(iClass))
		return	((CUclass*) (*this)[iClass])->getLabel();
	else
		return	"classIdNotValid";
}

bool
CUsetOfClass::countUpClass(int iClass)
{
	if (isValid(iClass)) {
		((CUclass*) (*this)[iClass])->countUp();
		return	true;
	}
	return	false;
}

bool
CUsetOfClass::countUpClassMatch(int iClass)
{
	if (isValid(iClass)) {
		((CUclass*) (*this)[iClass])->countUpMatch();
		return	true;
	}
	return	false;
}

// CUsetOfClass
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUsetOfData
//						//
bool
CUsetOfData::isValidFile(const string& sFilePath)
{
	string				sExt = getExt(sFilePath);
	if (sExt == "DAT" || sExt == "DAT")
		return	true;
	else
		return	false;
}

void
CUsetOfData::load(const string sDatasetRoot, const string sTrainingSetRoot)
{
	//to-do: Check if dataset root is valid

	cout << "**********************************************************" << endl;
	cout << "Loading dataset from root: " << sDatasetRoot << endl;
	cout << "Training set is: " << sTrainingSetRoot << endl;
	cout << "**********************************************************" << endl << endl;

	m_sDatasetRoot = sDatasetRoot;

	if (1) { //sDatasetRoot != sTrainingSetRoot) {
		m_paxClass->load(sTrainingSetRoot + "\\_class-set.txt", this);
//		m_paxClass->load(m_sDatasetRoot + "\\_class-set.txt", this);
	}

	// List the subdirectories of the root into listing_dir.txt
	string				sListingDir = "listing_dir.txt";
	string				sCommand = "dir /b/o /ad " + sDatasetRoot + " > " + sDatasetRoot + sListingDir;
	system(sCommand.c_str());

	// Read in the data group directories from listing_dir.txt
	std::ifstream		xInFile(m_sDatasetRoot + sListingDir);
	string				sLine;
	while (std::getline(xInFile, sLine))
		m_asRelaPath.push_back(sLine);

	xInFile.close();

	// For each subdirectory, list the data files into listing_file.txt
	for (int i = 0; i < m_asRelaPath.size(); i++) {
		string			sListingFile = "listing_file";
		string			sSubdirPath = "\"" + m_sDatasetRoot + m_asRelaPath[i] + "\"";
		string			sSubdirListingPath = m_sDatasetRoot + sListingFile + "_" + m_asRelaPath[i] + ".txt";
		sCommand = "dir /b/o /a-d " + sSubdirPath + " > " + "\"" + sSubdirListingPath + "\"";
		system(sCommand.c_str());

		cout << "command = " << sCommand << endl;
		cout << "*********** subdir listing path = " << sSubdirListingPath << endl << endl;

		// Get data file names in current subdirectory
		xInFile.open(sSubdirListingPath, std::ifstream::in);
		while (std::getline(xInFile, sLine)) {
			// Check if the file is of supported format
			if (!isValidFile(sLine))	continue;
			// Insert the file item
			if (insert(sLine, i, this))
				tagClassIds2file(sLine);
		}
		xInFile.close();
	}

	streamOut("Data files", cout);
	m_paxClass->streamOut("Data classes", cout);

	if (sDatasetRoot == sTrainingSetRoot)
		cout << "Saving class-set: " << ((m_paxClass->save(m_sDatasetRoot)) ? "succeeded" : "failed") << endl;
	cout << "Saving datum-class-tagging: "
			<< (saveDatumClassTagging(m_sDatasetRoot) ? "succeeded" : "failed") << endl;
}
//						//
void
CUsetOfData::tagClassIds2file(const string& sDatum)
{
// Extract class labels from file name; map the class labels to clasd-id and tag them to the file
// Also count up each corresponding class population
	int					iDatum = getId(sDatum);
	if (!isValid(iDatum)) {
		// Handle exception
		return;
	}
	CUdatum*			pxDatum = (CUdatum*) (*this)[iDatum];
	set<int>*			pgiClass =  pxDatum->classIdSetPtr();
	std::list<string>	lsLabel;
	extractClassLabels(sDatum, "_", '+', ".", lsLabel);

	for (auto psClass = lsLabel.begin(); psClass != lsLabel.end(); psClass++) {
		m_paxClass->insert(*psClass, -1, this);
		int				iClass = m_paxClass->getId(*psClass);
		pgiClass->insert(iClass);
		m_paxClass->countUpClass(iClass);
	}
}


void
CUsetOfData::extractClassLabels(string sFile, string sSep1, char cSep2, string sSep3, std::list<string>& lsLabel)
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
	
	string				sParse, sInput = sMainamble;
	stringstream	
				input_stringstream(sInput);
	while (getline(input_stringstream, sParse, cSep2)) {
		//cout << ">" << string_clean(sParse) << "<" << endl;
		lsLabel.push_back(string_clean(sParse));
	}
}


bool
CUsetOfData::saveDatumClassTagging(const string& sSavePath)
{
	if (!exists_path(sSavePath.c_str())) {
		CString			sMsg = CString("Save path for datum-class tagging is not valid:\n") + sSavePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}

	std::ofstream		xOutFile(sSavePath + "\\_datum-class-tagging.txt");
	for (int i = 0; i < size(); i++) {
		typeIdSet&		giClass = ((CUdatum *) (*this)[i])->getClassIdSet();
		for (typeIdSet::iterator it = giClass.begin(); it != giClass.end(); it++)
			xOutFile << *it << "\t";
		xOutFile << endl;
	}
	xOutFile.close();

	return	true;
}

// CUsetOfData
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUsetOfImage
//						//

bool
CUsetOfImage::isValidFile(const string& sFilePath)
{
	string				sExt = getExt(sFilePath);
	if (sExt == "JPG" || sExt == "JPEG")
		return	true;
	else if (sExt == "BMP")
		return	true;
	else
		return	false;
}

void
CUsetOfImage::extractDesc(cv::Mat& m2dfDesc, vector<int>* paiFile)
{
	for (int i=0; i<size(); i++)
		((CUdatumImage *)(*this)[i])->extractFeatures(m2dfDesc, paiFile);
}


void
CUsetOfImage::getBoVW(cv::Mat& m2fBoVW, void* pxBoVW)
{
	for (int i = 0; i < size(); i++)
		((CUdatumImage*) (*this)[i])->getBoVW(m2fBoVW, pxBoVW);
}



// CUsetOfImage
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<














////////////////////////////////////////////////////////////////////
// xxxCUsetItem

const int
xxxCUsetItem::getId(string sLabel)
{
	auto		pRet = m_xMapLabel2id.find(sLabel);
	return	(pRet == m_xMapLabel2id.end()) ? 0 : pRet->second;
}

void
xxxCUsetItem::streamOut()
{
	cout << "*************************" << endl;
	cout << "* MAP ITEMS" << endl;
	cout << "*************************" << endl;

	//map<string, int>::iterator i = m_xMapLabel2id.begin();
	auto		i = m_xMapLabel2id.begin();
	for (; i != m_xMapLabel2id.end(); i++)
		cout << "\t\t" << i->first << "==>\t" << i->second << endl;
}


//						//
////////////////////////////////////////////////////////////////////
// xxxCUsetClass

xxxCUsetClass&
xxxCUsetClass::operator=(const xxxCUsetClass& rhs)
{
	if (this == &rhs)
		return  *this;
	m_axClass = rhs.m_axClass;

	return  *this;
}


void
xxxCUsetClass::xformBR_getLabels(xxxCUsetData* pxDataset, vector<cv::Mat>& aaiLaberls)
{
	for (int i = 1; i < m_axClass.size(); i++)
		m_axClass[i].xformBR_getLabels(pxDataset, aaiLaberls[i-1]);
}


void
xxxCUsetClass::xformBR_show()
{
	for (int i = 1; i < m_axClass.size(); i++)
		m_axClass[i].xformBR_show();
}

void
xxxCUsetClass::xformBR_setup(xxxCUsetData* pxDataset)
{
	for (int i = 1; i < m_axClass.size(); i++)
		m_axClass[i].xformBR_setup(pxDataset);
}

bool
xxxCUsetClass::save(const string sSavePath)
{
	if (!exists_path(sSavePath.c_str())) {
		CString			sMsg = CString("Class-set save path is not valid:\n") + sSavePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}
	std::ofstream		xOutFile(sSavePath + "_class-set.txt");
	for (int i = 0; i < m_axClass.size(); i++) {
		xOutFile << m_axClass[i].getID() << "\t" 
				<< m_axClass[i].getLabel() << "\t"
				<< m_axClass[i].getPop() << endl;
	}
	xOutFile.close();

	return	true;
}

bool
xxxCUsetClass::load(const string sLoadPath)
{
	string				sLoadFilePath = sLoadPath + "\\_class-set.txt";
	if (!exists_path(sLoadFilePath.c_str(), false)) {
		CString			sMsg = CString("Class-set file path is not valid:\n") + sLoadFilePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}
	std::ifstream		xInFile(sLoadFilePath);
	string				sLine;
	while (std::getline(xInFile, sLine)) {
		string			sParse, sInput = sLine;
		stringstream	inStrStrm(sInput);
		int				iID, nPop;
		string			sLabel;

		std::getline(inStrStrm, sParse, '\t');
		iID = std::stoi(sParse);
		// item-0 is a filler and is already inserted on creation of class-set
		if (iID == 0)	continue;

		std::getline(inStrStrm, sParse, '\t');
		sLabel = sParse;
		std::getline(inStrStrm, sParse, '\t');
		nPop = std::stoi(sParse);

		registerItem(sLabel);
		m_axClass[iID].setPop(nPop);

		cout << "id = " << m_axClass[iID].getID() << "; label = " << m_axClass[iID].getLabel() << "; pop = " << m_axClass[iID].getPop() << endl;
	}

	xInFile.close();
	return	true;
}

bool
xxxCUsetClass::sync(const string sLoadPath)
{
	string				sLoadFilePath = sLoadPath + "\\_class-set.txt";
	if (!exists_path(sLoadFilePath.c_str(), false)) {
		CString			sMsg = CString("Class-set file path is not valid:\n") + sLoadFilePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}
	std::ifstream		xInFile(sLoadFilePath);
	string				sLine;
	while (std::getline(xInFile, sLine)) {
		string			sParse, sInput = sLine;
		stringstream	inStrStrm(sInput);
		int				iID, nPop;
		string			sLabel;

		std::getline(inStrStrm, sParse, '\t');
		iID = std::stoi(sParse);
		// item-0 is a filler and is already inserted on creation of class-set
		if (iID == 0)	continue;

		std::getline(inStrStrm, sParse, '\t');
		sLabel = sParse;
		std::getline(inStrStrm, sParse, '\t');
		nPop = std::stoi(sParse);

		registerItem(sLabel);
		//m_axClass[iID].setPop(nPop);

		cout << "id = " << m_axClass[iID].getID() << "; label = " << m_axClass[iID].getLabel() << "; pop = " << m_axClass[iID].getPop() << endl;
	}

	xInFile.close();
	return	true;
}




xxxCUsetClass::xxxCUsetClass(xxxCUsetClass& rhs1, xxxCUsetClass& rhs2)
{
	for (int i = 0; i < rhs1.m_axClass.size(); i++) {
		string			sRhs1Label = rhs1.getLabel(i);
		int				iRhs2Id = rhs2.getId(sRhs1Label);
		int				nRhs2pop = rhs2.getPop(iRhs2Id);
		CUclass		xClass(i, sRhs1Label, 0);
		xClass.setPop(nRhs2pop);

		m_axClass.push_back(xClass);
	}
}

bool
xxxCUsetClass::registerItem(const string& sClass, int dummy)
{
	if (itemExists(sClass))
		return	false;

	int					iNewItemId = m_axClass.size();
	m_axClass.push_back(CUclass(iNewItemId, sClass, this));

	return	m_xMapLabel2id.insert(make_pair(sClass, iNewItemId)).second;
}

void
xxxCUsetClass::streamOut(ofstream* pxOut, const string& sTile, EnumClassListingOpt eOpt)
{
//	xxxCUsetItem::streamOut();

	if (pxOut) {
		*pxOut << endl << endl;
		*pxOut << "*************************" << endl;
		*pxOut << "* CLASS STAT" << endl;
		*pxOut << "*************************" << endl;
	}

	cout << endl << endl;
	cout << "*************************" << endl;
	cout << "* CLASS STAT" << endl;
	cout << "*************************" << endl;

	for (int i = 0; i < m_axClass.size(); i++) {
		// class-id
		cout << "\t" << m_axClass[i].getID();
		if (pxOut)	*pxOut << "\t" << m_axClass[i].getID();
		// label
		string	sLabel = m_axClass[i].getLabel();
		cout << "\t" << sLabel;
		if (pxOut)	*pxOut << "\t" << sLabel;
		if (sLabel.length() <= 6) {
			cout << "\t";
			if (pxOut)	*pxOut << "\t";
		}
		// pop
		cout << "\t" << m_axClass[i].getPop();
		if (pxOut)	*pxOut << "\t" << m_axClass[i].getPop();
			// match
		if (eOpt == EnumClassListingOpt::showMatch) {
			cout << "\t" << m_axClass[i].getMatchCount();
			cout << "\t" << (float) (100.0 * m_axClass[i].getMatchCount() / m_axClass[i].getPop()) << "%";
			if (pxOut) {
				*pxOut << "\t" << m_axClass[i].getMatchCount();
				*pxOut << "\t" << (float)(100.0 * m_axClass[i].getMatchCount() / m_axClass[i].getPop()) << "%";
			}
		}
		cout << endl;
		if (pxOut)	*pxOut << endl;
	}
}


////////////////////////////////////////////////////////////////////
// xxxCUsetFile

void
xxxCUsetFile::getBoVW(cv::Mat& mfBoVW, void* pxBoVW)
{
	// m_apxDataFile[0] is a dummy
	// Take care of >>>exception<<<
	for (int i = 1; i<m_apxDataFile.size(); i++)
		((CUdatumImage*)m_apxDataFile[i])->getBoVW(mfBoVW, pxBoVW);
}

void
xxxCUsetFile::extractFeatures(cv::Mat& m2dfDescs, vector<int>& paiFile)
{
	// m_apxDataFile[0] is a dummy
	// Take care of >>>exception<<<
	for (int i = 1; i<m_apxDataFile.size(); i++)
		m_apxDataFile[i]->extractFeatures(m2dfDescs, paiFile);
}

bool
xxxCUsetFile::registerItem(const string& sFile, int iSubset)
{
	if (itemExists(sFile))
		return	false;

	int			iNewItemId = m_apxDataFile.size();
	m_apxDataFile.push_back(new CUdatumImage(iNewItemId, sFile, iSubset, this));

	return	m_xMapLabel2id.insert(make_pair(sFile, iNewItemId)).second;
}



void
xxxCUsetFile::streamOut()
{
	cout << "*************************" << endl;
	cout << "* DATA FILES" << endl;
	cout << "*************************" << endl;

	for (int i = 0; i < m_apxDataFile.size(); i++) {
		cout << "\t" << m_apxDataFile[i]->getID() << ":\t" << m_apxDataFile[i]->getLabel() << endl;
		
		typeIdSet&	giClass = m_apxDataFile[i]->getClassIdSet();
		for (typeIdSet::iterator j = giClass.begin(); j != giClass.end(); j++)
			cout << "\t\t" << (*j) << endl;
	}
}

CUdatum*
xxxCUsetFile::getFileItemP(string sFile)
{
	return	m_apxDataFile[getId(sFile)];
}

string
xxxCUsetFile::getGroupLabel(int iSubset)
{
	return	m_rasDataFileSubset[iSubset];
}

bool
xxxCUsetFile::saveFileClassAssoc(const string sSavePath)
{
	if (!exists_path(sSavePath.c_str())) {
		CString			sMsg = CString("Save path for file-class accociation is not valid:\n") + sSavePath.c_str();
		AfxMessageBox(sMsg);
		return	false;
	}

	std::ofstream		xOutFile(sSavePath + "_file-class-assoc.txt");
	for (int i = 1; i < m_apxDataFile.size(); i++) { // 0-th item is a filler. Skip it.
		typeIdSet&	giClass = m_apxDataFile[i]->getClassIdSet();
		for (typeIdSet::iterator it = giClass.begin(); it != giClass.end(); it++)
			xOutFile << *it << "\t";
		xOutFile << endl;
	}
	xOutFile.close();

	return	true;
}




////////////////////////////////////////////////////////////////////
// xxxCUsetData

void
xxxCUsetData::extractClassLabels(string sFile, string sSep1, char cSep2, string sSep3, list<string>& lsLabel)
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
	
	string				sParse, sInput = sMainamble;
	stringstream	
				input_stringstream(sInput);
	while (getline(input_stringstream, sParse, cSep2)) {
		//cout << ">" << string_clean(sParse) << "<" << endl;
		lsLabel.push_back(string_clean(sParse));
	}
}

//						//
/////////////////////////////////////////////////////
void
xxxCUsetData::load(const string sDatasetRoot, bool bForTraining)
{
	m_sDatasetRoot = sDatasetRoot;

	// List the subdirectories of sDatasetRoot into listing_dir.txt
	string				sListingDir = "listing_dir.txt";
	string				sCommand = "dir /b/o /ad " + sDatasetRoot + " > " + sDatasetRoot + sListingDir;
	system(sCommand.c_str());

	// Read in the data group directories from listing_dir.txt
	std::ifstream		xInFile(m_sDatasetRoot + sListingDir);
	string				sLine;
	while (std::getline(xInFile, sLine)) {
		m_asRelaPath.push_back(sLine);
	}
	xInFile.close();

	// For each data group directory, list the data files into listing_file.txt
	for (int i = 0; i < m_asRelaPath.size(); i++) {
		string			sListingFile = "listing_file";
		string			sSubdirPath = "\"" + m_sDatasetRoot + m_asRelaPath[i] + "\"";
		string			sSubdirListingPath = m_sDatasetRoot + sListingFile + "_" + m_asRelaPath[i] + ".txt";
		sCommand = "dir /b/o /a-d " + sSubdirPath + " > " + "\"" + sSubdirListingPath + "\"";
		system(sCommand.c_str());

		cout << "command = " << sCommand << endl;
		cout << "*********** data group listing path = " << sSubdirListingPath << endl << endl;

		// Get data file names in current group directory
		xInFile.open(sSubdirListingPath, std::ifstream::in);
		while (std::getline(xInFile, sLine)) {
			// Check if the file is of supported image format
			if (!isValidImgFile(sLine))	continue;
			// Register file item
			if (m_pxDataFileSet->registerItem(sLine, i))
				// If the file is registered, tag classes to the file item
				tagClass2file(i, sLine);
		}
		xInFile.close();
	}

	m_pxDataFileSet->streamOut();
	m_xDataClassSet.streamOut();

	if (bForTraining) {
		cout << "Saving class-set: " << ((m_xDataClassSet.save(m_sDatasetRoot)) ? "succeeded" : "failed") << endl;
		cout << "Saving association between files and classes: "
			<< ((m_pxDataFileSet->saveFileClassAssoc(m_sDatasetRoot)) ? "succeeded" : "failed") << endl;
	}
}


//						//
void
xxxCUsetData::tagClass2file(int iSubset, string sFile)
{
	CUdatum*		pxFileItem = m_pxDataFileSet->getFileItemP(sFile);

	list<string>		lsLabel;
	extractClassLabels(sFile, "_", '+', ".", lsLabel);
	for (list<string>::iterator it = lsLabel.begin(); it != lsLabel.end(); it++) {
		// Register class. If regisered already, it has no effect
		m_xDataClassSet.registerItem(*it);
		// By now the class is registered and has id
		int			iClass = m_xDataClassSet.getId(*it);
		// Assign class id to file
		pxFileItem->getClassIdSet().insert(iClass);
		m_xDataClassSet.countClass(iClass);
	}
}



void
xxxCUsetData::extractFeatures(cv::Mat& m2dfDescs, vector<int>& paiFile)
{
	m_pxDataFileSet->extractFeatures(m2dfDescs, paiFile);
}


void
xxxCUsetData::getBoVW(cv::Mat& mfBoVW, void* pxBoVW)
{
	m_pxDataFileSet->getBoVW(mfBoVW, pxBoVW);
}




//						//

void
CUclass::xformBR_setup()
{
	int					iThisClass = getID();
	CUsetOfData*		pxDataset = (CUsetOfData*) m_pxDataset;
	for (int i=0; i<pxDataset->size(); i++) {
		set<int>*		pgiDatumClass = ((CUdatum*) (*pxDataset)[i])->classIdSetPtr();
		( pgiDatumClass->find(iThisClass) != pgiDatumClass->end() )
			? m_giDataPosiBR.insert(i) : m_giDataNegaBR.insert(i);
	}
}

void
CUclass::xformBR_getLabels(cv::Mat& mLabel)
{
	int					iThisClass = getID();
	CUsetOfData*		pxDataset = (CUsetOfData*) m_pxDataset;
	for (int i=0; i<pxDataset->size(); i++) {
		set<int>*		pgiDatumClass = ((CUdatum*) (*pxDataset)[i])->classIdSetPtr();
		mLabel.push_back( pgiDatumClass->find(iThisClass) != pgiDatumClass->end() ? 1 : 0 );
	}
}

void
CUclass::xformBR_show()
{
	int					iThisClass = getID();
	cout << endl;
	cout << "class-" << iThisClass << endl;
	cout << "\tpositive = " << m_giDataPosiBR.size() << endl;
	cout << "\tnegative = " << m_giDataNegaBR.size() << endl << endl;;
}




void
CUclass::xformBR_getLabels(xxxCUsetData* pxDataset, cv::Mat& mLabel)
{
	vector< CUdatum* >&
						apxFile = pxDataset->getDataFilePtrArray();
	int					iThisClass = getID();
	for (int i = 1; i < apxFile.size(); i++) {
		set<int>&	giFileClass = apxFile[i]->getClassIdSet();
		if (giFileClass.find(iThisClass) != giFileClass.end())
			mLabel.push_back(1);
		else
			mLabel.push_back(0);
	}

	cout << endl;
	cout << "class-" << iThisClass << " labels" << endl;
	cout << "\tsize = " << mLabel.rows << " x " << mLabel.cols << endl;
	cv::Mat				mOut;
	cv::transpose(mLabel, mOut);
	cout << mOut << endl << endl;

}
void
CUclass::xformBR_setup(xxxCUsetData* pxDataset)
{
	int					iThisClass = getID();
	vector< CUdatum* >&
						apxFile = pxDataset->getDataFilePtrArray();
	for (int i = 1; i < apxFile.size(); i++) {
		set<int>&	giFileClass = apxFile[i]->getClassIdSet();
		if (giFileClass.find(iThisClass) != giFileClass.end())
			m_giDataPosiBR.insert(i);
		else
			m_giDataNegaBR.insert(i);
	}
}

