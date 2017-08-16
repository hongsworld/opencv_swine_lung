#include "stdafx.h"
#include "Echelon_2.h"
#include <opencv2/flann.hpp>

//						//
////////////////////////////////////////////////////////////////////
// xxxCUindexNN

void
xxxCUindexNN::test(xxxCUsetData* pxDataSet)
{
	m_sTrainDataSetPath = pxDataSet->getDataSetPath();
	m_pxTrainDataSet = pxDataSet;

	// m_pxTrainDataSet is assumed set previously
	if (!m_pxTrainDataSet) {
		AfxMessageBox(_T("Training dataset was not loaded yet.\n Please choose the dataset folder and load data"));
		return;
	}

	cout << "**********************************************" << endl;
	cout << "* Extracting descriptor points" << endl;
	cout << "**********************************************" << endl;

	// Extract descriptor ponts
	m_pxTrainDataSet->extractFeatures(m_m2dfDesc, m_aiFile);
	int					nRow = m_m2dfDesc.rows;
	int					nCol = m_m2dfDesc.cols;

	cout << "m_m2dfDesc size (row x col) = " << nRow << " x " << nCol << endl;
	cout << "m_aiFile length = " << m_aiFile.size() << endl << endl;

	cvflann::Matrix<float>
						mxDatasetFX((float*)m_m2dfDesc.data, nRow, nCol);
	nRow = 500;
	m_mfCentroid.create(nRow, nCol, CV_32F);
	cvflann::KMeansIndexParams
						xKmeansParams;
	cvflann::Matrix<float>
						mxCentroid((float*)m_mfCentroid.data, m_mfCentroid.rows, m_mfCentroid.cols);
	int					nCentroid = cvflann::hierarchicalClustering<cvflann::L2<float>>( mxDatasetFX, mxCentroid, xKmeansParams );

	cout << "number of centroids created: " << nCentroid << endl;







	cout << endl;
	cout << "**********************************************" << endl;
	cout << "* FINIS" << endl;
	cout << "**********************************************" << endl;








	//string				sTimestamp = getNowS_1();
	//// preamble
	//G_xLogFile.open("..\\log_" + sTimestamp + "_" + sIndexType + "_index_build.txt");
	//G_xLogFile << endl;
	//G_xLogFile << "**********************************************" << endl;
	//G_xLogFile << "* BUILDING INDEX STARTS (" << sIndexType << ")" << endl;
	//G_xLogFile << "**********************************************" << endl << endl;


	//// Set up index structure to pass for indexing
	//cvflann::Matrix<float>
	//	mxDatasetFX((float*)m_m2dfDesc.data, m_m2dfDesc.rows, m_m2dfDesc.cols);

	//// Check index type
	//if (sIndexType == "composite")
	//	m_pxNNindex = new cvflann::CompositeIndex<cvflann::L1<float>>(mxDatasetFX);
	//else {
	//	G_xLogFile << "Index of type " << sIndexType << " is NOT supported";
	//	G_xLogFile << "Building index is aborted" << endl;
	//	G_xLogFile.close();

	//	G_xOstrStream.str("");
	//	G_xOstrStream << "Index of type " << sIndexType << " is NOT supported";
	//	AfxMessageBox(CString(G_xOstrStream.str().c_str()));
	//	return false;
	//}
	//m_sIndexType = sIndexType;

	//// Build index
	//m_pxNNindex->buildIndex();

	//G_xLogFile << endl;
	//G_xLogFile << "**********************************************" << endl;
	//G_xLogFile << "* BUILDING INDEX FINISHES (" << getIndexType() << ")" << endl;
	//G_xLogFile << "**********************************************" << endl << endl;

	//// Save index
	//string				sIndex = sTimestamp + "_" + getIndexType() + "_index.fln";
	//string				sIndexSavePath("..\\_dump_NNindex\\" + sIndex);
	//saveIndexTo(sIndexSavePath);

	//// Load m_agiClass
	//loadFileClassAssoc(m_sTrainDataSetPath);
	//// Reset m_xClassSet
	//m_xClassSet = *(m_pxTrainDataSet->getClassSetP());

	////// Copy class-set of the training dateset
	////string				sFromPath = m_sTrainDataSetPath + "_class-set.txt";
	////string				sIndexClassSet = sIndex + "_class-set.txt";
	////string				sToPath = "..\\_dump_NNindex\\" + sIndexClassSet;
	////string				sCommand = "copy " + sFromPath + "  " + sToPath;
	////system(sCommand.c_str());


	//// postamble
	//G_xLogFile << "+++++++++++++++++++++++++++++++++" << endl;
	//G_xLogFile << "number of descriptors = " << m_m2dfDesc.rows << "  " << m_aiFile.size() << endl;
	//G_xLogFile << "descriptor length = " << m_m2dfDesc.rows << endl;
	//G_xLogFile << "+++++++++++++++++++++++++++++++++" << endl;
	//G_xLogFile.close();
	//return true;
}



