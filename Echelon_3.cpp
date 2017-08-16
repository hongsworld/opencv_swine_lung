#include "stdafx.h"
#include "Echelon_3.h"



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUfxBoVW
//	
void
CUfxBoVW::buildDictionary()
{
	// Training dataset is assumed set previously
	if (!m_pxTrainDataset) {
		AfxMessageBox(_T("Training image set was not loaded yet.\n Please choose the dataset folder"));
		return;
	}

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "Building dictionary" << endl;
	cout << "Extracting descriptors" << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl << endl;

	// Extract descriptors
	cv::Mat				m2dfDesc;
	m_pxTrainDataset->extractDesc(m2dfDesc, &m_aiFile);

	int					nRowTrainDesc = m2dfDesc.rows;
	int					nColTrainDesc = m2dfDesc.cols;

	cout << "m2dfDesc size (row x col) = " << nRowTrainDesc << " x " << nColTrainDesc << endl;
	cout << "number of image files = " << m_aiFile.size() << endl << endl;

	cvflann::Matrix<float>
						mx2dfDesc((float*)m2dfDesc.data, nRowTrainDesc, nColTrainDesc);
	
	int					nDictionarySizeRequested = 500;
	m_m2dfDictionary.create(nDictionarySizeRequested, nColTrainDesc, CV_32F);
	cvflann::KMeansIndexParams
						xKmeansParams;
	cvflann::Matrix<float>
						mx2dfDictionary((float*)m_m2dfDictionary.data, m_m2dfDictionary.rows, m_m2dfDictionary.cols);
	int					nDictionarySize = cvflann::hierarchicalClustering<cvflann::L2<float>>(mx2dfDesc, mx2dfDictionary, xKmeansParams);
	// Fit the dictionary to the real size. It is assumed that nDictionarySize <= nDictionarySizeRequested
	if (nDictionarySize < nDictionarySizeRequested)
		m_m2dfDictionary = m_m2dfDictionary.rowRange(cv::Range(0, nDictionarySize));

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "Dictionary built" << endl;
	cout << "Dictionary size: requested = " << nDictionarySizeRequested << "; created = " << nDictionarySize << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl << endl;
}

