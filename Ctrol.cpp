#include "stdafx.h"
#include "Ctrol.h"

#include <Windows.h> 
#include "TissueAlg.h"

#include <chrono>   

using namespace chrono;

#pragma  warning(disable:4996)

string file_name = "F:/project/TissueProject/nature/7a.bmp";//1�� L_up.png -> 8b.bmp�쳣 :��ģ��ƥ��ȷ��λ�ã�Сģ��ƥ�������Ϊ����


string file_name_1 = "F:/project/TissueProject/nature1/9a2.bmp";//1�� L_up.png -> 8b.bmp�쳣 :��ģ��ƥ��ȷ��λ�ã�Сģ��ƥ�������Ϊ����

string file_tep = "F:/project/TissueProject/nature/L_downs.png";
string file_seamtep = "F:/project/TissueProject/nature/n_seam_tmpture.png";
string cate_dir = "F:/project/TissueProject/nature";

string balckDownimg = "F:/project/TissueProject/nature1/9a1.bmp";
string balckDownimgPoly = "F:/project/TissueProject/nature1/9a1_poly.bmp";

string balckDownimgNolight = "F:/project/TissueProject/nature1/45.bmp";

string  afterFac = "F:/share/823afterFac/15.bmp";
string  savePlv = "F:/share/823afterFac/15_ply.bmp";



#define PI 3.1415926
#define  max_sizeFv 1000

#define CHECK_NG_SINGLE 0
Ctrol* pCtrol = NULL;
Ctrol::Ctrol()
{
	pCtrol = this;
	m_exit = false;
	//m_CtrolCAM = new CtrolCAMHelp;

	m_onMessageFSubShow = NULL;
	ptopAlg = new TissueAlg;
	ptextPro = new CtextPro;
	_runState = ctrState::runStateBuilder().Build();
	InitializeCriticalSection(&_csSaveimg);
}


void  itoasetInfo(int val, string info, vector<string>& outInfo)
{
	char charTmp[100] = { 0 };
	itoa(val, charTmp, 10);
	info = info + ": " + charTmp;
	outInfo.push_back(info);

}
wchar_t* C2W(const char* str)
{
	int len = MultiByteToWideChar(CP_OEMCP, 0, str, -1/*null terminated*/, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	MultiByteToWideChar(CP_OEMCP, 0, str, -1/*null terminated*/, wstr, len);
	return wstr;
}

BOOL StringToWString(const std::string &str, std::wstring &wstr)
{
	_bstr_t t = str.c_str();
	wchar_t* pwchar = (wchar_t*)t;

	wstr = pwchar;
	return true;
	int nLen = str.size();// .length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}
__int64 GetFolderSize(const wstring& strDir)
{
	__int64 nSize = 0;



	wstring strRootPath = strDir + L"\\";
	wstring strRoot = strRootPath + L"*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(strRoot.c_str(), &fd);
	if (INVALID_HANDLE_VALUE == hFind)
		return nSize;
	while (FindNextFile(hFind, &fd))
	{
		if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0)
			continue;
		wstring strPath = strRootPath + fd.cFileName;
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			nSize += GetFolderSize(strPath);
		else
		{
			HANDLE hFile = CreateFile(strPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE == hFile)
				continue;
			LARGE_INTEGER size;
			if (::GetFileSizeEx(hFile, &size))
			{
				nSize += size.QuadPart;
			}
			CloseHandle(hFile);
		}
	}
	FindClose(hFind);
	return nSize;
}

bool FindOrCreateDirectory(string& pszPathStr, int caminfoID, double& sizeMB)
{


	char pszPath[MAX_PATH] = { 0 };

	char charTmp[100] = { 0 };
	itoa(caminfoID, charTmp, 10);
	pszPathStr = pszPathStr + "\\Cm_" + charTmp;
	sprintf_s(pszPath, "%s", pszPathStr);

	bool ret = false;


	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(C2W(pszPath), &fd);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wstring wstr;
			StringToWString(pszPathStr, wstr);
			//__int64 sizeF = GetFolderSize(wstr);
			//sizeMB = (sizeF / (1024 * 1024));


			ret = true;
			break;
		}

	}
	if (!::CreateDirectory(C2W(pszPath), NULL))
	{
		//char szDir[MAX_PATH];
		//sprintf_s(szDir, sizeof(szDir), "����Ŀ¼[%s]ʧ��,����Ȩ��", pszPath);


		ret = false;

	}
	//	CloseHandle(hFind);
	return ret;
	//wstring wstr;
	// StringToWString(pszPathStr, wstr);
	// __int64 size = GetFolderSize(wstr);

}
void delfileDir(const string& strDirPath)
{
	string strPathTmp = strDirPath + "\\*.*";
	long lFile;
	string sAddPath;
	struct _finddata_t sDate;
	lFile = _findfirst(strPathTmp.c_str(), &sDate);
	if (lFile == -1)
	{
		printf("read file fail!\n");
		return;
	}
	printf("�ļ�: %s\\%s\n", strDirPath.c_str(), sDate.name);
	while (_findnext(lFile, &sDate) == 0)
	{
		if (sDate.attrib == _A_SUBDIR)
		{
			sAddPath = strDirPath;
			sAddPath += "\\";
			sAddPath += sDate.name;
			if (string(sDate.name) == "." || string(sDate.name) == "..")
			{
				continue;
			}
			delfileDir(sAddPath);
		}
		else
		{
			// ɾ���ļ�
			string strFile = strDirPath + "\\" + string(sDate.name);
			printf("ɾ���ļ�: %s\n", strFile.c_str());
			remove(strFile.c_str());
		}
	}
	_findclose(lFile);

	// ɾ���ļ���
	rmdir(strDirPath.c_str());
	printf("ɾ��Ŀ¼: %s\n", sAddPath.c_str());

}