bool
xxxCUindexNN::buildIndexOn(xxxCUsetData* pxDataSet, const string& sIndexType)
{
	string				sOldTrainDataSetPath = m_sTrainDataSetPath;
	m_sTrainDataSetPath = pxDataSet->getDataSetPath();


	xxxCUsetData*			pxOldTrainDataSet = m_pxTrainDataSet;

	//test_IndexLoading(pxDataSet);
	//return true;

	m_pxTrainDataSet = pxDataSet;
	if (!buildIndex(sIndexType)) {
		m_sTrainDataSetPath = sOldTrainDataSetPath;
		m_pxTrainDataSet = pxOldTrainDataSet;
		return false;
	}
	return true;
}


void
xxxCUindexNN::test_IndexLoading(xxxCUsetData* pxDataSet)
{
	cv::Mat1f			mTestData(2, 128);
	float				fLow = -500.0;
	float				fHigh = +500.0;
	cv::randu(mTestData, cv::Scalar(fLow), cv::Scalar(fHigh));

	m_pxTrainDataSet = pxDataSet;
	m_pxTrainDataSet->extractFeatures(m_m2dfDesc, m_aiFile);


	//cout << mTestData << endl << endl;

	// Build index
	cv::flann::Index	xFlannIndex_1;
	xFlannIndex_1.build(
		m_m2dfDesc,
		cv::flann::KDTreeIndexParams(4),
		cvflann::FLANN_DIST_EUCLIDEAN
	);
//	cv::flann::Index	xFlannIndex_1(
//		m_m2dfDesc,
////		mTestData,
//		cv::flann::KDTreeIndexParams(4),
//		cvflann::FLANN_DIST_EUCLIDEAN
//		);
	cout << "kd-tree index building OK" << endl;

	// Save index
	xFlannIndex_1.save("d:\\d_temp\\index.fln");

	vector<int>			ai;
	ai.push_back(m_m2dfDesc.rows);
	ai.push_back(m_m2dfDesc.cols);
	setLinesToFile("d:\\d_temp\\index.fln.size", ai);


	cout << "kd-tree index saving OK" << endl;

	// Load index

	vector<int>			ai2;
	getLinesFromFile("d:\\d_temp\\index.fln.size", ai2);
	cout << "ai2's size = " << ai2.size() << endl;

	cv::Mat1f			mLoadData(ai2[0], ai2[1]);
	cv::flann::Index	xFlannIndex_2;
	//cv::flann::Index	xFlannIndex_2(
	//	//m_m2dfDesc,
	//	mLoadData,
	//	//mTestData,
	//	cv::flann::SavedIndexParams("d:\\d_temp\\index.fln"),
	//	cvflann::FLANN_DIST_EUCLIDEAN
	//	);

	m_m2dfDesc.create(ai2[0], ai2[1], CV_32F);
	bool kkk = xFlannIndex_2.load(m_m2dfDesc, "d:\\d_temp\\index.fln");

	cout << "kkk = " << kkk << endl;

//	cout << "\t" << "index size = " << xFlannIndex_2.size() << endl;
	cout << "kd-tree index loading OK" << endl;
}

