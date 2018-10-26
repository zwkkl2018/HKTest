#pragma once
#include "CameraConfig.h"
#include "MvCamera.h"

#include"topPlaneAlg.h"

#include "TissueAlg.h"
#include "CtextPro.h"
#include "global.h"
#include "warpAlg.h"
//#include <opencv2\opencv.hpp>
using namespace cv;


class Ctrol;
class camCtr
{
public:
	camCtr()
	{
		pCMvCamera = NULL;
		pmemery = NULL;
		check_rusult = enumCamCheck::Check_NO_CAM;
	};

	~camCtr(){};
public:
	CMvCamera*   pCMvCamera;
	char*        pmemery;
	HANDLE*      bufEvent;
	SYSTEMTIME*  imgeTime;
	Mat*		 img;//？
	CamIDType    caminfoIndex;
	enumCamCheck	 check_rusult;//check ok,nG
	//int          saveImg;
	int          validSaveSize;//1 valid ,0 invalid
	int          cnt;
};

//class  CtrolCAMHelp
//{
//public:
//	CtrolCAMHelp(){}
//	~CtrolCAMHelp(){}
//
//	Ctrol* pCtrol;
//	int    camNO;
//
//};

class infoState
{
public:
	int infoType;
	string info;
};
class imgInfo2Dlg
{
public:
	imgInfo2Dlg(){}
	~imgInfo2Dlg(){}

	SYSTEMTIME*  imgeTime;
	int     width;
	int	    hight;
	int     depth;
	uchar*	data;
	enumCamCheck	 check_rusult;//check ok,nG
	CamIDType     camIndex;
	vector<infoState> infoV;
	vector<string>* setparaInfo;
};

class subInfo2Ctr
{
public:
	subInfo2Ctr(){}
	~subInfo2Ctr(){}

	MessageType message;
	int      value;
	void*	data;
	int     camIndex;


};


class ctrState {
public:
	class runStateBuilder;
	void showState() {

		if (softTrigger_) std::cout << "-softTrigger_";
		if (hardTrigger_) std::cout << "-hardTrigger_";
		if (enterSetpar_) std::cout << "-setpar_";
		if (closeSub_) std::cout << "-closeSub_";
		std::cout << std::endl;
	}
	//private:

	bool softTrigger_ = false;
	bool hardTrigger_ = false;
	bool enterSetpar_ = false;
	bool Setparing_ = false;

	bool closeSub_ = true;

	bool imgGetFCamDone_ = false;
	bool imgGetFLocalDone_ = false;


};


class ctrState::runStateBuilder {
public:
	runStateBuilder() { static ctrState instance; runState_ = &instance; }
	runStateBuilder& SetSoftTrigger() { runState_->softTrigger_ = true; runState_->hardTrigger_ = false; return *this; }
	runStateBuilder& SetHardTrigger() { runState_->hardTrigger_ = true; runState_->softTrigger_ = false; return *this; }
	runStateBuilder& enterSetPara() { runState_->closeSub_ = false; runState_->enterSetpar_ = true; runState_->Setparing_ = false; return *this; }
	runStateBuilder& SetParaing() { runState_->closeSub_ = false; runState_->Setparing_ = true; runState_->enterSetpar_ = false; return *this; }


	runStateBuilder& SetCloseSub() { runState_->closeSub_ = true; runState_->enterSetpar_ = false; runState_->Setparing_ = false;  return *this; }

	runStateBuilder& SetimgGetFCamDone() { runState_->imgGetFCamDone_ = true;   return *this; }
	runStateBuilder& SetimgGetFLocalDone() { runState_->imgGetFLocalDone_ = true;   return *this; }


	ctrState* Build() { return runState_; }
private:
	ctrState* runState_;
};


class Ctrol
{
public:
	Ctrol();
	~Ctrol();
public:
	CameraConfig		 m_CameraConfig;


	bool  initCamero();
	bool  initAlg();
	void  startGetbuf();

	//	runStateType  runState;
	//runStateType  lastrunState;
	ctrState* _runState;
	//vector<runStateType> runStateVector;
	static DWORD WINAPI getFramloopE(LPVOID pParam);

	static DWORD WINAPI imageProcess(LPVOID pParam);

	static DWORD WINAPI processSubCmd(LPVOID pParam);

	static DWORD WINAPI processSoftwareTrigger(LPVOID pParam);


	__int64 TimeDiff(SYSTEMTIME left, SYSTEMTIME right);
	void LogString(int camIDinfo, vector<string> strV);

	bool Init(void* owner, ONMessage onMessage);
	bool initSetsubShow(void* owner, ONMessage onMessage);

	void  ShowForsetParaing(cv::Mat * poutImg, CamIDType camId, int depth, vector<string>* poutInfo);
	static void OnlcMessageFromSub(void* owner, MessageType msgType, void* data, int camIndex);
	vector<char*>       m_camerBuf;
public:
	vector<camCtr>      m_CameraV;
	char*				m_pMe[maxCamcnt];
	HANDLE				m_bufEvent[maxCamcnt + 1];// 注意;m_bufEvent[6] 为线程退出事件

	HANDLE				m_sub2Event[maxCamcnt + 1];

	SYSTEMTIME			m_imgeTime[maxCamcnt];
	Mat*				m_img[maxCamcnt];
	//CtrolCAMHelp*		m_CtrolCAM;
private:
	bool m_exit;
	//	bool m_Softtrigger;
	HANDLE	 	m_SoftT2hardware;



public:

	ONMessage    m_onMessage;
	ONMessage    m_onMessageFSub;
	ONMessage    m_onMessageFSubShow;


	HANDLE		m_hVideo;
	void*		m_owner;
	subInfo2Ctr m_subInfo2Ctr;
	sub2Fun 	m_sub2Fun;
public:
	TissueAlg*  ptopAlg;
	CtextPro*   ptextPro;
	//	bool        m_sub_debug;

	warpAlg*     m_pwarpAlg;
	CRITICAL_SECTION  _csSaveimg;
	//CmLog*       pCmLog;
};