bool getMapValue(ParaKey parkey, double& value)
{
	map<ParaKey, int>*	ppParaKey = &pCtrol->m_CameraConfig.m_mapParaKey;
	auto m = ppParaKey->find(parkey);
	if (m != ppParaKey->end())
	{
		value = m->second;
		return true;
	}
	value = 0;
	return false;
}

int SaveImga(int caminfoId, SYSTEMTIME sys, cv::Mat* rm)
{

	EnterCriticalSection(&pCtrol->_csSaveimg);
	int ret = 0;
	string apppath = pCtrol->m_CameraConfig.m_appPath;
	int indexinfoid = 0;
	for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
	{
		if (pCtrol->m_CameraV[i].caminfoIndex == caminfoId)
		{
			double value = 0;
			ParaKey saveDataLog(caminfoId, "�������", "��������");
			getMapValue(saveDataLog, value);
			if (value == 0)
			{
				ret = -1;
				break;
			}

			if (((*pCtrol).m_CameraV[i].cnt++) % (int)value != 0)
			{
				ret = -2;
				break;

			}

			if ((*pCtrol).m_CameraV[i].cnt > 100000) (*pCtrol).m_CameraV[i].cnt = 0;
			indexinfoid = i;
		}
	}
	if (ret < 0)
	{
		LeaveCriticalSection(&pCtrol->_csSaveimg);

		return ret;
	}

	DWORDLONG i64FreeBytesToCaller = 0;
	DWORDLONG i64TotalBytes = 0;
	DWORDLONG i64FreeBytes = 0;
	string tmpString = apppath.substr(0, 3);
	GetDiskFreeSpaceExA(tmpString.c_str()/*"F:\\"*/, (PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);
	int freeSizeGB = i64FreeBytes / (1024 * 1024 * 1024);
	if (freeSizeGB < 2)
	{
		LeaveCriticalSection(&pCtrol->_csSaveimg);
		return -3;
	}

	char szInfo[150] = { 0 };
	itoa(caminfoId, szInfo, 10);
	apppath = apppath + "\\Cm_" + szInfo;
	sprintf_s(szInfo, 128, "\\%02d_%02d_%02d_%02d_%02d_%04d.jpg", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	apppath = apppath + szInfo;
	cv::imwrite(apppath, *rm);
	LeaveCriticalSection(&pCtrol->_csSaveimg);

	return 0;
}

Ctrol::~Ctrol()
{
	m_exit = true;
	SetEvent(m_bufEvent[m_CameraConfig.m_MySEQ.size()]);
	for (int i = 0; i < m_CameraV.size(); i++)
	{
		CMvCamera*   pCMvCamera = m_CameraV[i].pCMvCamera;
		pCMvCamera->CloseDevice();
	}
	//DeleteCriticalSection(&_csSaveimg);

}
bool  Ctrol::initAlg()
{
	m_pwarpAlg = new warpAlg;
	m_pwarpAlg->m_pCameraConfig = &m_CameraConfig;
	m_pwarpAlg->p_MapParaKey = &m_CameraConfig.m_mapParaKey;

	m_pwarpAlg->appPath = m_CameraConfig.m_appPath;

	//m_pwarpAlg->pCmLog = m_CameraConfig.pm_CLog;
	bool ret = m_pwarpAlg->initAlg();
	string path = m_pwarpAlg->app_NormPath + "\\SS0SNormPath.jpg";
	m_pwarpAlg->ss0SNormMatback = cv::imread(path);

	path = m_pwarpAlg->app_NormPath + "\\SS1SNormPath.jpg";
	m_pwarpAlg->ss1SNormMatback = cv::imread(path);

	path = m_pwarpAlg->app_NormPath + "\\llSNormPath.jpg";
	m_pwarpAlg->llSNormMatback = cv::imread(path);

	return ret;
}

bool Ctrol::initCamero()
{
	bool retBool = false;
	m_CameraConfig.Load();
	//	pCmLog = m_CameraConfig.pm_CLog;
	for (int index = 0; index < m_CameraConfig.m_MySEQ.size(); index++)
	{
		CMvCamera *pCMvCamera = new CMvCamera;
		int memsize = m_CameraConfig.m_MySEQ[index]->m_hight*m_CameraConfig.m_MySEQ[index]->m_width*m_CameraConfig.m_MySEQ[index]->m_bitdepth;
		m_pMe[index] = new char[memsize];
		pCMvCamera->m_frameBuf = m_pMe[index];

		m_bufEvent[index] = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_sub2Event[index] = CreateEvent(NULL, FALSE, FALSE, NULL);


		pCMvCamera->m_frameEvent = m_bufEvent[index];

		pCMvCamera->m_psysTime = &m_imgeTime[index];

		m_img[index] = new Mat(m_CameraConfig.m_MySEQ[index]->m_hight, m_CameraConfig.m_MySEQ[index]->m_width, CV_8UC3);

		if (pCMvCamera->InitialMvDevice(m_CameraConfig.m_MySEQ[index]->m_Serio.c_str(), NULL))
		{

			//	pCMvCamera->m_nHeight = m_CameraConfig.m_MySEQ[index]->
			camCtr  tmpcamCtr;
			tmpcamCtr.pCMvCamera = pCMvCamera;
			tmpcamCtr.pmemery = pCMvCamera->m_frameBuf;
			tmpcamCtr.bufEvent = &m_bufEvent[index];// &pCMvCamera->m_frameEvent;
			tmpcamCtr.imgeTime = &m_imgeTime[index];// *pCMvCamera->m_psysTime;
			tmpcamCtr.img = m_img[index];
			tmpcamCtr.check_rusult = enumCamCheck::Check_OK;

			if (m_CameraConfig.m_MySEQ[index]->m_camInfo == "CAM_TOP")
			{
				tmpcamCtr.caminfoIndex = CAM_TOP;
			}
			else if (m_CameraConfig.m_MySEQ[index]->m_camInfo == "CAM_LONGSIDE")
			{
				tmpcamCtr.caminfoIndex = CAM_LONGSIDE;
			}
			else if (m_CameraConfig.m_MySEQ[index]->m_camInfo == "CAM_SHORTSIDE_0")
			{
				tmpcamCtr.caminfoIndex = CAM_SHORTSIDE_0;
			}
			else if (m_CameraConfig.m_MySEQ[index]->m_camInfo == "CAM_SHORTSIDE_1")
			{
				tmpcamCtr.caminfoIndex = CAM_SHORTSIDE_1;
			}


			//	pCMvCamera->SetExposureTime(NULL, m_CameraConfig.m_MySEQ[index]->m_exp);
			//	pCMvCamera->SetGain(NULL, m_CameraConfig.m_MySEQ[index]->m_gain);

			//tmpcamCtr.saveImg = m_CameraConfig.m_MySEQ[index]->m_SaveImg;
			tmpcamCtr.validSaveSize = m_CameraConfig.m_MySEQ[index]->m_validSaveSize;
			tmpcamCtr.cnt = m_CameraConfig.m_MySEQ[index]->m_cnt;

			pCMvCamera->SetWhiteBalance(NULL, m_CameraConfig.m_MySEQ[index]->m_WhiteBalance);




			m_CameraV.push_back(tmpcamCtr);

			string apppath = m_CameraConfig.m_appPath;
			double sizeBm = 0.0;
			bool issaveDir = FindOrCreateDirectory(apppath, tmpcamCtr.caminfoIndex, sizeBm);
			retBool = true;
		}
		else
		{
			delete  pCMvCamera;
			pCMvCamera = NULL;
		}
	}

	//�߳��˳��¼�

	m_bufEvent[m_CameraConfig.m_MySEQ.size()] = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (retBool)
	{
		startGetbuf();
	}

	m_SoftT2hardware = CreateEvent(NULL, FALSE, FALSE, NULL);
	return retBool;
}

void   Ctrol::startGetbuf()
{
	for (int i = 0; i < m_CameraV.size(); i++)
	{
		camCtr*  pcamCtr = &m_CameraV[i];
		CreateThread(NULL, 0, Ctrol::getFramloopE, pcamCtr, 0, NULL);

	}
}

DWORD WINAPI  Ctrol::getFramloopE(LPVOID pParam)
{
	camCtr* pcamCT = (camCtr*)pParam;
	HANDLE handle[2];

	handle[0] = pCtrol->m_bufEvent[pCtrol->m_CameraConfig.m_MySEQ.size()];
	handle[1] = *(pcamCT->bufEvent);

	int 	err = 0;

	while (!pCtrol->m_exit)
	{
		//Sleep(300);
		//	pCtrol->_runState->imgGetFCamDone_ = false;
		int nIndex = WaitForMultipleObjects(2, handle, FALSE, INFINITE);
		//	pCtrol->_runState = (ctrState::runStateBuilder().SetimgGetFCamDone().Build());
		if (nIndex < 0)
		{
			err = GetLastError();
		}
		if (nIndex == WAIT_OBJECT_0)
		{
			return 0;

		}
		else if (nIndex == WAIT_OBJECT_0 + 1)
		{
			// ���ڴ˴����show, �㷨��show����
			HANDLE thand = CreateThread(NULL, 0, pCtrol->imageProcess, pcamCT, 0, NULL);
			CloseHandle(thand);
		}
		//WaitForSingleObject(*pcamCT->bufEvent, INFINITE);		 

	}
	return 0;

}

enumCamCheck getPipelineCamCheck(CamIDType camId0)
{
	int ret = 0;
	for (int camIndex = 0; camIndex < pCtrol->m_CameraV.size(); camIndex++)
	{
		if (pCtrol->m_CameraV[camIndex].pCMvCamera->m_camIndex == camId0)
		{
			if (pCtrol->m_CameraV[camIndex].check_rusult == enumCamCheck::Check_NG)
			{
				pCtrol->m_CameraV[camIndex].check_rusult = enumCamCheck::Check_OK;
				return enumCamCheck::Check_NG;
			}
			else
			{
				return enumCamCheck::Check_OK;
			}
		}
	}

	return enumCamCheck::Check_NO_CAM;
}

DWORD WINAPI Ctrol::imageProcess(LPVOID pParam)
{

	camCtr* pcamCT = (camCtr*)pParam;
	imgInfo2Dlg myimgInfo2Dlg;
	myimgInfo2Dlg.width = pcamCT->pCMvCamera->m_nWidth;
	myimgInfo2Dlg.hight = pcamCT->pCMvCamera->m_nHeight;
	myimgInfo2Dlg.data = (uchar*)pcamCT->pmemery;
	myimgInfo2Dlg.imgeTime = pcamCT->imgeTime;
	myimgInfo2Dlg.camIndex = pcamCT->caminfoIndex;
	myimgInfo2Dlg.check_rusult = pcamCT->check_rusult;

	cv::Mat rm(myimgInfo2Dlg.hight, myimgInfo2Dlg.width, CV_8UC3);
	rm.data = (uchar*)myimgInfo2Dlg.data;
	SYSTEMTIME sys = *myimgInfo2Dlg.imgeTime;

	enumCamCheck frontCamCheckFLG = enumCamCheck::Check_NO_CAM;
	int  ret = -2;
	char szInfo[128] = { 0 };
	SYSTEMTIME  t1, t2;
	if (pCtrol->m_onMessageFSubShow != NULL && pCtrol->_runState->enterSetpar_ == true)
	{
		int camHandle = 0;
		for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
		{
			if (pCtrol->m_CameraV[i].caminfoIndex == pcamCT->caminfoIndex)
			{
				camHandle = i;
			}
		}
		pCtrol->m_onMessageFSubShow(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, camHandle);
	}

	pCtrol->m_onMessage(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, pcamCT->caminfoIndex);

	Mat outImg;
	vector<string> outInfo;
	vector<string> outInfoLOG;

	if (myimgInfo2Dlg.camIndex == CamIDType::CAM_LONGSIDE)
	{
		frontCamCheckFLG = getPipelineCamCheck(CamIDType::CAM_TOP);
		if (frontCamCheckFLG == enumCamCheck::Check_OK)outInfo.push_back("����������OK");
		if (frontCamCheckFLG == enumCamCheck::Check_NG)outInfo.push_back("���������� NG");
		if (frontCamCheckFLG == enumCamCheck::Check_NO_CAM)outInfo.push_back("�޶������");
	}

	if (myimgInfo2Dlg.camIndex == CamIDType::CAM_SHORTSIDE_1)
	{
		frontCamCheckFLG = getPipelineCamCheck(CamIDType::CAM_SHORTSIDE_0);
		if (frontCamCheckFLG == enumCamCheck::Check_OK)outInfo.push_back("�˷����0���OK");
		if (frontCamCheckFLG == enumCamCheck::Check_NG)outInfo.push_back("�˷����0��� NG");
		if (frontCamCheckFLG == enumCamCheck::Check_NO_CAM)outInfo.push_back("�޶˷����0");
	}
	if (enumCamCheck::Check_NG == frontCamCheckFLG)
	{
		ret = SaveImga((int)pcamCT->caminfoIndex, sys, &rm);
		if (ret == -3)outInfo.push_back("ʣ��ռ䲻��2G����ʧ��");
		if (ret == 0)outInfo.push_back("ͼƬ���ݱ���ɹ�");
		if (ret == -2)outInfo.push_back("ͼƬ����δ����");
		if (ret == -1)outInfo.push_back("δ����ͼƬ���ݱ���");

		pCtrol->m_onMessage(pCtrol->m_owner, MSG_INFO, (void*)&outInfo, pcamCT->caminfoIndex);
		return 0;
	}
	//front cam OK

	sprintf_s(szInfo, 128, "[%02d-%02d %02d:%02d:%02d:%04d]", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	outInfo.push_back(szInfo);

	GetLocalTime(&t1);


	if (pCtrol->_runState->closeSub_ != true)
	{
		outInfo.push_back("���ò�����....");
	}
	else
	{

		ret = pCtrol->m_pwarpAlg->algProcess(pcamCT->caminfoIndex, NULL, rm, outImg, outInfo, outInfoLOG);
		GetLocalTime(&t2);
		long int spendMs = pCtrol->TimeDiff(t2, t1);
		sprintf(szInfo, "���ʱ�� = %ld ms", spendMs);
		outInfo.push_back(szInfo);

		if (ret < 0)
		{
			pcamCT->check_rusult = enumCamCheck::Check_NG;
			outInfo.push_back("���  NG");
		}
		else
		{
			pcamCT->check_rusult = enumCamCheck::Check_OK;
			outInfo.push_back("���  OK");
		}
	}

	if (ret == 0)
	{
		if (pcamCT->caminfoIndex == CamIDType::CAM_LONGSIDE || pcamCT->caminfoIndex == CamIDType::CAM_SHORTSIDE_1)
		{
			pcamCT->pCMvCamera->SetoutSigle();
		}
	}
	ret = SaveImga((int)pcamCT->caminfoIndex, sys, &rm);
	if (ret == -3)outInfo.push_back("ʣ��ռ䲻��2G����ʧ��");
	if (ret == 0)outInfo.push_back("ͼƬ���ݱ���ɹ�");
	if (ret == -2)outInfo.push_back("ͼƬ����δ����");	
	if (ret == -1)outInfo.push_back("δ����ͼƬ���ݱ���");


	pCtrol->m_onMessage(pCtrol->m_owner, MSG_INFO, (void*)&outInfo, pcamCT->caminfoIndex);
	//pCtrol->LogString(pcamCT->caminfoIndex, outInfo); LOG�ж�ʧʱ�����
	//pCtrol->LogString(pcamCT->caminfoIndex, outInfoLOG);	
	return ret;
}
void Ctrol::LogString(int camIDinfo, vector<string> strV)
{
	/*for (size_t i = 0; i < m_pCameraConfig->m_MySEQ.size(); i++)
	{
	}*/
	double value = 0;
	ParaKey saveDataLog(camIDinfo, "�������", "��������");
	getMapValue(saveDataLog, value);
	if (value == 0) return;

	LogInstance()->logString(camIDinfo, strV);
}

__int64 Ctrol::TimeDiff(SYSTEMTIME left, SYSTEMTIME right)
{
	CTime tmLeft(left.wYear, left.wMonth, left.wDay, 0, 0, 0);
	CTime tmRight(right.wYear, right.wMonth, right.wDay, 0, 0, 0);

	CTimeSpan sp = tmLeft - tmRight;
	long MillisecondsL = (left.wHour * 3600 + left.wMinute * 60 + left.wSecond) * 1000 + left.wMilliseconds;
	long MillisecondsR = (right.wHour * 3600 + right.wMinute * 60 + right.wSecond) * 1000 + right.wMilliseconds;

	return  (__int64)sp.GetDays() * 86400000 + (MillisecondsL - MillisecondsR);//�˴����غ���
}

bool  Ctrol::initSetsubShow(void* owner, ONMessage onMessage)
{
	m_onMessageFSubShow = onMessage;
	if (m_onMessageFSubShow == NULL)
		return false;

	return true;
}
bool Ctrol::Init(void* owner, ONMessage onMessage)
{
	m_owner = owner;
	m_onMessage = onMessage;
	if (true != initCamero())
	{
		//SendMsg(MSG_ERROR, "��ʼ��ϵͳ����");
		return false;
	}
	return initAlg();
}
void Ctrol::OnlcMessageFromSub(void* owner, MessageType msgType, void* data, int camIndex)
{
	sub2Fun *_sub2Fun = NULL;
	if (data != NULL)
	{
		_sub2Fun = (sub2Fun *)data;
		_sub2Fun->resetpara = false;
	}

	if (msgType == MessageType::MSG_SubClose || msgType == MessageType::MSG_SubsubClose)
	{
		pCtrol->_runState = (ctrState::runStateBuilder().SetCloseSub().Build());
		return;
	}
	if (msgType == MessageType::MSG_SetPar)
	{
		pCtrol->_runState = (ctrState::runStateBuilder().enterSetPara().Build());
		return;
	}
	if (msgType == MessageType::MSG_SoftwareTrigger)
	{
		HANDLE h = CreateThread(NULL, 0, Ctrol::processSoftwareTrigger, NULL, 0, NULL);
		CloseHandle(h);
		pCtrol->_runState = (ctrState::runStateBuilder().SetSoftTrigger().Build());

		return;
	}
	if (msgType == MessageType::MSG_HardwareTrigger)
	{
		if (pCtrol->_runState->softTrigger_ == true)
		{
			pCtrol->_runState = (ctrState::runStateBuilder().SetHardTrigger().Build());
			WaitForSingleObject(pCtrol->m_SoftT2hardware, INFINITE);
		}
		for (int camIndex = 0; camIndex < pCtrol->m_CameraV.size(); camIndex++)
		{
			pCtrol->m_CameraV[camIndex].pCMvCamera->SetTriggerMode(NULL, 0);
		}
		return;
	}



	if (msgType == MessageType::MSG_ReSetAllGainExp)
	{
		pCtrol->_runState = (ctrState::runStateBuilder().enterSetPara().Build());
		camCtr* pcamCtr = NULL;
		bool ret = false;
		for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
		{
			pcamCtr = &pCtrol->m_CameraV[i];
			int camIdinfo = (*pCtrol).m_CameraV[i].caminfoIndex;

			ParaKey parkeyGain(camIdinfo, "�������", "gain");
			auto m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyGain);
			if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end()) ret = true;
			ret = pcamCtr->pCMvCamera->SetGain(NULL, m->second);

			Sleep(5);

			ParaKey parkeyExp(camIdinfo, "�������", "exp");
			m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyExp);
			if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end()) ret = true;
			ret = pcamCtr->pCMvCamera->SetExposureTime(NULL, m->second);
		}

		sub2Fun mysub2Fun;

		mysub2Fun.fparent = "�������";
		mysub2Fun.fpara = "ģ��";
		mysub2Fun.resetpara = true;

		string path;
		if (!pCtrol->m_pwarpAlg->llSNormMatback.empty())
		{
			pCtrol->m_pwarpAlg->llSNormMat = pCtrol->m_pwarpAlg->llSNormMatback;
			path = pCtrol->m_pwarpAlg->app_NormPath + "\\llSNormPath.jpg";
			cv::imwrite(path, pCtrol->m_pwarpAlg->llSNormMatback);

			mysub2Fun.cam_Index = CAM_LONGSIDE;
			memcpy(&pCtrol->m_sub2Fun, &mysub2Fun, sizeof(sub2Fun));

			HANDLE handdd = CreateThread(NULL, 0, Ctrol::processSubCmd, &pCtrol->m_sub2Fun, 0, NULL);
			CloseHandle(handdd);
		}

		if (!pCtrol->m_pwarpAlg->ss0SNormMatback.empty())
		{
			pCtrol->m_pwarpAlg->ss0SNormMat = pCtrol->m_pwarpAlg->ss0SNormMatback;
			path = pCtrol->m_pwarpAlg->app_NormPath + "\\SS0SNormPath.jpg";
			cv::imwrite(path, pCtrol->m_pwarpAlg->ss0SNormMatback);
			mysub2Fun.cam_Index = CAM_SHORTSIDE_0;
			memcpy(&pCtrol->m_sub2Fun, &mysub2Fun, sizeof(sub2Fun));

			HANDLE handdd = CreateThread(NULL, 0, Ctrol::processSubCmd, &pCtrol->m_sub2Fun, 0, NULL);
			CloseHandle(handdd);
		}

		if (!pCtrol->m_pwarpAlg->ss1SNormMatback.empty())
		{
			pCtrol->m_pwarpAlg->ss1SNormMat = pCtrol->m_pwarpAlg->ss1SNormMatback;
			path = pCtrol->m_pwarpAlg->app_NormPath + "\\SS1SNormPath.jpg";
			cv::imwrite(path, pCtrol->m_pwarpAlg->ss1SNormMatback);
			mysub2Fun.cam_Index = CAM_SHORTSIDE_1;

			memcpy(&pCtrol->m_sub2Fun, &mysub2Fun, sizeof(sub2Fun));

			HANDLE handdd = CreateThread(NULL, 0, Ctrol::processSubCmd, &pCtrol->m_sub2Fun, 0, NULL);
			CloseHandle(handdd);
		}


		return;
	}


	if (_sub2Fun == NULL) return;
	if (msgType == MessageType::MSG_SetOneGainExp)
	{
		//	_sub2Fun->
		pCtrol->_runState = (ctrState::runStateBuilder().enterSetPara().Build());
		camCtr* pcamCtr = NULL;
		bool ret = false;
		for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
		{
			int camIdinfo = (*pCtrol).m_CameraV[i].caminfoIndex;
			if (camIdinfo == _sub2Fun->cam_Index)
			{
				pcamCtr = &pCtrol->m_CameraV[i];
				ParaKey parkeyGain(camIdinfo, "�������", "gain");
				auto m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyGain);
				if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end())
				{
					ret = pcamCtr->pCMvCamera->SetGain(NULL, m->second);
				}

				ParaKey parkeyExp(camIdinfo, "�������", "exp");
				m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyExp);
				if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end())
				{
					ret = pcamCtr->pCMvCamera->SetExposureTime(NULL, m->second);
				}
			}
		}
		return;
	}
	memcpy(&pCtrol->m_sub2Fun, _sub2Fun, sizeof(sub2Fun));
	//pCtrol->_runState = (ctrState::runStateBuilder().SetParaing().Build());
	HANDLE handdd = CreateThread(NULL, 0, Ctrol::processSubCmd, &pCtrol->m_sub2Fun, 0, NULL);
	CloseHandle(handdd);
}