//						//
bool
xxxCUindexNN::buildIndex(const string& sIndexType)
{
	// m_pxTrainDataSet is assumed set previously
	if (!m_pxTrainDataSet) {
		AfxMessageBox(_T("Training dataset was not loaded yet.\n Please choose the dataset folder and load data"));
		return	false;
	}

	string				sTimestamp = getNowS_1();
	// preamble
	G_xLogFile.open("..\\log_" + sTimestamp + "_" + sIndexType + "_index_build.txt");
	G_xLogFile << endl;
	G_xLogFile << "**********************************************" << endl;
	G_xLogFile << "* BUILDING INDEX STARTS (" << sIndexType << ")" << endl;
	G_xLogFile << "**********************************************" << endl << endl;

	// Extract features to index
	m_pxTrainDataSet->extractFeatures(m_m2dfDesc, m_aiFile);
	G_xLogFile << "m_m2dfDesc size (row x col) = " << m_m2dfDesc.rows << " x " << m_m2dfDesc.cols << endl;
	G_xLogFile << "m_aiFile length = " << m_aiFile.size() << endl << endl;

	// Set up index structure to pass for indexing
	cvflann::Matrix<float>
						mxDatasetFX((float*)m_m2dfDesc.data, m_m2dfDesc.rows, m_m2dfDesc.cols);

	// Check index type
	if (sIndexType == "composite")
		m_pxNNindex = new cvflann::CompositeIndex<cvflann::L1<float>>(mxDatasetFX);
	else {
		G_xLogFile << "Index of type " << sIndexType << " is NOT supported";
		G_xLogFile << "Building index is aborted" << endl;
		G_xLogFile.close();

		G_xOstrStream.str("");
		G_xOstrStream << "Index of type " << sIndexType << " is NOT supported";
		AfxMessageBox(CString(G_xOstrStream.str().c_str()));
		return false;
	}
	m_sIndexType = sIndexType;

	// Build index
	m_pxNNindex->buildIndex();

	G_xLogFile << endl;
	G_xLogFile << "**********************************************" << endl;
	G_xLogFile << "* BUILDING INDEX FINISHES (" << getIndexType() << ")" << endl;
	G_xLogFile << "**********************************************" << endl << endl;

	// Save index
	string				sIndex = sTimestamp + "_" + getIndexType() + "_index.fln";
	string				sIndexSavePath("..\\_dump_NNindex\\" + sIndex);
	saveIndexTo(sIndexSavePath);

	// Load m_agiClass
	loadFileClassAssoc(m_sTrainDataSetPath);
	// Reset m_xClassSet
	m_xClassSet = *(m_pxTrainDataSet->getClassSetP());

	//// Copy class-set of the training dateset
	//string				sFromPath = m_sTrainDataSetPath + "_class-set.txt";
	//string				sIndexClassSet = sIndex + "_class-set.txt";
	//string				sToPath = "..\\_dump_NNindex\\" + sIndexClassSet;
	//string				sCommand = "copy " + sFromPath + "  " + sToPath;
	//system(sCommand.c_str());


	// postamble
	G_xLogFile << "+++++++++++++++++++++++++++++++++" << endl;
	G_xLogFile << "number of descriptors = " << m_m2dfDesc.rows << "  " << m_aiFile.size() << endl;
	G_xLogFile << "descriptor length = " << m_m2dfDesc.rows << endl;
	G_xLogFile << "+++++++++++++++++++++++++++++++++" << endl;
	G_xLogFile.close();
	return true;
}


void
xxxCUindexNN::saveIndexTo(const string& sIndexFilePath)
{
	// Save index proper
//	m_pxNNindex->save(sIndexFilePath);

	FILE*				pxStreamSave;
	if (pxStreamSave = fopen(sIndexFilePath.c_str(), "wb")) {
		m_pxNNindex->saveIndex(pxStreamSave);
	}
	else {
		AfxMessageBox(_T("problem saving index in\n xxxCUindexNN::saveIndexTo(~)"));
		return;
	}


	//errno_t				iOKzero = fopen_s(&pxStreamSave, sIndexFilePath.c_str(), "w");
	//if (iOKzero == 0)
	//	m_pxNNindex->saveIndex(pxStreamSave);
	//else {
	//	AfxMessageBox(_T("problem saving index in\n xxxCUindexNN::saveIndexTo(~)"));
	//	return;
	//}
	fclose(pxStreamSave);



	cout << "index saved to " << sIndexFilePath << endl;
	cout << "index size: " << m_pxNNindex->size() << endl;



	// Save index data
	cv::FileStorage		xOutFS(sIndexFilePath + ".data.yml", cv::FileStorage::WRITE);
	xOutFS << "m_m2dfDesc" << m_m2dfDesc;
	xOutFS.release();

	// Save index data size
	vector<int>			ai;
	ai.push_back(m_m2dfDesc.rows);
	ai.push_back(m_m2dfDesc.cols);
	setLinesToFile(sIndexFilePath + ".size", ai);
	cout << "index size saved to " << sIndexFilePath + ".size" << endl;

	// Save training dataset location
	vector<string>		as;
	as.push_back(m_sTrainDataSetPath);
	setLinesToFile(sIndexFilePath + ".dataset-path", as);
	cout << "training dataset path saved to " << sIndexFilePath + ".dataset-path" << endl;

	// Save index-file-ID association
	setLinesToFile(sIndexFilePath + ".file-ID", m_aiFile);
	cout << "descriptor-file-ID association saved to " << sIndexFilePath + ".size" << endl << endl << endl;
}

