#pragma once
 
#include"IniFile.h"
#include <iostream>
#include"baseCommon.h"

#pragma warning(disable:4996)

#define maxCamcnt 5

enum CamIDType
{
	CAM_TOP,
	CAM_LONGSIDE,
	CAM_SHORTSIDE_0,
	CAM_SHORTSIDE_1,
};
enum enumCamCheck
{
	Check_OK,
	Check_NG,
	Check_NO_CAM,

};
//
//enum runStateType
//{
//	runState_initNOset,
//	runState_SoftwareTrigger,
//	runState_HardwareTrigger,
//	runState_SetPar,
//};
enum MessageType
{
	MSG_INFO,
	MSG_ERROR,
	MSG_LOG,
	MSG_StatusChanged,
	MSG_Video,


	MSG_Gain,
	MSG_Exp,

	MSG_TextSetImgHSVNormP,

	MSG_TextimgHSVNorm,
	MSG_TSThresholdtransform,
	MSG_TSprocessapproxPolyDP,
	MSG_TextgetROImatTrue,
	MSG_TextgetROImatFalse,
	MSG_TextimgFileter,
	MSG_TexsetImgDensityNormP,
	MSG_TextgetComperDensityFeature,
	MSG_TSprocessPointLineFeature,



	MSG_NULL,
	MSG_SubClose,
	MSG_SubsubClose,

	MSG_SetPar,
	MSG_ReSetAllGainExp,
	MSG_SetOneGainExp,

	MSG_SoftwareTrigger,
	MSG_HardwareTrigger,

};
typedef void(*ONMessage)(void* owner, MessageType msgType, void* data, int camIndex);



class ParaKey
{
public:
	int camID;
	string paraParentName;
	string paraName;
public:
	ParaKey(int camID, string  paraParentName, string paraName)
	{
		this->camID = camID;
		this->paraParentName = paraParentName;
		this->paraName = paraName;
	}
	~ParaKey(){}
	bool operator<(const ParaKey & pkey) const   // 两个const是必需的。 
	{
		if (camID < pkey.camID)return true;
		else if (camID == pkey.camID)
		{
			if (paraParentName < pkey.paraParentName)return true;
			else if (paraParentName == pkey.paraParentName)
			{
				if (paraName < pkey.paraName)	return true;
			}
		}
		return false;
	}
};


class oneDeFectPara
{
public:
	oneDeFectPara(){};
	~oneDeFectPara(){};
public:
	int    camIndex;
	string parentName;
	int    paraIndex;
	string name;
	int   paraMax;
	int   paraVal;

};
class DeFectPara
{
public:
	DeFectPara(){};
	~DeFectPara(){};
public:
	string defectName;
	int    defectParaCnt;
	//vector<oneDeFectPara> defectParaArr;
	vector<string>  defectParaArrStr;

};

class MySEQ
{
public:
	MySEQ(){};
	~MySEQ(){};
public:
	string m_Serio;
	string m_camInfo;
	string m_IP;
//	int    m_exp;

//	int    m_gain;

	int    m_width;
	int    m_hight;

	int    m_bitdepth;
//	int    m_ParCnt;
	int    m_WhiteBalance;
//	int    m_SaveImg;//0 no,!0 save
//	vector<int> m_paraArr;
	int    m_trigger;//1 hardvware, 0 soft
	int    m_validSaveSize;//1 valid,0 invalid
	int    m_cnt;
	int    m_DefectCnt;
	vector<DeFectPara> m_DeFectParaStrArr;

};

//std::map<int, std::function<int(int, int)>>   yyytest;
class CameraConfig
{
public:
	CameraConfig();
	~CameraConfig();

	void Load();
	void getApppath();
	void Save();
	map<ParaKey, int>	 m_mapParaKey;
public:
	INIFile m_inifile;
public:
	string m_cameraConfigFile;
	string m_appPath;
	vector<MySEQ*>  m_MySEQ;

	void  getDeFectPara();
	vector<oneDeFectPara>	 m_mulDeFectPara;
	//CmLog*  pm_CLog;
};
class sub2Fun
{
public:
	sub2Fun(){}
	~sub2Fun(){}
public:
	string  fparent;
	string  fpara;
	bool resetpara;
	int  val;
	int  val_max;
	int  cam_Index;
private:
};