DWORD WINAPI  Ctrol::processSoftwareTrigger(LPVOID pParam)
{

	bool r = true;
	for (int camIndex = 0; camIndex < pCtrol->m_CameraV.size(); camIndex++)
	{
		r &= pCtrol->m_CameraV[camIndex].pCMvCamera->SetTriggerMode(NULL, 1);

	}
	while (!pCtrol->m_exit&& pCtrol->_runState->softTrigger_ == true && r)
	{
		for (int camIndex = 0; camIndex < pCtrol->m_CameraV.size(); camIndex++)
		{
			pCtrol->m_CameraV[camIndex].pCMvCamera->DoSoftWare(NULL);
		}

		Sleep(250);
	}

	SetEvent(pCtrol->m_SoftT2hardware);
	return 0;
}



void getEnum(int camInfoId, string fparent, string fpara, algEnum& algstep)
{


	if (camInfoId == CamIDType::CAM_TOP)
	{
		if (fparent == "��ֵ����ֵ")
		{
			algstep = topTopBV;
		}
		else
		{
			algstep = topOtherPara;
		}
	}

	if (camInfoId == CamIDType::CAM_LONGSIDE || camInfoId == CamIDType::CAM_SHORTSIDE_0 || camInfoId == CamIDType::CAM_SHORTSIDE_1)
	{

		if (fparent == "�������")
		{
			if (fpara == "ģ��")
			{
				algstep = lsNorm;
			}
		}
		else{ algstep = lsOtherPara; }
	}

}
DWORD WINAPI Ctrol::processSubCmd(LPVOID pParam)
{
	//	pCtrol->m_sub_debug = true;
	Mat outImg; bool ret = false;
	algEnum algstep = algEnum::algNULL;
	vector<string> outInfo,outInfoLOG;
	sub2Fun *_sub2Fun = (sub2Fun *)pParam;
	if (_sub2Fun == NULL) return 0;
	getEnum(_sub2Fun->cam_Index, _sub2Fun->fparent, _sub2Fun->fpara, algstep);

	//��ͬ�����ͬ���㷨����
	int camHandle = 0;
	camCtr* pcamCtr = NULL;
	for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
	{
		if (pCtrol->m_CameraV[i].caminfoIndex == _sub2Fun->cam_Index)
		{
			pcamCtr = &pCtrol->m_CameraV[i];
			camHandle = i;
		}
	}
	if (pcamCtr == NULL){ return -1; }

	pCtrol->_runState = (ctrState::runStateBuilder().SetParaing().Build());
	cv::Mat rm(pcamCtr->pCMvCamera->m_nHeight, pcamCtr->pCMvCamera->m_nWidth, CV_8UC3);
	memcpy(rm.data, pcamCtr->pmemery, pcamCtr->pCMvCamera->m_nHeight  *pcamCtr->pCMvCamera->m_nWidth * 3);

	if (rm.empty())
	{
		outInfo.push_back("�޲ɼ�����");
		pCtrol->ShowForsetParaing(&outImg, (CamIDType)camHandle, 1, &outInfo);
		return -1;
	}
	static bool processs = false;
	if (processs == true)
	{
		return 0;
	}
	processs = true;
	ret = pCtrol->m_pwarpAlg->algSetParaProcess(algstep, _sub2Fun->cam_Index, NULL, rm, outImg, outInfo, outInfoLOG, _sub2Fun->resetpara);
	//cv::imwrite("11outImg.jpg", outImg);
	itoasetInfo(_sub2Fun->cam_Index, "CamInfo", outInfo);


	//cv::imshow("outImg78", outImg);
	cv::waitKey(20);
	pCtrol->ShowForsetParaing(&outImg, (CamIDType)camHandle, outImg.channels(), &outInfo);


	SYSTEMTIME sys = *(*pcamCtr).imgeTime;
	SaveImga(_sub2Fun->cam_Index, sys, &rm);
	processs = false;
	return 0;
}