//						//
bool
xxxCUindexNN::loadIndexFrom(const string& sIndexFilePath)
{
	// Read in index size
	vector<int>			ai;
	getLinesFromFile(sIndexFilePath + ".size", ai);
	if (ai.size() != 2) {
		AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T(".size:\n Index size is not well formed"));
		return	false;
	}
	cout << "size of index data to load: " << ai[0] << " x " << ai[1] << endl << endl;

	// Read in training dataset path
	vector<string>		as;
	getLinesFromFile(sIndexFilePath + ".dataset-path", as);
	if (as.size() != 1) {
		AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T(".dataset-path:\n Training dataset path is not well formed"));
		return	false;
	}
	m_sTrainDataSetPath = as[0];


	// Read in descriptor-file-ID association
	vector<int>			aiTemp = m_aiFile;
	m_aiFile.clear();
	getLinesFromFile(sIndexFilePath + ".file-ID", m_aiFile);
	if (m_aiFile.size() != ai[0]) {
		AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T(".file-ID:\n Index sizes don't match"));
		m_aiFile = aiTemp;
		return	false;
	}

	// Reset index data
	m_m2dfDesc.create(ai[0], ai[1], CV_32F);
	//m_m2dfDesc = cv::Mat::zeros(ai[0], ai[1], CV_32F);


	// Load index data
	cv::FileStorage xInFS(sIndexFilePath + ".data.yml", cv::FileStorage::READ);
	xInFS["m_m2dfDesc"] >> m_m2dfDesc;
	xInFS.release();

	cout << "size of index data loaded: " << m_m2dfDesc.rows << "x" << m_m2dfDesc.cols << endl;
	//cin >> G_i;

	// Save old items to roll back in case
//	cv::flann::Index*	pxOldNNindex = m_pxNNindex;
//	cvflann::KMeansIndex<cvflann::L1<float>>*
	cvflann::NNIndex<cvflann::L1<float>>*
						pxOldNNindex = m_pxNNindex;
	xxxCUsetClass			xOldClassSet = m_xClassSet;
	string				sOldIndexType = m_sIndexType;

	// Reset index

	//m_pxNNindex = new cv::flann::Index();
	//m_pxNNindex = new cv::flann::Index(
	//	m_m2dfDesc,
	//	//cv::flann::KDTreeIndexParams(4),
	//	cv::flann::KMeansIndexParams(),
	//	cvflann::FLANN_DIST_EUCLIDEAN
	//	);



	cvflann::Matrix<float>
						mx2dfDatasetFX((float*)m_m2dfDesc.data, m_m2dfDesc.rows, m_m2dfDesc.cols);
	m_pxNNindex = new cvflann::KMeansIndex<cvflann::L1<float>>(mx2dfDatasetFX);
//	m_pxNNindex = new cvflann::KDTreeIndex<cvflann::L1<float>>(mx2dfDatasetFX);

	cout << "mx2dfDatasetFX size: " << mx2dfDatasetFX.rows << "x" << mx2dfDatasetFX.cols << endl;


	//m_pxNNindex = new cvflann::CompositeIndex<cvflann::L1<float>>(mx2dfDatasetFX);

	cout << "index size = " << m_pxNNindex->size() << "x" << m_pxNNindex->veclen() << endl;

	FILE*				pxStreamLoad;
	if (pxStreamLoad = fopen(sIndexFilePath.c_str(), "rb")) {


		m_pxNNindex->loadIndex(pxStreamLoad);
	}
	else {
		AfxMessageBox(_T("problem loading index in\n xxxCUindexNN::loadIndexFrom(~)"));
		return	false;
	}

	//errno_t				iOKzero = fopen_s(&pxStreamLoad, sIndexFilePath.c_str(), "r");
	//if (iOKzero == 0) {

	//	cout << "chkpt-10" << endl;
	//	m_pxNNindex->loadIndex(pxStreamLoad);

	//	cout << "chkpt-12" << endl;
	//}
	//else {
	//	AfxMessageBox(_T("problem loading index in\n xxxCUindexNN::loadIndexFrom(~)"));
	//	return	false;
	//}
	fclose(pxStreamLoad);




	// Load index
	bool				bLoadedRight = false;
		// Load index proper
	if (true) { //m_pxNNindex->load(m_m2dfDesc, sIndexFilePath)) {
			// Load class-set
		xxxCUsetClass		xClassSet;
		if (xClassSet.load(m_sTrainDataSetPath)) {
			m_xClassSet = xClassSet;
				// Load file-class association
			if (loadFileClassAssoc(m_sTrainDataSetPath)) {
					// Extract and set index type
				string		sIndexType = xxxCUindexNN::extractIndexType(sIndexFilePath);
				if (xxxCUindexNN::isValidIndexType(sIndexType)) {
					// All succeeded
					m_sIndexType = sIndexType;
					bLoadedRight = true;
				}
				else
					AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T(":\n Extracting index type failed"));
			}
			else
				AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T("_file-class-assoc.txt:\n Loading file-class-association failed"));
		}
		else
			AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T("_class-set.txt:\n Loading class-set failed"));
	}
	else
		AfxMessageBox(CStringA(sIndexFilePath.c_str()) + _T(":\n cv::flann::Index::load(~) failed"));

	if (bLoadedRight)
		delete pxOldNNindex;
	else {
	// Roll back
		m_sIndexType = sOldIndexType;
		m_xClassSet = xOldClassSet;
		delete m_pxNNindex;
		m_pxNNindex = pxOldNNindex;
		return	false;
	}