//						//
void
CUfxBoVW::test(CUsetOfData* pxDataset)
{
	m_pxTrainDataset = pxDataset;

	//buildDictionary();
	//saveDictionary("D://(VX.4h-2//_jc3//_ws//vsce13_ws//_IBIR_2//_dump_dictionary//dictionary170220_2small.Mat.yml");
	loadDictionary("D://(VX.4h-2//_jc3//_ws//vsce13_ws//_IBIR_2//_dump_dictionary//dictionary170220.Mat.yml");
	cout << "dictionary size = " << m_m2dfDictionary.rows << " x " << m_m2dfDictionary.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;

	cv::Mat				m2fBoVW4train;
	//((CUsetOfImage*) m_pxTrainDataset)->getBoVW(m2fBoVW4train, this);
	//saveMat(m2fBoVW4train, "m2fBoVW4train", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//train-170220_big.mat.yml");
	
	loadMat(m2fBoVW4train, "m2fBoVW4train", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//train-170220_big.mat.yml");
	cout << "training BoVW size = " << m2fBoVW4train.rows << " x " << m2fBoVW4train.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;



	m_pxTrainDataset->classes().xformBR_setup();
	m_pxTrainDataset->classes().xformBR_show();


	vector<cv::Mat>		am1iBRlabel_train;
	int					nClasses = pxDataset->classes().count();
	cout << "number of training classes: " << nClasses << endl << endl;
	am1iBRlabel_train.resize(nClasses);
	pxDataset->classes().xformBR_getLabels(am1iBRlabel_train);

	CUmlSvmplex				axSvm(nClasses);
	axSvm.train(m2fBoVW4train, am1iBRlabel_train);

	cout << endl << endl;
	cout << "SVM training finished" << endl << endl;
//	cout << "DONE" << endl;
//	return;



/////////////////////////////////////
// TESTING
/////////////////////////////////////
	CUsetOfImage		xTestDataset;
	xTestDataset.load(
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\testing\\",
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\training\\");

	cout << endl;
	cout << "test dataset loaded" << endl;
//	cout << "DONE" << endl;
//	return;



	cv::Mat				m2fBoVW4test;
	//xTestDataset.getBoVW(m2fBoVW4test, this);
	//saveMat(m2fBoVW4test, "m2fBoVW4test", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//test-170220.mat.yml");
	loadMat(m2fBoVW4test, "m2fBoVW4test", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//test-170220.mat.yml");
	cout << endl << endl;
	cout << "test BoVW size = " << m2fBoVW4test.rows << " x " << m2fBoVW4test.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;

	

	vector<cv::Mat>		am1iBRlabel4test;
	nClasses = xTestDataset.classes().count();
	cout << "number of testing classes: " << nClasses << endl << endl;
	am1iBRlabel4test.resize(nClasses);
	xTestDataset.classes().xformBR_getLabels(am1iBRlabel4test);

	cout << "***********************" << endl;
//	cout << "DONE" << endl;
//	return;

	vector<set<int>>	agiClass;
	//vector< CUdatum* >&
	//					apxTestImgFileP = xTestDataset.getDataFilePtrArray();
	//int					nFiles = apxTestImgFileP.size();
	int					nFiles = xTestDataset.size();
	agiClass.resize(nFiles);

	cout << "nFiles = " << nFiles << endl;
	for (int i = 0; i < nFiles; i++)
		//agiClass[i] = apxTestImgFileP[i]->getClassIdSetPtr();
		agiClass[i] = ((CUdatum*) xTestDataset[i])->getClassIdSet();

	axSvm.predict(m2fBoVW4test, am1iBRlabel4test);
	return;


	CUsetOfClass*		pxClassSet = &xTestDataset.classes();
	axSvm.test(m2fBoVW4test, agiClass, pxClassSet);
	cout << "DONE" << endl;
	return;


	//string				sImgFile("D://UDA//_SNUP_1_dataset//dataset_01_dodram14//training//Enzootic//14.05.02_누리_유행성폐렴 + 바이러스성폐렴-1.JPG");

	//cv::Mat				vfBoVW;
	//getBoVW(sImgFile, vfBoVW);

	//cout << "vfBoVW size = " << vfBoVW.rows << " x " << vfBoVW.cols << endl;
	//cout << vfBoVW << endl;
}


//						//
void
CUfxBoVW::test2(CUsetOfData* pxDataset)
{
	m_pxTrainDataset = pxDataset;

	//buildDictionary();
	//saveDictionary("D://(VX.4h-2//_jc3//_ws//vsce13_ws//_IBIR_2//_dump_dictionary//dictionary170220_2small.Mat.yml");
	loadDictionary("D://(VX.4h-2//_jc3//_ws//vsce13_ws//_IBIR_2//_dump_dictionary//dictionary170220.Mat.yml");
	cout << "dictionary size = " << m_m2dfDictionary.rows << " x " << m_m2dfDictionary.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;

	cv::Mat				m2fBoVW4train;
	//((CUsetOfImage*) m_pxTrainDataset)->getBoVW(m2fBoVW4train, this);
	//saveMat(m2fBoVW4train, "m2fBoVW4train", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//train-170220_big.mat.yml");
	
	loadMat(m2fBoVW4train, "m2fBoVW4train", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//train-170220_big.mat.yml");
	cout << "training BoVW size = " << m2fBoVW4train.rows << " x " << m2fBoVW4train.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;



	m_pxTrainDataset->classes().xformBR_setup();
	m_pxTrainDataset->classes().xformBR_show();


	vector<cv::Mat>		am1iBRlabel_train;
	int					nClasses = pxDataset->classes().count();
	cout << "number of training classes: " << nClasses << endl << endl;

	am1iBRlabel_train.resize(nClasses);
	pxDataset->classes().xformBR_getLabels(am1iBRlabel_train);

	
	cv::Mat				m2Label;
	cv::hconcat(am1iBRlabel_train, m2Label);
	CUcmlCCdata			xCmlCCdata(m2fBoVW4train, m2Label);

	CUmlSvmplex			axSvm(nClasses);

//	axSvm.train(m2fBoVW4train, am1iBRlabel_train);
	axSvm.train(xCmlCCdata);

	cout << endl << endl;
	cout << "SVM training-2 finished" << endl << endl;
//	cout << "DONE" << endl;
//	return;



/////////////////////////////////////
// TESTING
/////////////////////////////////////
	CUsetOfImage		xTestDataset;
	xTestDataset.load(
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\testing\\",
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\training\\");

	cout << endl;
	cout << "test dataset loaded" << endl;
//	cout << "DONE" << endl;
//	return;



	cv::Mat				m2fBoVW4test;
	//xTestDataset.getBoVW(m2fBoVW4test, this);
	//saveMat(m2fBoVW4test, "m2fBoVW4test", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//test-170220.mat.yml");
	loadMat(m2fBoVW4test, "m2fBoVW4test", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//test-170220.mat.yml");
	cout << endl << endl;
	cout << "test BoVW size = " << m2fBoVW4test.rows << " x " << m2fBoVW4test.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;

	

	vector<cv::Mat>		am1iBRlabel4test;
	nClasses = xTestDataset.classes().count();
	cout << "number of testing classes: " << nClasses << endl << endl;
	am1iBRlabel4test.resize(nClasses);
	xTestDataset.classes().xformBR_getLabels(am1iBRlabel4test);

	cout << "***********************" << endl;
//	cout << "DONE" << endl;
//	return;

	vector<set<int>>	agiClass;
	//vector< CUdatum* >&
	//					apxTestImgFileP = xTestDataset.getDataFilePtrArray();
	//int					nFiles = apxTestImgFileP.size();
	int					nFiles = xTestDataset.size();
	agiClass.resize(nFiles);

	cout << "nFiles = " << nFiles << endl;
	for (int i = 0; i < nFiles; i++)
		//agiClass[i] = apxTestImgFileP[i]->getClassIdSetPtr();
		agiClass[i] = ((CUdatum*) xTestDataset[i])->getClassIdSet();



	cv::Mat				m2Label4test;
	cv::hconcat(am1iBRlabel4test, m2Label4test);
	CUcmlCCdata			xCmlCCdata2(m2fBoVW4test, m2Label4test);

//	axSvm.predict(m2fBoVW4test, am1iBRlabel4test);
	axSvm.predict(xCmlCCdata2, am1iBRlabel4test);
	return;


	CUsetOfClass*		pxClassSet = &xTestDataset.classes();
	axSvm.test(m2fBoVW4test, agiClass, pxClassSet);
	cout << "DONE" << endl;
	return;


	//string				sImgFile("D://UDA//_SNUP_1_dataset//dataset_01_dodram14//training//Enzootic//14.05.02_누리_유행성폐렴 + 바이러스성폐렴-1.JPG");

	//cv::Mat				vfBoVW;
	//getBoVW(sImgFile, vfBoVW);

	//cout << "vfBoVW size = " << vfBoVW.rows << " x " << vfBoVW.cols << endl;
	//cout << vfBoVW << endl;
}



//						//
void
CUfxBoVW::test3(CUsetOfData* pxDataset)
{
	m_pxTrainDataset = pxDataset;

	//buildDictionary();
	//saveDictionary("D://(VX.4h-2//_jc3//_ws//vsce13_ws//_IBIR_2//_dump_dictionary//dictionary170220_2small.Mat.yml");
	loadDictionary("D://(VX.4h-2//_jc3//_ws//vsce13_ws//_IBIR_2//_dump_dictionary//dictionary170220.Mat.yml");
	cout << "dictionary size = " << m_m2dfDictionary.rows << " x " << m_m2dfDictionary.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;

	cv::Mat				m2fBoVW4train;
	//((CUsetOfImage*) m_pxTrainDataset)->getBoVW(m2fBoVW4train, this);
	//saveMat(m2fBoVW4train, "m2fBoVW4train", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//train-170220_big.mat.yml");
	
	loadMat(m2fBoVW4train, "m2fBoVW4train", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//train-170220_big.mat.yml");
	cout << "training BoVW size = " << m2fBoVW4train.rows << " x " << m2fBoVW4train.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;


	m_pxTrainDataset->classes().xformBR_setup();
	m_pxTrainDataset->classes().xformBR_show();


	vector<cv::Mat>		am1iBRlabel_train;
	int					nClasses = pxDataset->classes().count();
	cout << "number of training classes: " << nClasses << endl << endl;

	am1iBRlabel_train.resize(nClasses);
	pxDataset->classes().xformBR_getLabels(am1iBRlabel_train);

	
	cv::Mat				m2Label;
	cv::hconcat(am1iBRlabel_train, m2Label);
	CUcmlCCdata			xCmlCCdata(m2fBoVW4train, m2Label);

	CUmlSvmplex			axSvm(nClasses);

//	axSvm.train(m2fBoVW4train, am1iBRlabel_train);
	axSvm.train(xCmlCCdata);

	cout << endl << endl;
	cout << "SVM training-2 finished" << endl << endl;
//	cout << "DONE" << endl;
//	return;



/////////////////////////////////////
// TESTING
/////////////////////////////////////
	CUsetOfImage		xTestDataset;
	xTestDataset.load(
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\testing\\",
		"D:\\_data\\_SNUP@0j\\pig\\dodram14\\02\\training\\");

	cout << endl;
	cout << "test dataset loaded" << endl;
//	cout << "DONE" << endl;
//	return;



	cv::Mat				m2fBoVW4test;
	//xTestDataset.getBoVW(m2fBoVW4test, this);
	//saveMat(m2fBoVW4test, "m2fBoVW4test", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//test-170220.mat.yml");
	loadMat(m2fBoVW4test, "m2fBoVW4test", "d://(vx.4h-2//_jc3//_ws//vsce13_ws//_ibir_2//_dump_bovw//test-170220.mat.yml");
	cout << endl << endl;
	cout << "test BoVW size = " << m2fBoVW4test.rows << " x " << m2fBoVW4test.cols << endl << endl;
//	cout << "DONE" << endl;
//	return;

	

	vector<cv::Mat>		am1iBRlabel4test;
	nClasses = xTestDataset.classes().count();
	cout << "number of testing classes: " << nClasses << endl << endl;
	am1iBRlabel4test.resize(nClasses);
	xTestDataset.classes().xformBR_getLabels(am1iBRlabel4test);

	cout << "***********************" << endl;
//	cout << "DONE" << endl;
//	return;

	vector<set<int>>	agiClass;
	//vector< CUdatum* >&
	//					apxTestImgFileP = xTestDataset.getDataFilePtrArray();
	//int					nFiles = apxTestImgFileP.size();
	int					nFiles = xTestDataset.size();
	agiClass.resize(nFiles);

	cout << "nFiles = " << nFiles << endl;
	for (int i = 0; i < nFiles; i++)
		//agiClass[i] = apxTestImgFileP[i]->getClassIdSetPtr();
		agiClass[i] = ((CUdatum*) xTestDataset[i])->getClassIdSet();



	cv::Mat				m2Label4test;
	cv::hconcat(am1iBRlabel4test, m2Label4test);
	CUcmlCCdata			xCmlCCdata2(m2fBoVW4test, m2Label4test);

//	axSvm.predict(m2fBoVW4test, am1iBRlabel4test);
	axSvm.predict(xCmlCCdata2, am1iBRlabel4test);
	return;


	CUsetOfClass*		pxClassSet = &xTestDataset.classes();
	axSvm.test(m2fBoVW4test, agiClass, pxClassSet);
	cout << "DONE" << endl;
	return;


	//string				sImgFile("D://UDA//_SNUP_1_dataset//dataset_01_dodram14//training//Enzootic//14.05.02_누리_유행성폐렴 + 바이러스성폐렴-1.JPG");

	//cv::Mat				vfBoVW;
	//getBoVW(sImgFile, vfBoVW);

	//cout << "vfBoVW size = " << vfBoVW.rows << " x " << vfBoVW.cols << endl;
	//cout << vfBoVW << endl;
}


//						//
void
CUfxBoVW::getBoVW(const string& sImgPath, cv::Mat& vfBoBW)
{
	CUdatumImage		xImgFile;
	cv::Mat				mfDesc;
	vector<int>			aiFile;
	std::vector<cv::KeyPoint>
						axKeypt, axKeyptMatched;
	// Extract feature vectors
	xImgFile.extractFeatures((string&)sImgPath, mfDesc, aiFile, axKeypt);
	cout << "mfDesc size = " << mfDesc.rows << " x " << mfDesc.cols << endl;
	//cout << mfDesc << endl;


	int					nDesc = mfDesc.rows;
	int					nCentroid = m_m2dfDictionary.rows;
	cv::Mat1f			mfMu(nCentroid, nDesc);

//>>>>>>>>>>>>>>>>>>>>>>
// encoding
	vector<priority_queue<typePair_IF, vector<typePair_IF>, IF_pair_less >>
						axPQknn(nDesc);
	vector<map<int, float>>
						axMapDescKnn(nDesc);
	vector<float>		afKnnDistSum(nDesc);

	// computes (Euclidean) distances between desc-i and centroid-j and inserts into map
	for (int i = 0; i < nDesc; i++) {
		for (int j = 0; j < m_m2dfDictionary.rows; j++) {
			float		fDistIJ = dist(mfDesc.row(i), m_m2dfDictionary.row(j));

			axPQknn[i].push({ j, fDistIJ});
			axMapDescKnn[i].insert({ j, fDistIJ });
		}

		// keeps only k-nearest neighbors
		int				nKnn = 100;
		while (axPQknn[i].size() > nKnn) {
			int			j = axPQknn[i].top().second;
			axPQknn[i].pop();
			auto		px = axMapDescKnn[i].find(j);
			if (px != axMapDescKnn[i].end())
				axMapDescKnn[i].erase(px);
		}
	}
	// fills up mfMu
	fillBoVWmat(mfMu, axMapDescKnn);
//<<<<<<<<<<<<<<<<<<<<<<

//>>>>>>>>>>>>>>>>>>>>>>
// pooling
	reduce(mfMu, vfBoBW, 1, CV_REDUCE_MAX, -1);

	cv::transpose(vfBoBW, vfBoBW);
//<<<<<<<<<<<<<<<<<<<<<<
}


inline void
CUfxBoVW::fillBoVWmat(cv::Mat1f& mfMu, vector<map<int, float>>& axMapDescKnn)
{
	float				fBeta = 1.0/10;
	for (int iCol = 0; iCol < axMapDescKnn.size(); iCol++)
		for (int iRow = 0; iRow < mfMu.rows; iRow++) {
			mfMu.at<float>(iRow, iCol) = 0;
			auto		px = axMapDescKnn[iCol].find(iRow);
			if (px != axMapDescKnn[iCol].end()) {
				float	fLocalDist = px->second;
				mfMu.at<float>(iRow, iCol) = exp(-fBeta * fLocalDist);
			}
		}
	cv::Mat				afLocalDistSum;

	reduce(mfMu, afLocalDistSum, 0, CV_REDUCE_SUM, -1);
	for (int iCol = 0; iCol < axMapDescKnn.size(); iCol++)
		mfMu.col(iCol) = mfMu.col(iCol) / afLocalDistSum.at<float>(iCol);
}

//cv::Mat C = (cv::Mat_<float>(3, 3) << 1, 2, 3, 4, 5, 6, 7, 8, 9);
//cout << "C = " << endl << " " << C << endl << endl;
//
//C.col(1) = C.col(1) / 2.0;
//cout << "C = " << endl << " " << C << endl << endl;
//return;
//
//
////reduce(InputArray src, OutputArray dst, int dim, int rtype, int dtype=-1 )¶
//cv::Mat				kkk;
//
//cv::reduce(C, kkk, 0, CV_REDUCE_SUM, -1);
////cv::reduce(C, kkk, 1, CV_REDUCE_MAX, -1);
//
//cout << "kkk = " << endl << " " << kkk << endl << endl;
//
//return;



// CUfxBoVW
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<








//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUcmlCCdata
//						//

CUcmlCCdata::CUcmlCCdata(cv::Mat& m2fData, cv::Mat& m2Label)
	: m_m2fBareData((cv::Mat_<float>&) m2fData),
		m_m2Label((cv::Mat_<int>&) m2Label),
		m_nBareDataLen(m2fData.cols)
{
	m_m2fChainData.create(m2fData.rows, m2fData.cols + m2Label.cols);
	cv::Mat				m2fLabel;
	m2Label.convertTo(m2fLabel, CV_32F);

	cout << m2fData.rows << " x " << m2fData.cols << "---" << m2fLabel.rows << " x "  << m2fLabel.cols 
		<< "---" << m_m2fChainData.rows << " x " << m_m2fChainData.cols << endl;
	cout << m2fData.depth() << "~" << m2fLabel.depth() << "~" << m_m2fBareData.depth() << endl;
	
	cv::hconcat(m2fData, m2fLabel, m_m2fChainData);
}

void
CUcmlCCdata::setDataExtCol(int i, cv::Mat& mvf)
{
	cv::Mat				mvfDataExtCol = m_m2fChainData.colRange(m_nBareDataLen + i, m_nBareDataLen + i+1);
	mvf.copyTo(mvfDataExtCol);
}


// CUcmlCCdata
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// CUmlSvmplex
//						//
void
CUmlSvmplex::train(const cv::Mat& m2fData, const vector<cv::Mat>& aaiLabels)
{
	for (int i = 0; i < aaiLabels.size(); i++) {
		(*this)[i]->setType(SVM::C_SVC);
		(*this)[i]->setKernel(SVM::LINEAR);
		(*this)[i]->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
		(*this)[i]->train(m2fData, cv::ml::ROW_SAMPLE, aaiLabels[i]);

		cout << "SVM training for class-" << i << " is finished" << endl << endl;
	}
}
void
CUmlSvmplex::train(CUcmlCCdata& xCmlCCdata)
{
	for (int i = 0; i < size(); i++) {
		(*this)[i]->setType(SVM::C_SVC);
		(*this)[i]->setKernel(SVM::LINEAR);
		(*this)[i]->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
		(*this)[i]->train(xCmlCCdata.data4Class(i), cv::ml::ROW_SAMPLE, xCmlCCdata.label4Class(i));
	}
}

bool
CUmlSvmplex::belongsClass(const cv::Mat& m1fData, const int iClass)
{
	return	(*this)[iClass]->predict(m1fData) == 1;
}

int
CUmlSvmplex::belongsClass(const cv::Mat& m1fData, const set<int>& giClass)
{
//	cout << "mData is of size " << mData.rows << " x " << mData.cols << endl;
//	cout << "pgiClass size = " << pgiClass.size() << endl;


	for (auto i = giClass.begin(); i != giClass.end(); i++) {
//	for (auto i : giClass) {
//		cout << "class is " << *i << endl;
		if (belongsClass(m1fData, *i))
			return	*i;
	}
	return	-1;
}

void
CUmlSvmplex::test(const cv::Mat& m2fData, const vector<set<int>>& agiClass, CUsetOfClass* pxClassSet)
{
	cout << "inside CUmlSvmplex::test_" << pxClassSet << endl;
	int					nHit = 0;
	for (int i = 0; i < m2fData.rows; i++) {
//		cout << "file-" << i << endl;
		int				iClass = belongsClass(m2fData.row(i), agiClass[i]);
		if ( iClass >= 0) {
			nHit++;
			pxClassSet->countUpClassMatch(iClass);
		}
//		cout << i << "\t" << nHit << endl;
	}

	cout << "hit = " << nHit << " out of " << m2fData.rows;

	pxClassSet->streamOut(0, "", EnumClassListingOpt::showMatch);
}


void
CUmlSvmplex::predict(CUcmlCCdata& xCmlCCdata, const vector<cv::Mat>& am1iLabels)
{
	cv::Mat				m2iLabel;
	cv::hconcat(am1iLabels, m2iLabel);
	//cout << "m2iLabel = " << endl;
	//cout << m2iLabel << endl << endl;
	//return;

	cv::Mat				m2iResponse;
	predict(xCmlCCdata, m2iResponse);
	//cout << "m2iResponse = " << endl;
	//cout << m2iResponse << endl << endl;
	//return;

	cv::Mat				m2Confusion;

	sumupConfusion(m2iLabel, m2iResponse, m2Confusion);
	//sumupConfusion(A, B, m2Confusion);
	cout << "m2Confusion = " << endl;
	cout << m2Confusion << endl << endl;
}

void
CUmlSvmplex::predict(const cv::Mat& m2fData, const vector<cv::Mat>& am1iLabels)
{
	cout << "Inside predict(~)" << endl;
	cout << "\thas " << size() << " SVMs" << endl;
	cout << "m2fData size = " << m2fData.rows << " x " << m2fData.cols << endl << endl;;

	cv::Mat				m2iLabel;
	cv::hconcat(am1iLabels, m2iLabel);
	//cout << "m2iLabel = " << endl;
	//cout << m2iLabel << endl << endl;
	//return;

	cv::Mat				m2iResponse;
	predict(m2fData, m2iResponse);
	//cout << "m2iResponse = " << endl;
	//cout << m2iResponse << endl << endl;
	//return;

	cv::Mat				m2Confusion;

	//cv::Mat_<float> A = (cv::Mat_<float>(3, 3) << 
	//	1, 0, 0,
	//	1, 0, 1,
	//	0, 1, 1);
	//cv::Mat_<float> B = (cv::Mat_<float>(3, 3) << 
	//	1, 0, 1,
	//	1, 0, 1,
	//	0, 1, 1);

	sumupConfusion(m2iLabel, m2iResponse, m2Confusion);
	//sumupConfusion(A, B, m2Confusion);
	cout << "m2Confusion = " << endl;
	cout << m2Confusion << endl << endl;
	return;


	cv::Mat				mvResult;
	for (int i = 0; i < am1iLabels.size(); i++) {
//		cout << "i = " << i << endl;

		(*this)[i]->predict(m2fData, mvResult);// , cv::ml::StatModel::RAW_OUTPUT);

		cv::Mat			m1Out;
		cout << "result = " << endl;
		cv::transpose(mvResult, m1Out);
		cout << m1Out << endl;

		cout << "SVM for class-" << i << " is finished" << endl << endl;
	}
}


void
CUmlSvmplex::predict(CUcmlCCdata& xCmlCCdata, cv::Mat& m2iResult)
{
	vector<cv::Mat>		amv;
	for (int i=0; i<size(); i++) {
		cv::Mat			mvResult;
		(*this)[i]->predict(xCmlCCdata.data4Class(i), mvResult);
		amv.push_back(mvResult);

		xCmlCCdata.setDataExtCol(i, mvResult);
	}
	cv::hconcat(amv, m2iResult);
}


void
CUmlSvmplex::predict(const cv::Mat& m2fData, cv::Mat& m2iResult)
{
	vector<cv::Mat>		amv;
	for (int i=0; i<size(); i++) {
		cv::Mat				mvResult;
		(*this)[i]->predict(m2fData, mvResult);
		amv.push_back(mvResult);
	}
	cv::hconcat(amv, m2iResult);
}

void
CUmlSvmplex::sumupConfusion(const cv::Mat& m2Label, const cv::Mat& m2Response, cv::Mat& m2Confusion)
{
	m2Confusion = cv::Mat::zeros(m2Label.cols, m2Label.cols, CV_32S);
	for (int i=0; i<m2Label.rows; i++)
		add2confusion(m2Label.row(i), m2Response.row(i), m2Confusion);
}

void
CUmlSvmplex::add2confusion(const cv::Mat& m1Label, const cv::Mat& m1Response, cv::Mat& m2Confusion)
{
	for (int i=0; i<m1Label.cols; i++) {
		if (!m1Label.at<int>(i))	continue;
		for (int j=0; j<m1Response.cols; j++) {
			if (!m1Response.at<int>(j))	continue;
			if (i==j  ||  !m1Label.at<int>(j))
				m2Confusion.at<int>(i, j)++;
		}
	}
}

// CUmlSvmplex
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