void  Ctrol::ShowForsetParaing(cv::Mat * poutImg, CamIDType camId, int depth, vector<string>* poutInfo)
{


	imgInfo2Dlg myimgInfo2Dlg;
	myimgInfo2Dlg.width = poutImg->cols;
	myimgInfo2Dlg.hight = poutImg->rows;


	myimgInfo2Dlg.data = (uchar*)poutImg->data;
	myimgInfo2Dlg.camIndex = camId;
	myimgInfo2Dlg.depth = depth;
	myimgInfo2Dlg.setparaInfo = poutInfo;

	//cv::imwrite("12345.jpg", *poutImg);
	//cv::waitKey(10);


	//typedef void(*ONMessage)(void* owner, MessageType msgType, void* data, int camIndex);
	//pCtrol->m_onMessageFSubShow(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, myimgInfo2Dlg.camIndex/* myimgInfo2Dlg.camIndex*/);
	pCtrol->m_onMessageFSubShow(pCtrol->m_owner, MSG_Video, &myimgInfo2Dlg, myimgInfo2Dlg.camIndex/* myimgInfo2Dlg.camIndex*/);

	return;

	//int camId = pCsetPar->m_pcurrentCamID;
	//void* mm = &myimgInfo2Dlg;
	imgInfo2Dlg* iminfo = &myimgInfo2Dlg;// (imgInfo2Dlg*)mm;
	cv::Mat rm;
	if (iminfo->depth == 1)
	{
		rm.create(iminfo->hight, iminfo->width, CV_8UC1);
	}
	else
	{
		rm.create(iminfo->hight, iminfo->width, CV_8UC3);
	}

	//memmove(rm.data, iminfo->data, iminfo->hight * iminfo->width * 3);
	rm.data = (uchar*)iminfo->data;

	//cv::imwrite("110110.jpg", rm);


	rm.data = (uchar*)poutImg->data;

	//cv::imwrite("011.jpg", rm);


	cv::Mat rr(*poutImg);
	//cv::imwrite("001.jpg", rr);



	//  ���� frame Ϊ��ͨ��ͼ��
	cv::Mat roiImg(*poutImg);
	//frame.rowRange(frame.rows / 2, frame.rows).
	//	colRange(frame.cols / 4, frame.cols * 3 / 4).
	//	copyTo(roiImg); // ��ȡROI����

	int widthStep = (roiImg.cols*roiImg.elemSize() + 3) / 4 * 4; // �������ֽ�����ʹ���ܹ���4����
	uchar *frameData = (uchar *)calloc(roiImg.rows*widthStep, sizeof(uchar)); // �����ڴ�
	memset(frameData, 0, roiImg.rows*widthStep);

	// ��һ��������
	uchar *p1, *p2;
	for (int i = 0; i < roiImg.rows; i++)
	{
		p1 = roiImg.data + i*roiImg.cols*roiImg.channels();
		p2 = frameData + i * widthStep;
		for (int j = 0; j < roiImg.cols; j++)
		{
			*(p2) = *(p1);
			*(p2 + 1) = *(p1 + 1);
			*(p2 + 2) = *(p1 + 2);
			p1 += 3;
			p2 += 3;
		}
	}

	rm.data = (uchar*)frameData;
	//	cv::imwrite("002.jpg", rm);

}