//	test_loading_index();
	return true;
}


bool
xxxCUindexNN::loadFileClassAssoc(const string sLoadPath)
{
	m_agiClass.clear();

	string				sLoadFilePath = sLoadPath + "\\_file-class-assoc.txt";
	if (!exists_path(sLoadFilePath.c_str(), false)) {
		cout << "Path for loading file-class association is not valid: " << sLoadFilePath << endl;
		return	false;
	}

	std::ifstream		xInFile(sLoadFilePath);
	string				sLine, sParse, sInput;
	m_agiClass.push_back(list<int>());
	while (std::getline(xInFile, sLine)) {
		sInput = sLine;
		m_agiClass.push_back(list<int>());

		stringstream	xInStrStream(sInput);
		while (getline(xInStrStream, sParse, '\t')) {
			int		iClass = stoi(sParse);
			//cout << iClass << "\t";
			m_agiClass.back().push_back(iClass);
		}
		//cout << endl;
	}
	xInFile.close();

////	testing m_agiClass
//	for (int i = 0; i < m_agiClass.size(); i++) {
//		list<int>::iterator it;
//		for (it = m_agiClass[i].begin(); it != m_agiClass[i].end(); it++)
//			cout << *it << "\t";
//		cout << endl;
//	}
//	cout << m_agiClass.size() << endl;
}


void
xxxCUindexNN::test_loading_index()
{
	cout << "*****************************************" << endl;
	cout << "* loaded index" << endl;
	cout << "*****************************************" << endl << endl;

	cout << "index type: " << getIndexType() << endl;
	cout << "index data size: " << m_m2dfDesc.rows << " x " << m_m2dfDesc.cols << endl;
	cout << "class-set:" << endl;
	m_xClassSet.streamOut();
}

bool
xxxCUindexNN::isValidIndexType(const string& sIndexType)
{
	if (sIndexType == "composite")
		return	true;
	return	false;
}

string
xxxCUindexNN::extractIndexType(const string& sIndexFilePath)
{
//	cout << "sIndexFilePath = " << sIndexFilePath << endl;

	int					iFileDelim = sIndexFilePath.find_last_of("\\");
	string				sIndexFile = iFileDelim == string::npos ? 
	sIndexFilePath : sIndexFilePath.substr(iFileDelim + 1, string::npos);

//	cout << "sIndexFile = " << sIndexFile << endl;

	int					iIndexDelim = sIndexFile.find_last_of("_");
	if (iIndexDelim == string::npos)
		return	"";
	string				sNameStub = sIndexFile.substr(0, iIndexDelim);

//	cout << "sNameStub = " << sNameStub << endl;

	int					iLastDelim = sNameStub.find_last_of("_");
	if (iLastDelim == string::npos)
		return	"";
	string				sIndexType = sNameStub.substr(iLastDelim + 1, string::npos);
	return	sIndexType;
}


//						//
void
xxxCUindexNN::classify(xxxCUsetData* pxTestDataSet, string sInputFilePath, string& sResult)
{
	list<int>		giClass;
	getClassIdSet(sInputFilePath, giClass);
	vector<SUmatchStat>
					axMatchStat;
	matchIndex(sInputFilePath, axMatchStat);

//	cout << "passed matchIndex" << endl;

	if (axMatchStat.size() == 0) {
		cout << "\t!!!!! IMAGE DIDN'T YIELD ANY MATCH !!!!!" << endl;
		return;
	}

	// SUmatchStat = <iClass, nClassCount, nClassPop, fDistMean, fDistMin, f?>
	cout << "\tclass" << "\tfMatch" << "\tmean-d" << "\tmin-d" << "\thit" << "\tpop" << endl;
	for (int k = 0; k < axMatchStat.size(); k++) {
		int		iClass = axMatchStat[k].getClassId();
		cout << "rank-" << k << ":";
		cout << "\t" << iClass;
		cout << "\t" << (float)axMatchStat[k].getMatchMetric();
		cout << "\t" << (float)axMatchStat[k].getDistMean();
		cout << "\t" << (float)axMatchStat[k].getDistMin();
		cout << "\t" << axMatchStat[k].getMatchCount();
		cout << "\t" << axMatchStat[k].getClassPop();
		//cout << "  " << pxTrainClassSet->getLabel(iClass) << endl;
		//cout << "  " << m_xClassSet.getLabel(iClass) << endl;

		if (k == 0)
			//sResult = pxTrainClassSet->getLabel(iClass);
			sResult = m_xClassSet.getLabel(iClass);
	}

	cout << "\t------------------------------------" << endl;
	cout << "\tclass " << "(" << giClass.size() << ")" << "= ";
	for (list<int>::iterator j = giClass.begin(); j != giClass.end(); j++) {
		cout << *j << " ";
		//if (*j == axRet[0].first || axRet[1].second.second <= axRet[0].second.second * 1.1 || axRet[2].second.second <= axRet[0].second.second * 1.1)
		if (*j == axMatchStat[0].getClassId()) {
			//nMatch++;
			cout << "(MATCH) ";
			//xClassTestStat.countClassMatch(*j);
		}
	}
	cout << endl;
	cout << "\tresult = ";
	for (vector<SUmatchStat>::iterator ii = axMatchStat.begin(); ii != axMatchStat.end(); ii++)
		cout << ii->getClassId() << " ";
	cout << endl;
	//cout << "nMatch = " << nMatch << endl << endl;
}

