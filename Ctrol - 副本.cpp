#include "stdafx.h"
#include "Ctrol.h"

#include <Windows.h> 
#include "TissueAlg.h"

#include <chrono>   

using namespace chrono;

#pragma  warning(disable:4996)

string file_name = "F:/project/TissueProject/nature/7a.bmp";//1、 L_up.png -> 8b.bmp异常 :大模板匹配确定位置，小模板匹配错误，判为不良


string file_name_1 = "F:/project/TissueProject/nature1/9a2.bmp";//1、 L_up.png -> 8b.bmp异常 :大模板匹配确定位置，小模板匹配错误，判为不良

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
	int nLen = (int)str.length();
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

bool FindOrCreateDirectory(string& pszPathStr, int caminfoID,double& sizeMB)
{
	char pszPath[MAX_PATH] = {0};

	char charTmp[100] = { 0 };
	itoa(caminfoID, charTmp, 10);	 
	pszPathStr = pszPathStr + "\\Cm_" + charTmp;
	sprintf_s(pszPath, "%s", pszPathStr);
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(C2W(pszPath), &fd);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wstring wstr;
			StringToWString(pszPathStr, wstr);
			sizeMB = GetFolderSize(wstr) / (1024 * 1024);
			return true;
		}
			
	}
	if (!::CreateDirectory(C2W(pszPath), NULL))
	{
		//char szDir[MAX_PATH];
		//sprintf_s(szDir, sizeof(szDir), "创建目录[%s]失败,请检查权限", pszPath);
		return false;
	}
	//wstring wstr;
	// StringToWString(pszPathStr, wstr);
	// __int64 size = GetFolderSize(wstr);

	//return true;
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
	printf("文件: %s\\%s\n", strDirPath.c_str(), sDate.name);
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
			// 删除文件
			string strFile = strDirPath + "\\" + string(sDate.name);
			printf("删除文件: %s\n", strFile.c_str());
			remove(strFile.c_str());
		}
	}
	_findclose(lFile);

	// 删除文件夹
	rmdir(strDirPath.c_str());
	printf("删除目录: %s\n", sAddPath.c_str());

}

 

void SaveImga(int caminfoId,SYSTEMTIME sys,cv::Mat* rm)
{ 
	for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
	{
		if (pCtrol->m_CameraV[i].caminfoIndex == caminfoId)
		{		 
			if (!pCtrol->m_CameraV[i].saveImg) return;			 
		}
	}	 

	string apppath = pCtrol->m_CameraConfig.m_appPath;
	double sizeBm = 0.0;
	if (FindOrCreateDirectory(apppath, caminfoId, sizeBm))
	{
		if (sizeBm>2*1024)
		{  
			delfileDir(apppath);			 
			return;
		}
		char   szInfo[128] = { 0 };
		sprintf_s(szInfo, 128, "\\%02d_%02d_%02d_%02d_%02d_%04d.jpg", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		apppath = apppath + szInfo;
		cv::imwrite(apppath, *rm);
	} 

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
}
bool  Ctrol::initAlg()
{
	m_pwarpAlg = new warpAlg;
	m_pwarpAlg->m_pCameraConfig = &m_CameraConfig;
	m_pwarpAlg->p_MapParaKey = &m_CameraConfig.m_mapParaKey;
	return m_pwarpAlg->initAlg();

}

bool Ctrol::initCamero()
{
	bool retBool = false;
	m_CameraConfig.Load();
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
			tmpcamCtr.check_rusult = CamCheck::Check_OK;

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

			tmpcamCtr.saveImg = m_CameraConfig.m_MySEQ[index]->m_SaveImg;

			pCMvCamera->SetWhiteBalance(NULL, m_CameraConfig.m_MySEQ[index]->m_WhiteBalance);
			m_CameraV.push_back(tmpcamCtr);
			retBool = true;
		}
		else
		{
			delete  pCMvCamera;
			pCMvCamera = NULL;
		}
	}

	//线程退出事件

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
			// 可在此处添加show, 算法与show隔离
			HANDLE thand = CreateThread(NULL, 0, pCtrol->imageProcess, pcamCT, 0, NULL);
			CloseHandle(thand);
		}
		//WaitForSingleObject(*pcamCT->bufEvent, INFINITE);		 

	}
	return 0;

}

bool getPipelineCamCheck(CamIDType camId0)
{
	for (int camIndex = 0; camIndex < pCtrol->m_CameraV.size(); camIndex++)
	{
		if (pCtrol->m_CameraV[camIndex].pCMvCamera->m_camIndex == camId0)
		{
			if (pCtrol->m_CameraV[camIndex].check_rusult == CamCheck::Check_NG)
			{
				pCtrol->m_CameraV[camIndex].check_rusult == CamCheck::Check_OK;
				return true;
			}
		}
	}
	return false;
}