//						//
void
xxxCUindexNN::testIndexOn(xxxCUsetData* pxTestDataSet)
{
	vector< CUdatum* >&
						rapxTestFile = pxTestDataSet->getDataFilePtrArray();

	int					nMatch = 0;
	G_xLogFile << "*****************************************" << endl;
	G_xLogFile << "* TESTING COMMENCES" << endl;
	G_xLogFile << "* DATASET: " << pxTestDataSet->getPath() << endl;
	G_xLogFile << "* DATE-TIME: " << getNowS_1() << endl;
	G_xLogFile << "*****************************************" << endl << endl;

	//xxxCUsetClass*			pxTrainClassSet = m_pxTrainDataSet->getClassSetP();
	xxxCUsetClass*			pxTrainClassSet = &m_xClassSet;
	xxxCUsetClass*			pxTestClassSet = pxTestDataSet->getClassSetP();
	xxxCUsetClass			xClassTestStat(*pxTrainClassSet, *pxTestClassSet);

	cv::Mat				mConfusion = cv::Mat::zeros(6, 6, CV_32S);

	for (int i = 1; i < rapxTestFile.size(); i++) {
		string			sInputFilePath = rapxTestFile[i]->getFilePath();
		list<int>		giClass;
		getClassIdSet(sInputFilePath, giClass);

		//		vector<typePair_IX>
		//						axRet;
		G_xLogFile << "*****************************************" << endl;
		G_xLogFile << "File = " << i << ": " << sInputFilePath << endl;

		vector < SUmatchStat >
						axMatchStat;
		matchIndex(sInputFilePath, axMatchStat);

		if (axMatchStat.size() == 0) {
			G_xLogFile << "\t!!!!! IMAGE DIDN'T YIELD ANY MATCH !!!!!" << endl;
			continue;
		}

		// SUmatchStat = <iClass, nClassCount, nClassPop, fDistMean, fDistMin, f?>
		G_xLogFile << "\tclass" << "\tfMatch" << "\tmean-d" << "\tmin-d" << "\thit" << "\tpop" << endl;
		for (int k = 0; k < axMatchStat.size(); k++) {
			int		iClass = axMatchStat[k].getClassId();
			G_xLogFile << "rank-" << k << ":";
			G_xLogFile << "\t" << iClass;
			G_xLogFile << "\t" << (float)axMatchStat[k].getMatchMetric();
			G_xLogFile << "\t" << (float)axMatchStat[k].getDistMean();
			G_xLogFile << "\t" << (float)axMatchStat[k].getDistMin();
			G_xLogFile << "\t" << axMatchStat[k].getMatchCount();
			G_xLogFile << "\t" << axMatchStat[k].getClassPop();
			G_xLogFile << "  " << pxTrainClassSet->getLabel(iClass) << endl;
		}

		G_xLogFile << "\t------------------------------------" << endl;
		G_xLogFile << "\tclass " << "(" << giClass.size() << ")" << "= ";
		for (list<int>::iterator j = giClass.begin(); j != giClass.end(); j++) {
			G_xLogFile << *j << " ";
			//if (*j == axRet[0].first || axRet[1].second.second <= axRet[0].second.second * 1.1 || axRet[2].second.second <= axRet[0].second.second * 1.1)

			mConfusion.at<int>(*j, axMatchStat[0].getClassId())++; // = mConfusion.at<int>(*j, axMatchStat[0].getClassId()) + 1;

			if (*j == axMatchStat[0].getClassId()) {
				nMatch++;
				G_xLogFile << "(MATCH) ";
				xClassTestStat.countClassMatch(*j);
			}
		}
		G_xLogFile << endl;
		G_xLogFile << "\tresult = ";
		for (vector<SUmatchStat>::iterator ii = axMatchStat.begin(); ii != axMatchStat.end(); ii++)
			G_xLogFile << ii->getClassId() << " ";
		G_xLogFile << endl;
		G_xLogFile << "nMatch = " << nMatch << endl << endl;
	}

	G_xLogFile << endl << endl;
	G_xLogFile << "*****************************************" << endl;
	G_xLogFile << "* TESTING DONE" << endl;
	G_xLogFile << "* TRAINING DATASET: " << m_sTrainDataSetPath << endl;
	G_xLogFile << "* TESTING DATASET: " << pxTestDataSet->getPath() << endl;
	G_xLogFile << "* DATE-TIME: " << getNowS_1() << endl;
	G_xLogFile << "*****************************************" << endl << endl;
	G_xLogFile << "nMatch = " << nMatch << " out of " << rapxTestFile.size() - 1; // Effective file id starts from 1, not 0
	G_xLogFile << ": " << (float)(100.0 * nMatch) / (rapxTestFile.size() - 1) << "%" << endl;

	pxTrainClassSet->streamOut(&G_xLogFile, "Training class-set");
	pxTestClassSet->streamOut(&G_xLogFile, "Testing class-set");
	xClassTestStat.streamOut(0, "", EnumClassListingOpt::showMatch);

	cout << endl << mConfusion << endl << endl;
	G_xLogFile << endl << mConfusion << endl << endl;
}





void				
xxxCUindexNN::getMatchStat(vector<typePair_IF>& axIdDist, typeMap_I2matchStat& xMapI2matchStat, int nTopWhat)
{
	//sort(axIdDist.begin(), axIdDist.end(), typePair_IF_less);

	// SUmatchStat = <iClass, nClassCount, nClassPop, fDistMean, fDistMin, f?>
	//for (int i = 0; i < axIdDist.size() && i < nTopWhat; i++) {
	for (int i = 0; i < axIdDist.size(); i++) {
		int				iFile = axIdDist[i].first;
		float			fDist = axIdDist[i].second;

		//cout << "i = " << i << "\t iFile = " << iFile << "\t ali size = " << m_agiClass.size() << endl;
		//typeIdList&		giClass = m_pxTrainDataSet->getClassIdSet(iFile);
		typeIdList&		giClass = m_agiClass[iFile];
		
		for (typeIdList::iterator j = giClass.begin(); j != giClass.end(); j++) {
			int			iClass = *j;
			auto		pRet = xMapI2matchStat.find(iClass);

			//G_xLogFile << iClass << "\t" << (float)fDist << endl;

			if (pRet == xMapI2matchStat.end()) {
				// not in the map. Insert.
				//int		nClassPop = m_pxTrainDataSet->getClassPop(iClass);
				int		nClassPop = m_xClassSet.getPop();
				SUmatchStat
						xMatchStat(iClass, 1, nClassPop, fDist, fDist, -1.0);
				xMapI2matchStat.insert(make_pair(iClass, xMatchStat));
			}
			else {
				// in the map. Update.
				SUmatchStat&	rxMatchStat = pRet->second;
				rxMatchStat.getMatchCount()++;
				rxMatchStat.getDistMean() += fDist;
				rxMatchStat.getDistMin() = min(rxMatchStat.getDistMin(), fDist);
			}
			//G_xLogFile << "-----------------------" << endl;
		}
	}

}
//						//
void
xxxCUindexNN::matchIndex(string sInputFilePath, vector<SUmatchStat>& axMatchStat)
{
	vector < typePair_IF >
						axIdDist;
	matchIndex(sInputFilePath, axIdDist);

	typeMap_I2matchStat	xMapI2matchStat;
	int					nTopWhat = 200;
	getMatchStat(axIdDist, xMapI2matchStat, nTopWhat);

	// Sum up and sort match-stat
	// SUmatchStat = <iClass, nClassCount, nClassPop, fDistMean, fDistMin, ?>
	//int					nTotalPop = m_pxTrainDataSet->getClassPop();
	int					nTotalPop = m_xClassSet.getPop();

	typeMap_I2matchStat::iterator
						pxStat;
	for (pxStat = xMapI2matchStat.begin(); pxStat != xMapI2matchStat.end(); pxStat++) {
		SUmatchStat&	rxStat = pxStat->second;
		rxStat.getDistMean() = (float) rxStat.getDistMean() / rxStat.getMatchCount();

		rxStat.getPopRatio() = (float) rxStat.getClassPop() / nTotalPop;

		cout << rxStat.getClassId() << "\t" << rxStat.getMatchMetric() << endl;
		axMatchStat.push_back(rxStat);
	}
	sort(axMatchStat.begin(), axMatchStat.end(), SUmatchStat_less);
	//sort(axMatchStat.begin(), axMatchStat.end(), SUmatchStat_greater);
}