DWORD WINAPI Ctrol::imageProcess(LPVOID pParam)
{

	camCtr* pcamCT = (camCtr*)pParam;
	imgInfo2Dlg myimgInfo2Dlg;
	myimgInfo2Dlg.width = pcamCT->pCMvCamera->m_nWidth;
	myimgInfo2Dlg.hight = pcamCT->pCMvCamera->m_nHeight;
	myimgInfo2Dlg.data = (void*)pcamCT->pmemery;
	myimgInfo2Dlg.imgeTime = pcamCT->imgeTime;
	myimgInfo2Dlg.camIndex = pcamCT->caminfoIndex;
	myimgInfo2Dlg.check_rusult = pcamCT->check_rusult;

	cv::Mat rm(myimgInfo2Dlg.hight, myimgInfo2Dlg.width, CV_8UC3);
	rm.data = (uchar*)myimgInfo2Dlg.data;
	SYSTEMTIME sys = *myimgInfo2Dlg.imgeTime;


	if (pCtrol->m_onMessageFSubShow != NULL && pCtrol->_runState->enterSetpar_ == true)
	{
		int camHandle = 0;
		//camCtr* pcamCtr = NULL;
		for (int i = 0; i < pCtrol->m_CameraV.size(); i++)
		{
			if (pCtrol->m_CameraV[i].caminfoIndex == pcamCT->caminfoIndex)
			{
				//pcamCtr = &pCtrol->m_CameraV[i];
				camHandle = i;
			}
		}
		pCtrol->m_onMessageFSubShow(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, camHandle);
		//pCtrol->m_onMessageFSubShow(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, pcamCT->camIndex);
	}

	pCtrol->m_onMessage(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, pcamCT->caminfoIndex);
	char tempChar[200] = { 0 };
	bool pipeLineCamCheck = false;
	Mat outImg;
	vector<string> outInfo;
	if (myimgInfo2Dlg.camIndex == CamIDType::CAM_LONGSIDE)
	{
		if (pipeLineCamCheck = getPipelineCamCheck(CamIDType::CAM_TOP))
		{
			sprintf(tempChar, "camTop check NG");
			outInfo.push_back(tempChar);
		}

	}

	if (myimgInfo2Dlg.camIndex == CamIDType::CAM_SHORTSIDE_1)
	{
		if (pipeLineCamCheck = getPipelineCamCheck(CamIDType::CAM_SHORTSIDE_0))
		{
			sprintf(tempChar, "CAM_SHORTSIDE_0 check NG");
			outInfo.push_back(tempChar);
		}
		else
		{
			sprintf(tempChar, "CAM_SHORTSIDE_0 check OK");
			outInfo.push_back(tempChar);
		}

	}
	if (pipeLineCamCheck)// true NG    需要修改成OK给信号
	{

#if CHECK_NG_SINGLE
		pcamCT->pCMvCamera->SetoutSigle();
#endif
		pCtrol->m_onMessage(pCtrol->m_owner, MSG_INFO, (void*)&outInfo, pcamCT->caminfoIndex);
		SaveImga((int)pcamCT->caminfoIndex, sys, &rm);
		return 0;
	}
	
	char   szInfo[128] = { 0 };	
	sprintf_s(szInfo, 128, "[%02d-%02d %02d:%02d:%02d:%04d]", sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	outInfo.push_back(szInfo);
	SYSTEMTIME  t1, t2;
	GetLocalTime(&t1);
	//string stime = szInfo;	
	int ret = pCtrol->m_pwarpAlg->algProcess(pcamCT->caminfoIndex, NULL, rm, outImg, outInfo);
	// time
	GetLocalTime(&t2);
	long int spendMs = pCtrol->TimeDiff(t2, t1);
	sprintf(tempChar, "run time %ld ms", spendMs);
	outInfo.push_back(tempChar);

#if CHECK_NG_SINGLE
	if (ret < 0)
	{
		pcamCT->check_rusult = CamCheck::Check_NG;
		outInfo.push_back("检测  NG");
		if (pcamCT->caminfoIndex == CamIDType::CAM_LONGSIDE || pcamCT->caminfoIndex == CamIDType::CAM_SHORTSIDE_1)
		{
			pcamCT->pCMvCamera->SetoutSigle();
		}

	}
	else
	{
		pcamCT->check_rusult = CamCheck::Check_OK;
		outInfo.push_back("检测  OK");
	}
#endif
#if !CHECK_NG_SINGLE

	if (ret < 0)
	{
		pcamCT->check_rusult = CamCheck::Check_NG;
		outInfo.push_back("检测  NG");

	}
	else
	{
		pcamCT->check_rusult = CamCheck::Check_OK;
		outInfo.push_back("检测  OK");

		if (pcamCT->caminfoIndex == CamIDType::CAM_LONGSIDE || pcamCT->caminfoIndex == CamIDType::CAM_SHORTSIDE_1)
		{
			pcamCT->pCMvCamera->SetoutSigle();
		}
	}
#endif

	pCtrol->m_onMessage(pCtrol->m_owner, MSG_INFO, (void*)&outInfo, pcamCT->caminfoIndex);
	SaveImga((int)pcamCT->caminfoIndex, sys,  &rm);

	return ret;
}


__int64 Ctrol::TimeDiff(SYSTEMTIME left, SYSTEMTIME right)
{
	CTime tmLeft(left.wYear, left.wMonth, left.wDay, 0, 0, 0);
	CTime tmRight(right.wYear, right.wMonth, right.wDay, 0, 0, 0);

	CTimeSpan sp = tmLeft - tmRight;
	long MillisecondsL = (left.wHour * 3600 + left.wMinute * 60 + left.wSecond) * 1000 + left.wMilliseconds;
	long MillisecondsR = (right.wHour * 3600 + right.wMinute * 60 + right.wSecond) * 1000 + right.wMilliseconds;

	return  (__int64)sp.GetDays() * 86400000 + (MillisecondsL - MillisecondsR);//此处返回毫秒
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
		//SendMsg(MSG_ERROR, "初始化系统错误");
		return false;
	}
	return initAlg();
}
void Ctrol::OnlcMessageFromSub(void* owner, MessageType msgType, void* data, int camIndex)
{
	sub2Fun *_sub2Fun = (sub2Fun *)data;
	if (msgType == MessageType::MSG_SubClose)
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

			ParaKey parkeyGain(camIdinfo, "相机参数", "gain");
			auto m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyGain);
			if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end()) ret = true;
			ret = pcamCtr->pCMvCamera->SetGain(NULL, m->second);

			Sleep(5);

			ParaKey parkeyExp(camIdinfo, "相机参数", "exp");
			m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyExp);
			if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end()) ret = true;
			ret = pcamCtr->pCMvCamera->SetExposureTime(NULL, m->second);
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
				ParaKey parkeyGain(camIdinfo, "相机参数", "gain");
				auto m = (*pCtrol).m_CameraConfig.m_mapParaKey.find(parkeyGain);
				if (m != (*pCtrol).m_CameraConfig.m_mapParaKey.end())
				{
					ret = pcamCtr->pCMvCamera->SetGain(NULL, m->second);
				}

				ParaKey parkeyExp(camIdinfo, "相机参数", "exp");
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
		if (fparent == "二值化阈值")
		{
			algstep = topTopBV;
		}
		else
		{
			algstep = topOtherPara;
		}
	}
	if (camInfoId == CamIDType::CAM_LONGSIDE)
	{
		if (fparent == "相机参数")
		{
			if (fpara == "模板")
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
	vector<string> outInfo;
	sub2Fun *_sub2Fun = (sub2Fun *)pParam;
	if (_sub2Fun == NULL) return 0;
	getEnum(_sub2Fun->cam_Index, _sub2Fun->fparent, _sub2Fun->fpara, algstep);

	//不同相机不同的算法流程
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
		outInfo.push_back("无采集数据");
		pCtrol->ShowForsetParaing(&outImg, (CamIDType)camHandle, 1, &outInfo);
		return -1;
	}

	ret = pCtrol->m_pwarpAlg->algSetParaProcess(algstep, _sub2Fun->cam_Index, NULL, rm, outImg, outInfo);

	itoasetInfo(_sub2Fun->cam_Index, "CamInfo", outInfo);
	pCtrol->ShowForsetParaing(&outImg, (CamIDType)camHandle, 1, &outInfo);
	SYSTEMTIME sys = *(*pcamCtr).imgeTime;
	SaveImga(_sub2Fun->cam_Index, sys, &rm);	 
	return 0;
}


void  Ctrol::ShowForsetParaing(cv::Mat * poutImg, CamIDType camId, int depth, vector<string>* poutInfo)
{


	imgInfo2Dlg myimgInfo2Dlg;
	myimgInfo2Dlg.width = poutImg->cols;
	myimgInfo2Dlg.hight = poutImg->rows;


	myimgInfo2Dlg.data = (void*)poutImg->data;
	myimgInfo2Dlg.camIndex = camId;
	myimgInfo2Dlg.depth = depth;
	myimgInfo2Dlg.setparaInfo = poutInfo;

	pCtrol->m_onMessageFSubShow(pCtrol->m_owner, MSG_Video, (void*)&myimgInfo2Dlg, myimgInfo2Dlg.camIndex/* myimgInfo2Dlg.camIndex*/);
}