void
showKeypt(string sPath, vector<cv::KeyPoint>& axKeypt)
{
	const cv::Mat	xmImg_1 = cv::imread(sPath);
	cv::Mat			xmImg_2; //= xmImg_1.clone();

	cv::resize(xmImg_1, xmImg_2, cv::Size(1600, 1200));    // Size = [width (cols), height (rows)]

	cv::Mat	xmImgOut;
	//cv::drawKeypoints(xmImg_2, vxKeypoint, xmImgOut);
	cv::drawKeypoints(xmImg_2, axKeypt, xmImgOut, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	char* source_window = "Source";
	//cv::namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	cv::namedWindow(source_window, CV_WINDOW_NORMAL);
	cv::imshow(source_window, xmImgOut);

}

						
void
xxxCUindexNN::matchIndex(string sInputFilePath, vector<typePair_IF>& axRet)
{
	CUdatumImage			xImgFile;
	cv::Mat				m2dfDesc;
	vector<int>			aiFile;

	std::vector<cv::KeyPoint>
						axKeypt, axKeyptMatched;
	// Extract feature vectors
	xImgFile.extractFeatures(sInputFilePath, m2dfDesc, aiFile, axKeypt);

	// Display keypoints
	showKeypt(sInputFilePath, axKeypt);

	//cout << "keypoints displayed (" << axKeypt.size() << ")" << endl;
	//cout << "descriptors = " << m2dfDesc.rows << endl;
	//int k;
	//cin >> k;

	int					nDescriptor = m2dfDesc.rows;
	int					nKnn = 2;
	cv::Mat1i			mi(nDescriptor, nKnn);
	cv::Mat1f			mfDist(nDescriptor, nKnn);

	//cv::Mat				mi(nDescriptor, nKnn, CV_32S);
	//cv::Mat				mfDist(nDescriptor, nKnn, CV_32F);

//	cv::Mat nresps(vfv_reg_quie.size(), K, CV_32S);
//	cv::Mat dists(vfv_reg_quie.size(), K, CV_32F);

	cvflann::Matrix<float>
		mxInput((float*)m2dfDesc.data, m2dfDesc.rows, m2dfDesc.cols);
	cvflann::Matrix<int>
		mxi((int*)mi.data, nDescriptor, nKnn);
	cvflann::Matrix<float>
		mx2dfDist(new float[nDescriptor * nKnn], nDescriptor, nKnn);

	m_pxNNindex->knnSearch(mxInput, mxi, mx2dfDist, nKnn, cvflann::SearchParams(64));


	//m_pxNNindex->knnSearch(m2dfDesc, mi, mfDist, nKnn, cvflann::SearchParams(64));
//	cout << mi << endl;

	//cout << mfDist << endl;



	// Filter out likely incorrect matches using distance-ratio
	for (int i = 0; i < mxi.rows; i++) {
		//float			fDist1 = *(mx2dfDist[i] + 0), fDist2 = *(mx2dfDist[i] + 1);
		float			fDist1 = mx2dfDist[i][0], fDist2 = mx2dfDist[i][1];
		float			fDistRatio = fDist1 / fDist2;
		//if (fDistRatio > 0.76)	continue;
		if (fDistRatio > 0.8)	continue;

		axKeyptMatched.push_back(axKeypt[i]);
		for (int j = 0; j < 1; j++) // nKnn; j++)
			//axRet.push_back(make_pair(m_aiFile[*(mxi[i] + j)], fDist1));
			axRet.push_back(make_pair(m_aiFile[mxi[i][j]], fDist1));
	}
	return;


	// Filter out likely incorrect matches using distance-ratio
	for (int i = 0; i < nDescriptor; i++) {
		float			fDist1 = mx2dfDist[i][0]; // .at<float>(i, 0);
		float			fDist2 = mfDist.at<float>(i, 1);
		float			fDistRatio = fDist1 / fDist2;
		if (fDistRatio > 0.85)	continue;

		axKeyptMatched.push_back(axKeypt[i]);
		for (int j = 0; j < 1; j++)  // nKnn; j++)
			axRet.push_back(make_pair(m_aiFile[ mi.at<int>(i, j) ], fDist1));
	}

}



inline void
xxxCUindexNN::getClassIdSet(string sInputFilePath, list<int>& rgiClass)
{
	list<string>		lsClass;
	size_t				iFileDelim = sInputFilePath.find_last_of("\\");
	string				sFile = sInputFilePath.substr(iFileDelim + 1, string::npos);


	xxxCUsetData::extractClassLabels(sFile, "_", '+', ".", lsClass);

	for (list<string>::iterator i = lsClass.begin(); i != lsClass.end(); i++)
		//rgiClass.push_back(m_pxTrainDataSet->getClassId(*i));
		rgiClass.push_back(m_xClassSet.getId(*i));
}

